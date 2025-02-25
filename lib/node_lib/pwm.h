#ifndef _pwm_h__
#define _pwm_h__

#include <Arduino.h>

void update_pwm();
void setup_pwm();
bool write_pwm(String name, int value);
bool get_pwm(String name, String &value);
void set_pwm_max(String name, int value);

struct pwm_t {
    const String name;
    const int pin;
    const int max_value;
    const unsigned long timeout;
    int value;
    int act_max_value;
    unsigned long set_time;
};

extern const int num_pwms;
extern pwm_t pwms[];

#endif