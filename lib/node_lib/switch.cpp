#include "node.h"
#include "switch.h"

int this_switch = 0;

void setup_switches() {
    Serial.println("INFO: setup switches");
    for (int i = 0; i < num_switches; i++) {
        alloc_pin(switches[i].pin);
        pinMode(switches[i].pin, INPUT);
        digitalWrite(switches[i].pin, HIGH);
    }
}

int invert(int val) {
    if (val == 1)
        return 0;
    return 1;
}

int get_edge(int value, int prev_value) {
    if (value == 1 && prev_value == 0)
        return 1;
    if (value == 0 && prev_value == 1)
        return -1;
    return 0;
}

String ident_string(int this_switch) {
    if (switches[this_switch].name == "")
        return "number: " + String(this_switch);
    return switches[this_switch].name;
}

void update_switches() {
    if (num_switches > 0) {
        switches[this_switch].prev_value = switches[this_switch].value;
        switches[this_switch].value = invert(digitalRead(switches[this_switch].pin));
        int edge = get_edge(switches[this_switch].value, switches[this_switch].prev_value);
        switches[this_switch].press = false;
        switches[this_switch].release_early = false;
        switches[this_switch].release_late = false;
        switches[this_switch].edge = (edge != 0);

        if (edge == 1) {
            switches[this_switch].press = true;
            Serial.println("INFO: Switch: " + ident_string(this_switch) + " pressed.");
        }
        if (edge == -1) {
            if (switches[this_switch].last_edge + 500 > millis()) {
                switches[this_switch].release_early = true;
                Serial.println("INFO: Switch: " + ident_string(this_switch) + " released early.");
            } else {
                switches[this_switch].release_late = true;
                Serial.println("INFO: Switch: " + ident_string(this_switch) + " released late.");
            }
            if (switches[this_switch].last_edge + 80000 < millis()) delay(10000); // restart by pressing switch 8 secs

        }
        if (edge != 0) {
            switches[this_switch].last_edge = millis();
            if (switches[this_switch].is_public)
                send_state(switches[this_switch].name, switches[this_switch].value);
        }

        this_switch = (this_switch + 1) % num_switches;
    }
    if (this_switch == 0)
        user_logic();
}

bool get_switch(String name, String &value) {
    for (int i = 0; i < num_switches; i++) {
        if (switches[i].name == name and switches[i].is_public) {
            value = String(switches[i].value, DEC);
            return true;
        }
    }
    return false;
}

int get_switch(String name){
    for (int i = 0; i < num_switches; i++) {
        if (switches[i].name == name) {
            return switches[i].value;
        }
    }
    return -99;
}