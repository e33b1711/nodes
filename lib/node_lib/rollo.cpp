#include "node.h"
#include "rollo.h"
#include "output.h"
#include "switch.h"


void post_all_rollos() {
    static unsigned long last_time = millis();
    const unsigned long post_interval = 75000;
    if ((millis() - last_time) > post_interval) {
        last_time = millis();
        for (int i = 0; i < num_rollos; i++) {
            send_state(rollos[i].name, rollos[i].value);
        }
    }
}

// real rollos: the _UP relay powers the motor, the _DO relay sets the direction
void drive(rollo_t &r, int dir) {
    write_output(r.name + "_UP", dir != 0);
    write_output(r.name + "_DO", dir > 0);
    r.dir = dir;
    r.last_tick = millis();
}

int percent(rollo_t &r) {
    return r.known ? (int)((r.pos_ms * 100 + r.duration / 2) / r.duration) : 50;
}

void report(rollo_t &r) {
    r.last_report = millis();
    r.value = percent(r);
    send_state(r.name, r.value);
}

void start_move(rollo_t &r, int target) {
    // full travel time like a hand-operated switch, the end switch of the motor stops it
    if (target == 0 or target == 100) {
        r.start_time = millis();
        r.pending = -1;
        r.target = target;
        drive(r, target == 0 ? -1 : 1);
        return;
    }
    // position unknown: open completely first, or let the running calibration finish
    if (!r.known) {
        if (r.dir >= 0 or r.target != 0) {
            r.start_time = millis();
            r.target = 0;
            drive(r, -1);
        }
        r.pending = target;
        return;
    }
    r.pending = -1;
    r.target = target;
    long target_ms = (long)target * r.duration / 100;
    if (target_ms == r.pos_ms) {
        drive(r, 0);
        report(r);
        return;
    }
    drive(r, target_ms > r.pos_ms ? 1 : -1);
}

// real rollos are opened to calibrate the position
void setup_rollos() {
    for (int i = 0; i < num_rollos; i++) {
        send_state(rollos[i].name, rollos[i].value);
        if (rollos[i].is_rollo)
            start_move(rollos[i], 0);
    }
}

void update_tracked(rollo_t &r) {
    if (r.dir == 0)
        return;
    unsigned long now = millis();
    r.pos_ms = constrain(r.pos_ms + r.dir * (long)(now - r.last_tick), 0L, (long)r.duration);
    r.last_tick = now;

    bool done;
    if (r.target == 0 or r.target == 100) {
        done = (now - r.start_time) > (unsigned long)r.duration;
    } else {
        long target_ms = (long)r.target * r.duration / 100;
        done = r.dir > 0 ? r.pos_ms >= target_ms : r.pos_ms <= target_ms;
    }

    if (done) {
        drive(r, 0);
        if (r.target == 0 or r.target == 100) {
            r.pos_ms = r.target == 0 ? 0 : r.duration;
            r.known = true;
        }
        if (r.pending >= 0) {
            start_move(r, r.pending);
            return;
        }
        report(r);
    } else if ((now - r.last_report) > 2000 and percent(r) != r.value) {
        report(r);
    }
}

void update_rollos() {
    for (int i = 0; i < num_rollos; i++) {
        if (rollos[i].is_rollo) {
            update_tracked(rollos[i]);
            continue;
        }
        String l_up = rollos[i].name + "_UP";
        String l_down = rollos[i].name + "_DO";

        if (rollos[i].value != rollos[i].last_value) {
            rollos[i].last_value = rollos[i].value;
            switch (rollos[i].value) {
                case 0:
                    write_output(l_up, 1);
                    write_output(l_down, 0);
                    rollos[i].start_time = millis();
                    rollos[i].stop_pending = true;
                    break;
                case 100:
                    write_output(l_up, 0);
                    write_output(l_down, 1);
                    rollos[i].start_time = millis();
                    rollos[i].stop_pending = true;
                    break;
                default:
                    write_output(l_up, 0);
                    write_output(l_down, 0);
            }
        }

        if (rollos[i].stop_pending
            and (millis() - rollos[i].start_time) > (unsigned long)rollos[i].duration) {
            rollos[i].stop_pending = false;
            write_output(l_up, 0);
            write_output(l_down, 0);
        }

        if (rollos[i].has_stops) {
            bool open = get_switch(rollos[i].switch_open);
            bool closed = get_switch(rollos[i].switch_closed);
            int new_val = 50;
            if (open and !closed)
                new_val = 0;
            if (!open and closed)
                new_val = 100;
            if (open and closed) {
                if (!rollos[i].trapped)
                    Serial.println("ERROR: Rollo with stops: open and closed: " + rollos[i].name);
                rollos[i].trapped = true;
            } else {
                rollos[i].trapped = false;
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
        if (rollos[i].is_rollo and name == rollos[i].name + "_POS") {
            start_move(rollos[i], constrain(value, 0, 100));
            return true;
        }
        if (rollos[i].name == name and rollos[i].is_rollo) {
            if (value == 0 or value == 100) {
                start_move(rollos[i], value);
            } else {  // stop
                rollos[i].pending = -1;
                drive(rollos[i], 0);
                report(rollos[i]);
            }
            return true;
        }
        if (rollos[i].name == name) {
            if (value == 100)
                rollos[i].value = 100;
            if (value == 0)
                rollos[i].value = 0;
            if (!rollos[i].has_stops)
                send_state(rollos[i].name, rollos[i].value);
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

int get_rollo(String name) {
    for (int i = 0; i < num_rollos; i++) {
        if (rollos[i].name == name) {
            return rollos[i].value;
        }
    }
    Serial.println("WARNING: no such rollo.");
    return 300;
}