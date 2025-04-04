#ifndef _valve_h__
#define _valve_h__
#include <Arduino.h>

void setup_valves();
void update_valves();

bool write_valve(String name, String val_str);
bool write_valve(String name, int value);
bool get_valve(String name, String &value);

struct valve_t {
    const String name;
    const String couple;
    const int pin;
    int value;
    int sigmadelta;
};

extern const int num_valves;
extern valve_t valves[];

#endif