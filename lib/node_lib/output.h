#ifndef _output_h__
#define _output_h__

#include <Arduino.h>

void update_outputs();
void setup_outputs();
bool write_output(String name, int value, bool silent);
bool get_output(String name, int &value);

struct output_t {
    const String name;
    const int pin;
    const bool invert;
    bool value;
    unsigned long set_time;
};

extern const int num_outputs;
extern output_t outputs[];

#endif