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
    const unsigned long duration;
    const ha_t ha;      // HA_NONE: not announced to home assistant
    const char *label;  // friendly name in home assistant
    bool value;
    bool running;
    unsigned long set_time;

    ntimer_t(String name, String slave, unsigned long duration, ha_t ha = HA_NONE, const char *label = nullptr)
        : name(name), slave(slave), duration(duration), ha(ha), label(label), value(false), running(false),
          set_time(0) {}
};

extern const int num_timers;
extern ntimer_t timers[];

#endif