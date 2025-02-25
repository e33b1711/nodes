#ifndef _switch_h__
#define _switch_h__
#include <Arduino.h>

void setup_switches();
void update_switches();
bool get_switch(String name, String &value);
int get_switch(String name);

struct switch_t {
    const String name;
    const bool is_public;
    const int pin;
    int value;
    int prev_value;
    unsigned long last_edge;
    bool edge;
    bool press;
    bool release_early;
    bool release_late;
};

extern const int num_switches;
extern switch_t switches[];

#endif