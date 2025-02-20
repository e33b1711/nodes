#ifndef _thermos_h__
#define _thermos_h__
#include <Arduino.h>

void update_thermos();
void setup_thermos();
bool get_thermos(String name, String &String);
bool write_thermos(String name, int value, bool silent);

struct thermos_t {
    const String name;      //same as a temp on same unit
    const String valve;     //any valve 
    const float abs_weight;
    const float lin_weigth;
    const float int_weight;
    const unsigned long period;
    float target_temp;
    float int_value;
    unsigned long last_update;
};

extern const int num_thermos;
extern const long period_thermos;
extern thermos_t thermos[];

#endif