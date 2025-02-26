#include "node.h"
#include "switch.h"
#include "output.h"
#include "rollo.h"

// switch with long / short press
void long_short(const int b_index,
                const String l_long,
                const int l_value,
                const String l_short,
                const int s_value) {
    if (switches[b_index].release_late)
        write_output(l_long, l_value);
    if (switches[b_index].release_early)
        write_output(l_short, s_value);
}

// switch with long / short press
void long_short_extern(const int b_index,
                       const String l_long,
                       const int l_value,
                       const String l_short,
                       const int s_value) {
    if (switches[b_index].release_late)
        send_command(l_long, l_value);
    if (switches[b_index].release_early)
        send_command(l_short, s_value);
}

// simple switch to output state
void simple(const int b_index, const int value, const String l) {
    if (switches[b_index].press) {
        write_rollo(l, value);
    }
}

void simple_extern(const int b_index, const int value, const String l) {
    if (switches[b_index].press) {
        send_command(l, value);
    }
}

// roolo switch click for complete up/down / hold and release for partial up/down
void rollo_lock(const int b_index, const int value, const String l) {
    if (switches[b_index].press){
        write_rollo(l, value);
        Serial.println("DEBUG: rollo_lock press: " + l + " " + String(value));
    }
    if (switches[b_index].release_late){
        write_rollo(l, 50);
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
    send_command(output, not(value));
    write_output(output, not(value));
}

// toggle a output that lives here and on another unit on press
void toggle_couple(const int b_index, const String output) {
    if (switches[b_index].press)
        toggle_couple(output);
}