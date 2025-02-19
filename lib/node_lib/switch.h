#ifndef _switch_h__
#define _switch_h__
#include <Arduino.h>

void setup_switches();
void update_switches();

struct switch_t {
    const int pin;
    int value;
    int prev_value;
    int edge;
    unsigned long last_falling_edge;
    unsigned long last_rising_edge;
};

extern const int num_switches;
extern switch_t switches[];

#endif