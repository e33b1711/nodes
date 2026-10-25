#include "node.h"
#include "output.h"
#include "pinio.h"

void setup_outputs() {
    Serial.println("INFO: setup outputs");
    for (int i = 0; i < num_outputs; i++) {
        alloc_pin(outputs[i].pin);
        pinio_write(outputs[i].pin, !outputs[i].invert);
        pinio_mode(outputs[i].pin, OUTPUT);
        outputs[i].value = 0;
        send_state(outputs[i].name, int(outputs[i].value));
    }
}

void send_all_outputs() {
    for (int i = 0; i < num_outputs; i++) {
        send_state(outputs[i].name, outputs[i].value);
    }
}

void post_all_outputs() {
    static unsigned long last_time = millis();
    const unsigned long post_interval = 80000;
    if ((millis() - last_time) > post_interval) {
        last_time = millis();
        send_all_outputs();
    }
}

void update_outputs() {
    for (int i = 0; i < num_outputs; i++) {
        pinio_write(outputs[i].pin, !outputs[i].invert ^ outputs[i].value);
    }
    post_all_outputs();
}

bool write_output(String name, String val_str) {
    int value = convert_value(val_str);
    return write_output(name, value);
}

bool fs_active = false;

bool failsafe_active() { return fs_active; }

// without heartbeat from home assistant outputs with a failsafe value take it,
// afterwards they get back the last value home assistant has set
void update_failsafe() {
    bool lost = ha_lost();
    if (lost == fs_active)
        return;
    fs_active = lost;
    if (lost)
        Serial.println(F("WARNING: no heartbeat from home assistant, failsafe on"));
    else
        Serial.println(F("INFO: heartbeat from home assistant, failsafe off"));
    for (int i = 0; i < num_outputs; i++) {
        if (outputs[i].failsafe == no_failsafe)
            continue;
        if (lost) {
            outputs[i].saved = outputs[i].value;
            outputs[i].value = outputs[i].failsafe;
        } else {
            outputs[i].value = outputs[i].saved;
        }
        send_state(outputs[i].name, outputs[i].value);
    }
}

bool write_output(String name, int value) {
    bool found = false;
    for (int i = 0; i < num_outputs; i++) {
        if (outputs[i].name == name) {
            outputs[i].set_time = millis();
            // during the failsafe a command only takes effect once it ends
            bool &target = (fs_active and outputs[i].failsafe != no_failsafe) ? outputs[i].saved
                                                                              : outputs[i].value;
            switch (value) {
                case 0:
                    target = false;
                    break;
                case 1:
                    target = true;
                    break;
                default:
                    target = not(target);
                    break;
            }
            send_state(name, outputs[i].value);
            found = true;
        }
    }
    return found;
}

bool get_output(String name, String &value) {
    for (int i = 0; i < num_outputs; i++) {
        if (outputs[i].name == name) {
            value = String(outputs[i].value, DEC);
            return true;
        }
    }
    return false;
}

bool get_output(String name, int &value) {
    for (int i = 0; i < num_outputs; i++) {
        if (outputs[i].name == name) {
            value = outputs[i].value;
            return true;
        }
    }
    return false;
}

int get_output(String name) {
    for (int i = 0; i < num_outputs; i++) {
        if (outputs[i].name == name) {
            return outputs[i].value;
        }
    }
    return 0;
}
