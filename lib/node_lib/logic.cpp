#include "node.h"
#include "switch.h"
#include "output.h"

// switch with long / short press
void long_short(const int b_index,
                const String l_long,
                const int l_value,
                const String l_short,
                const int s_value) {
    if (switches[b_index].release_late)
        write_any(l_long, String(l_value));
    if (switches[b_index].release_early)
        write_any(l_short, String(s_value));
}

// simple switch to output state
void simple(const int b_index, const int value, const String l) {
    if (switches[b_index].press) {
        write_any(l, String(value));
    }
}

// roolo switch click for complete up/down / hold and release for partial up/down
void rollo_lock(const int b_index, const int value, const String l) {
    if (switches[b_index].press) {
        write_any(l, String(value));
        Serial.println("DEBUG: rollo_lock press: " + l + " " + String(value));
    }
    if (switches[b_index].release_late) {
        write_any(l, String(50));
        Serial.println("DEBUG: rollo_lock release_late: " + l + " " + 50);
    }
}

// toggle a output that lives here and on another unit
void toggle_couple(const String output) {
    int value;
    if (!get_output(output, value)) {
        Serial.println("ERROR: toggle_couple: no such state.");
        return;
    }
    write_any(output, String(not(value)));
}

// toggle a output that lives here and on another unit on press
void toggle_couple(const int b_index, const String output) {
    if (switches[b_index].press)
        toggle_couple(output);
}