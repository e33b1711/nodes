#ifndef _timer_h__
#define _timer_h__
#include <Arduino.h>
#include "node.h"

void setup_timers();
void handle_timers();

bool write_timer(String name, String val_str);
bool get_timer(String name, String &value);

struct ntimer_t {
    const String name;
    const String slave;
    bool value;
    bool running;
    unsigned long set_time;
    const unsigned long duration;
    const ha_t ha;      // HA_NONE: not announced to home assistant
    const char *label;  // friendly name in home assistant
};

extern const int num_timers;
extern ntimer_t timers[];

#endif