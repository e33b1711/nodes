#include "node.h"
#include "output.h"
#include "pinio.h"
#include <PubSubClient.h>
#include <avr/wdt.h>
#include <SPI.h>
#include <Ethernet.h>

EthernetClient netClient;
const int ethernet_sc_pin = 53;
const int ethernet_reset_pin = 12;

PubSubClient mqttClient(netClient);

unsigned long last_try_connect = 0;
bool initial_connect = true;
const int retry_period = 20000;
bool resend_pending = false;

// home assistant publishes this every minute, without it the outputs go to their failsafe value
const char *heartbeat_topic = "nodes/heartbeat";
const unsigned long heartbeat_timeout = 600000;
unsigned long last_heartbeat = 0;  // the timeout also runs from the start

bool ha_lost() { return millis() - last_heartbeat > heartbeat_timeout; }

void mqtt_callback(char *topic, byte *payload, unsigned int length);

// the W5100 reset hangs on D12 only, a watchdog reset of the mega leaves the chip as it is
const unsigned int link_reset_after = 3;  // failed connects in a row, about a minute
unsigned int failed_connects = 0;
unsigned int link_resets = 0;

void reset_link_hw() {
    Serial.println("INFO: Resetting ethernet chip.");
    pinio_write(ethernet_reset_pin, LOW);
    delay(1);  // the W5100 needs 2 us
    pinio_write(ethernet_reset_pin, HIGH);
    delay(200);  // until its PLL is stable
    wdt_reset();
}

void init_link() {
    static bool pins_allocated = false;
    if (!pins_allocated) {
        alloc_pin(ethernet_sc_pin);
        alloc_pin(ethernet_reset_pin);
        pinio_write(ethernet_reset_pin, HIGH);
        pinio_mode(ethernet_reset_pin, OUTPUT);
        reset_link_hw();  // a clean chip also after a watchdog reset
        pins_allocated = true;
    }

    if (Ethernet.linkStatus() == LinkOFF) {
        Serial.println("ERROR: Ethernet cable disconnected.");
        return;
    }

    Ethernet.init(ethernet_sc_pin);
    if (Ethernet.localIP() == IPAddress(0, 0, 0, 0)) {
        bool got_ip = Ethernet.begin(node_info.mac, 2000, 2000);
        wdt_reset();
        if (!got_ip) {
            Serial.println("ERROR: Got no IP from DHCP.");
            return;
        }
        Serial.println("INFO: IP: " + ip_address());
        // a send to a vanished peer gives up after 3 s (default 30 s), below the watchdog timeout
        Ethernet.setRetransmissionCount(3);
        start_ota();
    }
}

void send_state(String name, String value) {
    String topic = "nodes/state/" + name;
    Serial.println("INFO: Publish [" + topic + "]: " + value);
    mqttClient.publish(topic.c_str(), value.c_str());
}

void send_state(String name, int value) { send_state(name, String(value)); }

// retained and not limited by the PubSubClient buffer size
bool send_retained(String topic, String payload) {
    Serial.println("INFO: Publish retained [" + topic + "]");
    if (!mqttClient.beginPublish(topic.c_str(), payload.length(), true))
        return false;
    mqttClient.write((const uint8_t *)payload.c_str(), payload.length());
    return mqttClient.endPublish();
}

bool subscribe_topic(String topic) { return mqttClient.subscribe(topic.c_str()); }

bool unsubscribe_topic(String topic) { return mqttClient.unsubscribe(topic.c_str()); }

bool send_command(String name, String value) {
    if (!mqttClient.connected()) {
        return false;
    }
    String topic = "nodes/cmd/" + name;
    return mqttClient.publish(topic.c_str(), value.c_str());
}

bool send_command(String name, int value) { return send_command(name, String(value)); }

void execute_message(String type, String name, String val_str) {
    Serial.println("INFO: execute_message: " + type + " " + name + " " + val_str);
    if (type == "r") {
        String value_string;
        if (get_any(name, value_string))
            send_state(name, value_string);
    } else if (type == "c") {
        write_any_internal(name, val_str);
        remember_output(name);
    }
}

void mqtt_callback(char *topic, byte *payload, unsigned int length) {
    String message;
    for (unsigned int i = 0; i < length; i++) {
        message += (char)payload[i];
    }

    String topicStr = String(topic);

    if (topicStr == heartbeat_topic) {
        last_heartbeat = millis();
        return;
    }

    Serial.println("INFO: Received [" + topicStr + "]: " + message);

    // home assistant (re)started: it needs discovery and all states, sent from handle_comm
    if (topicStr == "homeassistant/status") {
        if (message == "online")
            resend_pending = true;
        return;
    }

    // Topic format: nodes/{cmd|read}/{name}
    int lastSlash = topicStr.lastIndexOf('/');
    if (lastSlash == -1)
        return;

    String name = topicStr.substring(lastSlash + 1);

    if (topicStr.indexOf("/cmd/") != -1) {
        execute_message("c", name, message);
    } else if (topicStr.indexOf("/read/") != -1) {
        execute_message("r", name, message);
    } else if (topicStr.indexOf("/soll/") != -1) {
        restore_output(name, message);
    }
}

bool maintain_connection() {
    if (mqttClient.connected()) {
        return true;
    }

    if ((millis() - last_try_connect > retry_period) or initial_connect) {
        initial_connect = false;
        last_try_connect = millis();
        init_link();

        Serial.println("INFO: Connecting to MQTT Broker...");

        // availability topic for home assistant, offline is sent by the broker as last will
        String statusTopic = "nodes/" + node_info.unit_name + "/status";

        bool connected = mqttClient.connect(node_info.unit_name.c_str(),
                                            statusTopic.c_str(),  // last will topic
                                            1,                    // last will qos
                                            true,                 // last will retain
                                            "offline");           // last will message

        wdt_reset();
        if (connected) {
            Serial.println("INFO: Connected to MQTT.");

            mqttClient.publish(statusTopic.c_str(), "online", true);

            mqttClient.subscribe("nodes/cmd/#");
            mqttClient.subscribe("nodes/read/#");
            mqttClient.subscribe("homeassistant/status");
            mqttClient.subscribe(heartbeat_topic);
            request_soll();
            mqtt_connects++;
            failed_connects = 0;

            send_discovery();
            send_diagnostics();

            return true;
        } else {
            Serial.print("ERROR: MQTT connection failed, rc=");
            Serial.println(mqttClient.state());
            // a hung chip still reports its old ip, so init_link alone does not help;
            // after the reset the ip is 0.0.0.0 and the next try starts with DHCP
            if (++failed_connects >= link_reset_after) {
                failed_connects = 0;
                link_resets++;
                reset_link_hw();
            }
        }
    }
    return false;
}

void setup_comm() {
    Serial.println("INFO: setup comm (MQTT)");
    init_link();

    mqttClient.setServer(server, port);
    mqttClient.setCallback(mqtt_callback);
    mqttClient.setSocketTimeout(4);  // stay below the watchdog timeout

    handle_comm();
}

void handle_comm() {
    if (maintain_connection()) {
        mqttClient.loop();
        if (resend_pending) {
            resend_pending = false;
            send_discovery();
            send_all_states();
            send_diagnostics();
        }
    }
}