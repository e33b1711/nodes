#include "node.h"

String input_buffer_debug = "";

bool get_message_debug() {
    while (Serial.available()) {
        // get the new byte:
        char inChar = Serial.read();
        // add it to the inputString:
        input_buffer_debug += inChar;
        if (inChar == '$') {
            return true;
        }
    }
    return false;
}

void send_state_debug(String name, int value) {
    String message = "!s!" + name + '!' + String(value, DEC) + "$\n";
    Serial.print(message);
}

void handle_message() {
    String type;
    String name;
    int value;
    parse_message(input_buffer, type, name, value);
    input_buffer = "";
    execute_message(type, name, value);
}

void send_state_debug(String name, String value) {
    String message = "!s!" + name + '!' + value + "$\n";
    Serial.print(message);
}

void handle_debug() {
    if (get_message_debug())
        handle_message();
}

void init_debug() {
    alloc_pin(0);
    alloc_pin(1);
    Serial.begin(9600);
    Serial.println("INFO: " + node_info.name + " starting...")
}