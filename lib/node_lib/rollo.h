#ifndef _rollo_h__
#define _rollo_h__
#include <Arduino.h>

void update_rollos();
void setup_rollos();

bool write_rollo(String name, int value);
bool get_rollo(String name, String &value);
int get_rollo(String name);

struct rollo_t {
    const String name;
    const int duration;
    const bool is_rollo;
    const bool has_stops;  //uses stop switches
    const String switch_open;
    const String switch_closed;
    int value;
    int last_value;
    unsigned long stop_time;
    boolean stop_pending;
};

extern const int num_rollos;
extern rollo_t rollos[];

#endif