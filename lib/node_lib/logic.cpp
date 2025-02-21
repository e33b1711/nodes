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
        write_any(l_long, l_value, false);
    if (switches[b_index].release_early)
        write_any(l_short, s_value, false);
}

// simple switch to output state
void simple(const int b_index, const int value, const String l) {
    if (switches[b_index].press) {
        write_any(l, value, false);
    }
}

// roolo switch click for complete up/down / hold and release for partial up/down
void rollo_lock(const int b_index, const int value, const String l) {
    if (switches[b_index].press)
        write_any(l, value, false);
    if (switches[b_index].release_late)
        write_any(l, 50, false);
}

// toggle a output that lives here and on another unit
void toggle_couple(const String output) {
    int value;
    if (!get_output(output, value)) {
        Serial.println("ERROR: toggle_couple: no such state.");
        return;
    }
    send_command(output, not(value));
}

// toggle a output that lives here and on another unit on press
void toggle_couple(const int b_index, const String output) {
    if (switches[b_index].press)
        toggle_couple(output);
}