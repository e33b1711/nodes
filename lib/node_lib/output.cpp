#include "node.h"
#include "output.h"

void setup_outputs() {
    Serial.println("INFO: setup outputs");
    for (int i = 0; i < num_outputs; i++) {
        alloc_pin(outputs[i].pin);
        digitalWrite(outputs[i].pin, !outputs[i].invert);
        pinMode(outputs[i].pin, OUTPUT);
        outputs[i].value = 0;
        send_state(outputs[i].name, int(outputs[i].value));
    }
}

void update_outputs() {
    for (int i = 0; i < num_outputs; i++) {
        digitalWrite(outputs[i].pin, !outputs[i].invert ^ outputs[i].value);
    }
}

bool write_output(String name, int value, bool silent) {
    for (int i = 0; i < num_outputs; i++) {
        if (outputs[i].name == name) {
            outputs[i].set_time = millis();
            switch (value) {
                case 0:
                    outputs[i].value = false;
                    break;
                case 1:
                    outputs[i].value = true;
                    break;
                default:
                    outputs[i].value = not(outputs[i].value);
                    break;
            }
            if (!silent)
                send_state(name, outputs[i].value);
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
