#include "node.h"
#include "rollo.h"
#include "output.h"
#include "switch.h"

void setup_rollos() {
    for (int i = 0; i < num_rollos; i++) {
        send_state(rollos[i].name, rollos[i].value);
    }
}

void post_all_rollos() {
    static unsigned long last_time = millis();
    const unsigned long post_interval = 75000;
    if ((last_time + post_interval) < millis()) {
        last_time = millis();
        for (int i = 0; i < num_rollos; i++) {
            send_state(rollos[i].name, rollos[i].value);
        }
    }
}

void update_rollos() {
    for (int i = 0; i < num_rollos; i++) {
        String l_up = rollos[i].name + "_UP";
        String l_down = rollos[i].name + "_DO";

        if (rollos[i].value != rollos[i].last_value) {
            rollos[i].last_value = rollos[i].value;
            switch (rollos[i].value) {
                case 0:
                    write_output(l_up, 1);
                    write_output(l_down, 0);
                    rollos[i].stop_time = millis() + rollos[i].duration;
                    rollos[i].stop_pending = true;
                    break;
                case 100:
                    write_output(l_up, rollos[i].is_rollo);
                    write_output(l_down, 1);
                    rollos[i].stop_time = millis() + rollos[i].duration;
                    rollos[i].stop_pending = true;
                    break;
                default:
                    write_output(l_up, 0);
                    write_output(l_down, 0);
            }
        }

        if (rollos[i].stop_pending and rollos[i].stop_time < millis()) {
            rollos[i].stop_pending = false;
            write_output(l_up, 0);
            write_output(l_down, 0);
        }

        if (rollos[i].has_stops) {
            bool open = get_switch(rollos[i].switch_open);
            bool closed = get_switch(rollos[i].switch_closed);
            int new_val = 50;
            if (open and !closed)  // TODO polarity seems false. 100 is down not up
                new_val = 100;
            if (!open and closed)
                new_val = 0;
            static bool trapped = false;
            if (open and closed and !trapped) {
                trapped = true;
                Serial.println("ERROR Rollo with stops: open and closed");
            } else {
                trapped = false;
            }
            if (rollos[i].value != new_val) {
                rollos[i].value = new_val;
                rollos[i].last_value = new_val;
                send_state(rollos[i].name, rollos[i].value);
            }
        }
    }
    post_all_rollos();
}

bool write_rollo(String name, String val_str) {
    int value = convert_value(val_str);
    return write_rollo(name, value);
}

bool write_rollo(String name, int value) {
    for (int i = 0; i < num_rollos; i++) {
        if (rollos[i].name == name) {
            if (rollos[i].is_rollo)
                rollos[i].value = 50;
            if (value == 100)
                rollos[i].value = 100;
            if (value == 0)
                rollos[i].value = 0;
            if (!rollos[i].has_stops)
                send_state(rollos[i].name, rollos[i].value);
        }
    }
    return false;
}

bool get_rollo(String name, String &value) {
    for (int i = 0; i < num_rollos; i++) {
        if (rollos[i].name == name) {
            value = String(rollos[i].value, DEC);
            return true;
        }
    }
    return false;
}

int get_rollo(String name) {
    for (int i = 0; i < num_rollos; i++) {
        if (rollos[i].name == name) {
            return rollos[i].value;
        }
    }
    Serial.println("WARNING: no such rollo.");
    return 300;
}