#include "node.h"
#include "pwm.h"

void setup_pwm() {
    Serial.println("INFO: setup outputs");
    for (int i = 0; i < num_pwms; i++) {
        alloc_pin(pwms[i].pin);
        pinMode(pwms[i].pin, OUTPUT);
        pwms[i].value = 0;
        send_state(pwms[i].name, pwms[i].value);
        pwms[i].set_time = millis();
    }
}

void update_pwm() {
    for (int i = 0; i < num_pwms; i++) {
        if ((pwms[i].value != 0) and (pwms[i].set_time + pwms[i].timeout < millis())) {
            Serial.println("INFO: " + pwms[i].name + " timed out.");
            pwms[i].value = 0;
            send_state(pwms[i].name, pwms[i].value);
        }
        for (int i = 0; i < num_pwms; i++) {
            analogWrite(pwms[i].pin, pwms[i].value);
        }
    }
}

void set_pwm_max(String name, int value) {
    for (int i = 0; i < num_pwms; i++) {
        if (pwms[i].name == name) {
            if (value <= pwms[i].max_value) {
                pwms[i].act_max_value = value;
            }
        }
    }
}

bool write_pwm(String name, int value, bool silent) {
    for (int i = 0; i < num_pwms; i++) {
        if (pwms[i].name == name) {
            pwms[i].set_time = millis();
            if (value > pwms[i].act_max_value) {
                pwms[i].value = pwms[i].act_max_value;
            } else {
                pwms[i].value = value;
            }
            if (!silent)
                send_state(name, pwms[i].value);
            return true;
        }
    }
    return false;
}

bool get_pwm(String name, int &value) {
    for (int i = 0; i < num_pwms; i++) {
        if (pwms[i].name == name) {
            value = pwms[i].value;
            return true;
        }
    }
    return false;
}
