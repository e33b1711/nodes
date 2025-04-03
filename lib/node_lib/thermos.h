#ifndef _thermos_h__
#define _thermos_h__
#include <Arduino.h>

void update_thermos();
void setup_thermos();
bool get_thermos(String name, String &String);
bool write_thermos(String name, String);

struct thermos_t {
    const String name;   // same as a temp on same unit
    const String valve;  // any valve
    const float lin_weight;
    const float int_weight;
    float target_temp;
    float int_value;
};

extern const int num_thermos;
extern thermos_t thermos[];

#endif