#include "node.h"
#include "rollo.h"

void setup_rollos() {
    for (int i = 0; i < num_rollos; i++) {
        send_state(rollos[i].name, rollos[i].value);
    }
}

void update_rollos() {
    for (int i = 0; i < num_rollos; i++) {
        if (rollos[i].value != rollos[i].old_value) {
            rollos[i].old_value = rollos[i].value;
            switch (rollos[i].value) {
                case 0:
                    write_any(rollos[i].l_up, 1, false);
                    write_any(rollos[i].l_down, 0, false);
                    rollos[i].stop_time = millis() + rollos[i].duration;
                    rollos[i].stop_pending = true;
                    break;
                case 100:
                    write_any(rollos[i].l_up, rollos[i].is_rollo, false);
                    write_any(rollos[i].l_down, 1, false);
                    rollos[i].stop_time = millis() + rollos[i].duration;
                    rollos[i].stop_pending = true;
                    break;
                default:
                    write_any(rollos[i].l_up, 0, false);
                    write_any(rollos[i].l_down, 0, false);
                    rollos[i].stop_time = rollos[i].stop_time;
                    rollos[i].stop_pending = false;
            }
        }
        if (rollos[i].stop_pending and rollos[i].stop_time < millis()) {
            rollos[i].stop_pending = false;
            write_any(rollos[i].l_up, 0, false);
            write_any(rollos[i].l_down, 0, false);
        }
    }
}

bool write_rollo(String name, int value, bool silent) {
    for (int i = 0; i < num_rollos; i++) {
        if (rollos[i].name == name) {
            if (rollos[i].is_rollo)
                rollos[i].value = 50;
            if (value == 100) {
                rollos[i].value = 100;
            }
            if (value == 0) {
                rollos[i].value = 0;
            }
            if (!silent)
                send_state(name, rollos[i].value);
            return true;
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