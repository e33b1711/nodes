#include "node.h"
#include "valve.h"

unsigned long valve_last_time;
const unsigned long valve_interval = 60000;
const int valve_max = 256;

void setup_valves() {
    Serial.println("INFO: setup valves");
    for (int i = 0; i < num_valves; i++) {
        alloc_pin(valves[i].pin);
        send_state(valves[i].name, valves[i].value);
        digitalWrite(valves[i].pin, HIGH);
        pinMode(valves[i].pin, OUTPUT);
    }
    valve_last_time = millis();
}

void update_valves() {
    if (valve_last_time + valve_interval < millis()) {
        valve_last_time += valve_interval;
        for (int i = 0; i < num_valves; i++) {
            valves[i].sigmadelta += valves[i].value;
            if (valves[i].sigmadelta > 0) {
                digitalWrite(valves[i].pin, HIGH);
                valves[i].sigmadelta -= (valve_max);
                Serial.println("DEBUG: HIGH " + valves[i].name);
            } else {
                digitalWrite(valves[i].pin, LOW);
                Serial.println("DEBUG: LOW " + valves[i].name);
            }
           
            Serial.println("DEBUG: sigmadelta " + String(valves[i].sigmadelta));
        }
    }
}

bool write_valve(String name, int value, bool silent) {
    for (int i = 0; i < num_valves; i++) {
        if (valves[i].name == name) {
            if ((value >= 0) and (value <= valve_max)) {
                valves[i].value = value;
                if (valves[i].couple != "") write_valve(valves[i].couple, value, false);
            }
            if (!silent)
                send_state(name, valves[i].value);
            return true;
        }
    }
    return false;
}

bool get_valve(String name, String &value) {
    for (int i = 0; i < num_valves; i++) {
        if (valves[i].name == name) {
            value = String(valves[i].value);
            return true;
        }
    }
    return false;
}
