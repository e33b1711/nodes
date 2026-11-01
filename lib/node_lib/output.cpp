#include "node.h"
#include "output.h"
#include "pinio.h"
#include "rollo.h"
#include "timer.h"

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

bool fs_active = false;

// The last commanded value of each output is kept retained on nodes/soll/<name>. After every connect a
// node reads the values of its outputs from there: a restart keeps the lights and valves, and a command
// missed while the connection was gone is caught up. Outputs behind a timer or a rollo are left out,
// repeating those would start them again. Afterwards the topics are given up again, otherwise the
// echo of an older value could overtake a newer command.
const unsigned long soll_window = 10000;
bool soll_requested = false;
bool soll_open = false;
unsigned long soll_start = 0;

void request_soll() { soll_requested = true; }

bool keeps_soll(const output_t &output) {
    return !is_timer_slave(output.name) and !is_rollo_output(output.name);
}

void update_soll() {
    if (soll_requested) {
        soll_requested = false;
        soll_open = true;
        soll_start = millis();
        for (int i = 0; i < num_outputs; i++) {
            if (keeps_soll(outputs[i]))
                subscribe_topic("nodes/soll/" + outputs[i].name);
        }
    }
    if (soll_open and millis() - soll_start > soll_window) {
        soll_open = false;
        for (int i = 0; i < num_outputs; i++) {
            if (keeps_soll(outputs[i]))
                unsubscribe_topic("nodes/soll/" + outputs[i].name);
        }
    }
}

void restore_output(String name, String val_str) {
    if (!soll_open or (val_str != "0" and val_str != "1"))
        return;
    for (int i = 0; i < num_outputs; i++) {
        // a command since the connect is newer than the kept value
        if (outputs[i].name == name and keeps_soll(outputs[i]) and
            millis() - outputs[i].set_time >= millis() - soll_start) {
            write_output(name, val_str);
            return;
        }
    }
}

void remember_output(String name) {
    for (int i = 0; i < num_outputs; i++) {
        if (outputs[i].name == name and keeps_soll(outputs[i])) {
            bool value = (fs_active and outputs[i].failsafe != no_failsafe) ? outputs[i].saved
                                                                            : outputs[i].value;
            send_retained("nodes/soll/" + name, String(int(value)));
            return;
        }
    }
}

void update_outputs() {
    update_soll();
    for (int i = 0; i < num_outputs; i++) {
        pinio_write(outputs[i].pin, !outputs[i].invert ^ outputs[i].value);
    }
    post_all_outputs();
}

bool write_output(String name, String val_str) {
    int value = convert_value(val_str);
    return write_output(name, value);
}

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
