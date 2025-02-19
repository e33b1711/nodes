// handles ethernet
#include <SPI.h>
#include "node.h"

EthernetClient client;
long last_try_connect = 0;
const int retry_period = 60000;
String message_buffer = "";
String input_buffer = "";

void init_hw() {
    alloc_pin(node_info.ethernet_sc_pin);
    alloc_pin(node_info.ethernet_reset_pin);
    Ethernet.init(node_info.ethernet_sc_pin);
    Ethernet.begin(node_info.mac, node_info.ip);
    delay(500);
    if (Ethernet.hardwareStatus() == EthernetNoHardware) {
        Serial.println("FAILURE: Ethernet shield was not found.");
        while (true) delay(1);
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
    return true;
}

void setup_comm() {
    Serial.println("INFO: setup comm");
    init_hw();
    connect_server();
    message_buffer.reserve(100);
    last_try_connect = millis();
}

void send_command(String name, int value) {
    String message = "!c!" + name + '!' + String(value, DEC) + "$\n";
    message_buffer += message;
}

void send_state(String name, int value) {
    String message = "!s!" + name + '!' + String(value, DEC) + "$\n";
    message_buffer += message;
}

void send_state(String name, String value) {
    String message = "!s!" + name + '!' + value + "$\n";
    message_buffer += message;
}

void send_state(String name, float value) {
    String message = "!s!" + name + '!' + int(value) + "." + (int(value * 10.0) % 10) + "$\n";
    message_buffer += message;
}

void handle_couples(String name, int value) {
    for (int i = 0; i < num_couples; i++) {
        if (name == couples[i].name)
            write_any(name, value, true);
    }
}

int covert_value(String value_string) {
    if (value_string == "STOP")
        return 50;
    if (value_string == "UP")
        return 0;
    if (value_string == "DOWN")
        return 100;
    if (value_string == "ON")
        return 1;
    if (value_string == "OFF")
        return 0;
    return value_string.toInt();
}

void handle_message(String type, String name, int value) {
    if (type == "restart") {
        if (name == node_info.unit_name)
            delay(100000);
    } else if (type == "r") {
        if (get_any(name, value))
            send_state(name, value);
    } else if (type == "c") {
        write_any(name, value, false);
    } else if (type == "s") {
        handle_couples(name, value);
    }
}

void parse_message(String &type, String &name, int &value) {
    int index1 = input_buffer.indexOf('!');
    int index2 = input_buffer.indexOf('!', index1 + 1);
    int index3 = input_buffer.indexOf('!', index2 + 1);
    int index4 = input_buffer.indexOf('$');
    type = input_buffer.substring(index1 + 1, index2);
    name = input_buffer.substring(index2 + 1, index3);
    String value_string = input_buffer.substring(index3 + 1, index4);
    value = convert_value(value_string);
}

void handle_message() {
    String type;
    String name;
    int value;
    parse_message(type, name, value_string);
    handle_message(type, name, value);
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
        return true;
    } else {
        input_buffer = "";
        message_buffer = "";
        if (last_try_connect + retry_period < millis()) {
            last_try_connect = millis();
            Serial.println("INFO: Trying to recover server connection.");
            return connect_server();
        }
        return false;
    }
}

void handle_comm() {
    if (maintain_connection()) {
        if (get_message())
            handle_message();
        send_messages();
    }
}
