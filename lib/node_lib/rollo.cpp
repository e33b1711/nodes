#include "node.h"
#include "rollo.h"
#include "output.h"

void setup_rollos() {
    for (int i = 0; i < num_rollos; i++) {
        send_state(rollos[i].name, rollos[i].value);
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
                    write_any(l_up, 1, false);
                    write_any(l_down, 0, false);
                    rollos[i].stop_time = millis() + rollos[i].duration;
                    rollos[i].stop_pending = true;
                    break;
                case 100:
                    write_any(l_up, rollos[i].is_rollo, false);
                    write_any(l_down, 1, false);
                    rollos[i].stop_time = millis() + rollos[i].duration;
                    rollos[i].stop_pending = true;
                    break;
                default:
                    write_any(l_up, 0, false);
                    write_any(l_down, 0, false);
            }
        }

        if (rollos[i].stop_pending and rollos[i].stop_time < millis()) {
            rollos[i].stop_pending = false;
            write_any(l_up, 0, false);
            write_any(l_down, 0, false);
        }

        if (rollos[i].has_stops) {
            bool open = get_output(rollos[i].switch_open);
            bool closed = get_output(rollos[i].switch_closed);
            int new_val = 50;
            if (open and !closed)
                new_val = 0;
            if (!open and closed)
                new_val = 0;
            if (open and closed)
                Serial.println("ERROR Rollo with stops:: both are HIGH.");
            if (rollos[i].value != new_val) {
                rollos[i].value = new_val;
                send_state(rollos[i].name, rollos[i].value);
            }
        }
    }
}

bool write_rollo(String name, int value, bool silent) {
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
    return 300;
}