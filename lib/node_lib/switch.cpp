#include "node.h"
#include "switch.h"

int this_switch = 0;

void setup_switches() {
    Serial.println("setup_switches");
    int i;
    for (i = 0; i < num_switches; i++) {
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
    int i = this_switch;
    // Serial.print("update_switches: ");
    // Serial.println(i);

    // read pin, store previous value
    switches[i].prev_value = switches[i].value;
    switches[i].value = invert(digitalRead(switches[i].pin));

    // get the edges
    int edge = get_edge(switches[i].value, switches[i].prev_value);
    switches[i].edge = edge;
    if (edge == 1) {
        switches[i].last_rising_edge = millis();
        Serial.print("update_abc: pos Flanke: ");
        Serial.println(i);
    }
    if (edge == -1) {
        switches[i].last_falling_edge = millis();
        Serial.println("update_abc: neg Flanke: ");
        Serial.println(i);
    }

    // count up
    this_switch = (this_switch + 1) % num_switches;
    if (this_switch == 0)
        user_logic();
}
