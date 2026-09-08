#include "node.h"
#include <PubSubClient.h>

  #include <SPI.h>
  #include <Ethernet.h>
  EthernetClient netClient;
  const int ethernet_sc_pin = 53;
  const int ethernet_reset_pin = 12;

PubSubClient mqttClient(netClient);

unsigned long last_try_connect = 0;
const int retry_period = 5000;

void mqtt_callback(char* topic, byte* payload, unsigned int length);

// --- Git Revision senden ---
void send_git_revision() {
    Serial.println("INFO: Sending git revision.");
    static unsigned int entropy = 0;
    entropy++;
    String val = (dirty ? "dirty_" : "") + auto_version.substring(0, 8) + " " + String(entropy);
    send_state(node_info.unit_name + "/git_revision", val);
}

// --- Netzwerk-Interface initialisieren ---
void init_link() {
    static bool pins_allocated = false;
    if (!pins_allocated) {
        alloc_pin(ethernet_sc_pin);
        alloc_pin(ethernet_reset_pin);
        pins_allocated = true;
    }

    if (Ethernet.linkStatus() == LinkOFF) {
        Serial.println("ERROR: Ethernet cable disconnected.");
        return;
    }

    Ethernet.init(ethernet_sc_pin);
    if (Ethernet.localIP() == IPAddress(0,0,0,0)) {
        if (!Ethernet.begin(node_info.mac, 2000, 2000)) {
            Serial.println("ERROR: Got no IP from DHCP.");
            return;
        }
        Serial.println("INFO: IP: " + String(Ethernet.localIP()) );
    }
}

// --- MQTT Sender-Funktionen ---
void send_state(String name, String value) {
    String topic = "nodes/state/" + name;
    Serial.println("INFO: Publish [" + topic + "]: " + value);
    mqttClient.publish(topic.c_str(), value.c_str());
}

void send_state(String name, int value) {
    send_state(name, String(value));
}

void send_state(String name, float value) {
    send_state(name, String(value, 1));
}

bool send_command(String name, String value) {
    if (!mqttClient.connected()) {
        return false;
    }
    String topic = "nodes/cmd/" + name;
    return mqttClient.publish(topic.c_str(), value.c_str());
}

bool send_command(String name, int value) {
    return send_command(name, String(value));
}


void execute_message(String type, String name, String val_str) {
    Serial.println("INFO: execute_message: " + type + " " + name + " " + val_str);
    if (type == "r") {
        String value_string;
        if (get_any(name, value_string))
            send_state(name, value_string);
    } else if (type == "c") {
        write_any_internal(name, val_str);
    }
}

// --- Eingehende Nachrichten (Callback) ---
void mqtt_callback(char* topic, byte* payload, unsigned int length) {
    String message;
    for (unsigned int i = 0; i < length; i++) {
        message += (char)payload[i];
    }
    
    String topicStr = String(topic);
    Serial.println("INFO: Received [" + topicStr + "]: " + message);

    // Topic format: nodes/{unit_name}/{type}/{name}
    int lastSlash = topicStr.lastIndexOf('/');
    if (lastSlash == -1) return;

    String name = topicStr.substring(lastSlash + 1);
    
    if (topicStr.indexOf("/cmd/") != -1) {
        execute_message("c", name, message);
    } else if (topicStr.indexOf("/read/") != -1) {
        execute_message("r", name, message);
    }
}

// --- Verbindung verwalten ---
bool maintain_connection() {
    if (mqttClient.connected()) {
        return true;
    }

    if (millis() - last_try_connect > retry_period) {
        last_try_connect = millis();
        init_link();

        Serial.println("INFO: Connecting to MQTT Broker...");

        // Topic für Online/Offline Status definieren
        String statusTopic = "nodes/" + node_info.unit_name + "/status";

        // connect(clientID, willTopic, willQoS, willRetain, willMessage)
        bool connected = mqttClient.connect(
            node_info.unit_name.c_str(), // Client ID
            statusTopic.c_str(),         // Last Will Topic
            1,                           // Last Will QoS
            true,                        // Last Will Retain (auf true setzen)
            "offline"                    // Last Will Message
        );

        if (connected) {
            Serial.println("INFO: Connected to MQTT.");

            // 1. Online-Status senden (Retained = true)
            mqttClient.publish(statusTopic.c_str(), "online", true);

            // 2. Abonnieren der Befehle und Leseaufforderungen
            String subCmdTopic = "nodes/cmd/#";
            String subReadTopic = "nodes/read/#";

            mqttClient.subscribe(subCmdTopic.c_str());
            mqttClient.subscribe(subReadTopic.c_str());

            send_git_revision();
            return true;
        } else {
            Serial.print("ERROR: MQTT connection failed, rc=");
            Serial.println(mqttClient.state());
        }
    }
    return false;
}

// --- Setup & Loop Hooks ---
void setup_comm() {
    Serial.println("INFO: setup comm (MQTT)");
    init_link();
    
    mqttClient.setServer(node_info.server, node_info.port); // Default MQTT Port ist meist 1883
    mqttClient.setCallback(mqtt_callback);
    
    last_try_connect = millis() - retry_period*2;

    handle_comm();
}

void handle_comm() {
    if (maintain_connection()) {
        mqttClient.loop(); // Verarbeitet eingehende Nachrichten und hält Keep-Alive aufrecht
    }
}