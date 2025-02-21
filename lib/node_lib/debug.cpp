#include "node.h"

String input_buffer_dg = "";

bool get_message_dg() {
    while (Serial.available()) {
        // get the new byte:
        char inChar = Serial.read();
        // add it to the inputString:
        input_buffer_dg += inChar;
        if (inChar == '$') {
            return true;
        }
    }
    return false;
}

void send_state_dg(String name, String value_string) {
    String message = "!s!" + name + '!' + value_string + "$\n";
    Serial.print(message);
}

void execute_message_dg(String type, String name, int value) {
    if (type == "restart") {
        if (name == node_info.unit_name)
            delay(100000);
    } else if (type == "r") {
        String value_string;
        if (get_any(name, value_string))
            send_state_dg(name, value_string);
    } else if (type == "c") {
        write_any(name, value, false);
    }
}

void handle_message_db() {
    String type;
    String name;
    int value;
    parse_message(input_buffer_dg, type, name, value);
    input_buffer_dg = "";
    execute_message(type, name, value);
}

void handle_debug() {
    if (get_message_dg())
        handle_message_db();
}

void init_debug() {
    alloc_pin(0);
    alloc_pin(1);
    Serial.begin(115200);
    Serial.println("INFO: " + node_info.unit_name + " starting...");
    Serial.println("INFO: GIT revision:" + auto_version);
#ifdef __TEST__
    Serial.println("WARNING: TEST BUILD!");
#else
    Serial.println("INFO: Productive Version.!");
#endif
}