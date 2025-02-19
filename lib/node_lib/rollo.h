#ifndef _rollo_h__
#define _rollo_h__
#include <Arduino.h>

void update_rollos();
void setup_rollos();

bool write_rollo(String name, int value, bool silent);
bool get_rollo(String name, int &value);

struct rollo_t {
    const String name;
    const String l_up;
    const String l_down;
    const int duration;
    const bool is_rollo;
    int value;
    int old_value;
    long stop_time;
    boolean stop_pending;
};

extern const int num_rollos;
extern rollo_t rollos[];

#endif