#include "node.h"

unsigned long last_try_connect = 0;
unsigned long last_keep_alive = 0;
const unsigned long keep_alive_period = 300000;
const int retry_period = 10000;
String message_buffer = "";
String input_buffer = "";

void send_git_revision() {
    Serial.println("INFO: Sending git revision.");
    static unsigned int entropy = 0;
    entropy++;
    if (dirty)
        send_state(node_info.unit_name,
                   "dirty_" + String(auto_version.substring(0, 8) + " " + String(entropy)));
    else
        send_state(node_info.unit_name,
                   String(auto_version.substring(0, 8) + " " + String(entropy)));
}

#ifdef __esp32__

#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>

WiFiClient client;
int conn_error_count = 0;

void print_bssid() {
    const uint8_t *bssid = WiFi.BSSID();
    Serial.print("INFO BSSID: ");
    for (int i = 0; i < 6; i++) {
        Serial.print(bssid[i], HEX);
        if (i < 5)
            Serial.print(":");
    }
    Serial.println("");
}

void init_link() {
    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("INFO: WIFI connected.");
        return;
    }

    WiFi.mode(WIFI_STA);
    WiFi.config(INADDR_NONE, INADDR_NONE, INADDR_NONE, INADDR_NONE);
    WiFi.setHostname(node_info.unit_name.c_str());  // define hostname
    WiFi.begin(ssid, pass, 6, bssid);

    int retries = 0;
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        retries++;
        if (retries == 4) {
            Serial.println("ERROR: Getting no WIFI");
            return;
        }
    }
    Serial.println("INFO: WiFi connected");
    Serial.print("INFO: IP address: ");
    Serial.println(WiFi.localIP());
    Serial.print("INFO: ESP32 HostName: ");
    Serial.println(WiFi.getHostname());
    Serial.print("INFO: RRSI: ");
    Serial.println(WiFi.RSSI());
    print_bssid();
    return;
}

bool connect_server() {
    if (!client.connect(node_info.server, node_info.port)) {
        Serial.println("ERROR: Server connection failed.");
        return false;
    }
    Serial.println("INFO: Connected to Server.");
    send_git_revision();
    return true;
}

#endif

#ifndef __esp32__
#include <SPI.h>
EthernetClient client;

const int ethernet_sc_pin = 53;
const int ethernet_reset_pin = 12;

void init_link() {
    static bool pins_allocated = false;
    if (!pins_allocated) {
        alloc_pin(ethernet_sc_pin);
        alloc_pin(ethernet_reset_pin);
        pins_allocated = true;
    }

    static bool link_up = false;
    if (link_up)
        return;

    Ethernet.init(ethernet_sc_pin);
    if (!Ethernet.begin(node_info.mac, 2000, 2000)) {
        Serial.println("ERROR: Got no IP from DHCP.");
    } else {
        Serial.print("INFO: IP: ");
        Serial.println(Ethernet.localIP());
        link_up = true;
    }
    delay(500);
    if (Ethernet.hardwareStatus() == EthernetNoHardware) {
        Serial.println("ERROR: Ethernet shield was not found.");
    }
}

bool connect_server() {
    while (Ethernet.linkStatus() == LinkOFF) {
        Serial.println("ERROR: Ethernet cable is not connected.");
        delay(500);
        return false;
    }
    if (!client.connect(node_info.server, node_info.port)) {
        Serial.println("ERROR: Server connection failed.");
        return false;
    }
    Serial.println("INFO: Connected to Server.");
    send_git_revision();
    return true;
}

#endif

void send_command(String name, int value) {
    String message = "!c!" + name + '!' + String(value, DEC) + "$\n";
    message_buffer += message;
}

void send_command(String name, String value) {
    String message = "!c!" + name + '!' + value + "$\n";
    message_buffer += message;
}

void send_state(String name, int value) {
    String message = "!s!" + name + '!' + String(value, DEC) + "$\n";
    Serial.print("INFO: send_state: " + message);
    message_buffer += message;
}

void send_state(String name, String value) {
    String message = "!s!" + name + '!' + value + "$\n";
    Serial.print("INFO: send_state: " + message);
    message_buffer += message;
}

void send_state(String name, float value) {
    String message = "!s!" + name + '!' + int(value) + "." + (int(value * 10.0) % 10) + "$\n";
    Serial.print("INFO: send_state: " + message);
    message_buffer += message;
}

void setup_comm() {
    Serial.println("INFO: setup comm");
    init_link();
    connect_server();
    message_buffer.reserve(100);
    last_try_connect = millis();
    last_keep_alive = millis();
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

void handle_message() {
    String type, name, val_str;
    parse_message(input_buffer, type, name, val_str);
    input_buffer = "";
    execute_message(type, name, val_str);
}

void send_messages() {
    if (client.connected() && message_buffer != "") {
        client.print(message_buffer);
        message_buffer = "";
    }
}

bool get_message() {
    while (client.available()) {
        char inChar = client.read();
        input_buffer += inChar;
        if (inChar == '$') {
            return true;
        }
    }
    return false;
}

bool maintain_connection() {
    if (client.connected()) {
        if (last_keep_alive + keep_alive_period < millis()) {
            send_git_revision();
            last_keep_alive = millis();
        }
        return true;
    }

    input_buffer = "";
    message_buffer = "";
    if (last_try_connect + retry_period < millis()) {
        last_try_connect = millis();
        Serial.println("INFO: Trying to recover server connection.");
        init_link();
        return connect_server();
    }
    return false;
}

void handle_comm() {
    if (maintain_connection()) {
        if (get_message())
            handle_message();
        send_messages();
    }
}
