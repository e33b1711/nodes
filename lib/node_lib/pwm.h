#ifndef _pwm_h__
#define _pwm_h__

#include <Arduino.h>

void update_pwm();
void send_all_pwms();
void safe_pwm_pins();
void setup_pwm();
bool write_pwm(String name, String val_str);
bool get_pwm(String name, String &value);
void set_pwm_max(String name, int value);

struct pwm_t {
    const String name;
    const int pin;
    const int max_value;
    const unsigned long timeout;
    const char *label;  // friendly name in home assistant, none: not announced
    int value;
    int act_max_value;
    unsigned long set_time;

    pwm_t(String name, int pin, int max_value, unsigned long timeout, const char *label = nullptr)
            : name(name),
              pin(pin),
              max_value(max_value),
              timeout(timeout),
              label(label),
              value(0),
              act_max_value(max_value),
              set_time(0) {}
};

extern const int num_pwms;
extern pwm_t pwms[];

#endif