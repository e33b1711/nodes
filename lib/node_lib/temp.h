#ifndef _temp_h__
#define _temp_h__
#include <Arduino.h>

void update_temps();
void setup_temps();
bool get_temp(String name, String &String);
bool get_temp(String name, int &value);
bool get_humi(String name, String &String);

struct temp_t {
    const String name;
    const uint8_t pin;
    float temp_value;
    float humi_value;
    unsigned long last_update;
};

extern const int num_temps;
extern const long period_t;
extern temp_t temps[];
extern const int ds18b_pin;

#endif