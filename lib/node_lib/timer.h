#ifndef _timer_h__
#define _timer_h__
#include <Arduino.h>

void setup_timers();
void handle_timers();

bool write_timer(String name, int value, bool silent);
bool get_timer(String name, int &value);

struct timer_t {
    const String name;
    const String slave;
    bool value;
    bool running;
    long set_time;
    const long duration;
};

extern const int num_timers;
extern timer_t timers[];

#endif