#ifndef _switch_h__
#define _switch_h__
#include <Arduino.h>

void setup_switches();
void send_all_switches();
void update_switches();
bool get_switch(String name, String &value);
int get_switch(String name);
String button_id(int i);

struct switch_t {
    const String name;
    const bool is_public;
    const int pin;
    const char *label;  // home assistant: sensor if public, else event entity; none: not announced
    int value;
    int prev_value;
    unsigned long last_edge;
    bool edge;
    bool press;
    bool release_early;
    bool release_late;

    switch_t(String name, bool is_public, int pin, const char *label = nullptr)
            : name(name),
              is_public(is_public),
              pin(pin),
              label(label),
              value(0),
              prev_value(0),
              last_edge(0),
              edge(false),
              press(false),
              release_early(false),
              release_late(false) {}
};

extern const int num_switches;
extern switch_t switches[];

#endif