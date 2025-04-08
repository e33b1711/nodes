#ifndef __temp_h__
#define __temp_h__
#include <Arduino.h>

void update_temps();
void setup_temps();
bool get_temp(String name, String &String);
bool get_temp(String name, float &value);
bool get_humi(String name, String &String);

enum sensor_t {DHT22_T, DS18B20_T, SHT31_T};

void handle_dht22(int this_temp);
void handle_ds18b20(int this_temp);

struct temp_t {
    const String name;
    const uint8_t pin;
    const sensor_t sensor;
    float temp_value;
    float humi_value;
    unsigned long last_update;
};

extern const int num_temps;
extern const long period_t;
extern temp_t temps[];

#endif