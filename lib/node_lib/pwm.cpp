#include "node.h"
#include "pwm.h"
#include "pinio.h"

void setup_pwm() {
    Serial.println("INFO: setupPWM");
    for (int i = 0; i < num_pwms; i++) {
        alloc_pin(pwms[i].pin);
        pinio_mode(pwms[i].pin, OUTPUT);
        pwms[i].value = 0;
        send_state(pwms[i].name, pwms[i].value);
        pwms[i].set_time = millis();
    }
}

void post_all_pwms() {
    static unsigned long last_time = millis();
    const unsigned long post_interval = 65000;
    if ((millis() - last_time) > post_interval) {
        last_time = millis();
        for (int i = 0; i < num_pwms; i++) {
            send_state(pwms[i].name, pwms[i].value);
        }
    }
}

void update_pwm() {
    for (int i = 0; i < num_pwms; i++) {
        if ((pwms[i].value != 0) and ((millis() - pwms[i].set_time) > pwms[i].timeout)) {
            Serial.println("INFO: " + pwms[i].name + " timed out.");
            pwms[i].value = 0;
            send_state(pwms[i].name, pwms[i].value);
        }
        analogWrite(pwms[i].pin, pwms[i].value);
    }
    post_all_pwms();
}

void set_pwm_max(String name, int value) {
    for (int i = 0; i < num_pwms; i++) {
        if (pwms[i].name == name) {
            if (value <= pwms[i].max_value) {
                if (pwms[i].act_max_value != value) {
                    pwms[i].act_max_value = value;
                    Serial.println("INFO: set_pwm_max: " + pwms[i].name + " " + String(value, DEC));
                }
                if (pwms[i].value > pwms[i].act_max_value) {
                    pwms[i].value = pwms[i].act_max_value;
                    send_state(pwms[i].name, pwms[i].value);
                }
            }
        }
    }
}

bool write_pwm(String name, String val_str) {
    int value = convert_value(val_str);
    for (int i = 0; i < num_pwms; i++) {
        if (pwms[i].name == name) {
            pwms[i].set_time = millis();
            if (value > pwms[i].act_max_value) {
                pwms[i].value = pwms[i].act_max_value;
            } else {
                pwms[i].value = value;
            }
            send_state(name, pwms[i].value);
            return true;
        }
    }
    return false;
}

bool get_pwm(String name, String &value) {
    for (int i = 0; i < num_pwms; i++) {
        if (pwms[i].name == name) {
            value = String(pwms[i].value, DEC);
            return true;
        }
    }
    return false;
}
