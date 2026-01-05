#include "node.h"
#include "valve.h"
#include "pinio.h"

unsigned long valve_last_time;
const unsigned long valve_interval = 60000;
const int valve_max = 256;

void setup_valves() {
    Serial.println("INFO: setup valves");
    for (int i = 0; i < num_valves; i++) {
        alloc_pin(valves[i].pin);
        send_state(valves[i].name, valves[i].value);
        pinio_write(valves[i].pin, valves[i].invert);
        pinio_mode(valves[i].pin, OUTPUT);
    }
    valve_last_time = millis();
}

void post_all_valves() {
    static unsigned long last_time = millis();
    const unsigned long post_interval = 65000;
    if ((last_time + post_interval) < millis()) {
        last_time = millis();
        for (int i = 0; i < num_valves; i++) {
            send_state(valves[i].name, valves[i].value);
        }
    }
}

void update_valves() {
    if (valve_last_time + valve_interval < millis()) {
        valve_last_time += valve_interval;
        for (int i = 0; i < num_valves; i++) {
            valves[i].sigmadelta += valves[i].value;
            if (valves[i].sigmadelta > 0) {
                pinio_write(valves[i].pin, not(valves[i].invert));
                valves[i].sigmadelta -= (valve_max);
                Serial.println("DEBUG: ON " + valves[i].name);
            } else {
                pinio_write(valves[i].pin, valves[i].invert);
                Serial.println("DEBUG: OFF " + valves[i].name);
            }

            Serial.println("DEBUG: sigmadelta " + String(valves[i].sigmadelta));
        }
    }
    post_all_valves();
}

bool write_valve(String name, String val_str) {
    int value = convert_value(val_str);
    return write_valve(name, value);
}

bool write_valve(String name, int value) {
    for (int i = 0; i < num_valves; i++) {
        if (valves[i].name == name) {
            if ((value >= 0) and (value <= valve_max)) {
                valves[i].value = value;
                if (valves[i].couple != "")
                    write_valve(valves[i].couple, value);
            }
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
