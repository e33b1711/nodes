#ifndef __temp_h__
#define __temp_h__
#include <Arduino.h>

void update_temps();
void setup_temps();
void send_all_temps();
bool get_temp(String name, String &value);
bool get_temp(String name, float &value);
bool get_humi(String name, String &value);

enum sensor_t { DHT22_T, DS18B20_T };

void handle_dht22(int this_temp);
void start_ds18b20(int this_temp);
void handle_ds18b20(int this_temp);

struct temp_t {
    const String name;
    const uint8_t pin;
    const sensor_t sensor;
    const char *label;  // friendly name in home assistant, none: not announced
    float temp_value;
    float humi_value;
    unsigned long last_update;

    temp_t(String name, uint8_t pin, sensor_t sensor, const char *label = nullptr)
            : name(name),
              pin(pin),
              sensor(sensor),
              label(label),
              temp_value(NAN),
              humi_value(NAN),
              last_update(0) {}
};

extern const int num_temps;
extern const long period_t;  // every sensor is read once per period
extern temp_t temps[];

#endif