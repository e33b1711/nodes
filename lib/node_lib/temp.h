#ifndef _temp_h__
#define _temp_h__
#include <Arduino.h>

void update_temps();
void setup_temps();
bool get_temp(String name, int &value);
bool get_humi(String name, int &value);

struct temp_t {
    const String name;
    const int pin;
    float temp_value;
    float humi_value;
    long last_update;
};

extern const int num_temps;
extern const long period_t;
extern temp_t temps[];

#endif