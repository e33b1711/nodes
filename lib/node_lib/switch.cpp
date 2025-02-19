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

void update_switches() {
    if (num_switches > 0) {
        switches[this_switch].prev_value = switches[this_switch].value;
        switches[this_switch].value = invert(digitalRead(switches[this_switch].pin));
        int edge = get_edge(switches[this_switch].value, switches[this_switch].prev_value);
        switches[this_switch].edge = edge;
        if (edge == 1)
            switches[this_switch].last_rising_edge = millis();
        if (edge == -1)
            switches[this_switch].last_falling_edge = millis();

        this_switch = (this_switch + 1) % num_switches;
    }
    if (this_switch == 0)
        user_logic();
}
