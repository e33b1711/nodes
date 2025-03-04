#include "node.h"
#include "timer.h"
#include "output.h"

void setup_timers() {
    Serial.println("INFO: setup timers");
    for (int i = 0; i < num_timers; i++) {
        timers[i].value = false;
        timers[i].running = false;
        send_state(timers[i].name, (int)timers[i].value);
    }
}

void update_timer(int i) {
    // start
    if (timers[i].value and !timers[i].running) {
        timers[i].running = true;
        timers[i].set_time = millis();
        write_output(timers[i].slave, 1);
    }
    // timer: running
    if (timers[i].value and timers[i].running) {
        if ((timers[i].set_time + 1000 * timers[i].duration) < millis()) {
            timers[i].running = false;
            timers[i].value = false;
            write_output(timers[i].slave, 0);
            write_output(timers[i].name, timers[i].value);
        }
    }
    // timer: external off
    // timer: running
    if (!timers[i].value and timers[i].running) {
        timers[i].running = false;
        write_timer(timers[i].slave, "0");
    }
}

void handle_timers() {
    for (int i = 0; i < num_timers; i++) update_timer(i);
}

bool write_timer(String name, String val_str) {
    int value = convert_value(val_str);
    for (int i = 0; i < num_timers; i++) {
        if (timers[i].name == name) {
            timers[i].set_time = millis();
            switch (value) {
                case 0:
                    timers[i].value = false;
                    break;
                case 1:
                    timers[i].value = true;
                    break;
                default:
                    timers[i].value = not(timers[i].value);
                    break;
            }
            send_state(name, timers[i].value);
            return true;
        }
    }
    return false;
}

bool get_timer(String name, String &value) {
    for (int i = 0; i < num_timers; i++) {
        if (timers[i].name == name) {
            value = String(timers[i].running);
            return true;
        }
    }
    return false;
}
