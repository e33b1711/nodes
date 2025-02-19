#ifdef __ds18b20__

#include "node.h"
#include "temp.h"

float tempHigh, tempLow;
unsigned long s_time_t;

#include <DS18B20.h>

DS18B20 ds(ds18b_pin);

void setup_temps() {
    s_time_t = millis() - period_t * 2;
    alloc_pin(ds18b_pin);
    update_temps();
}

void update_temps() {
    if ((s_time_t + period_t) < millis()) {
        s_time_t = millis();
        while (ds.selectNext()) {
            uint8_t address[8];
            ds.getAddress(address);
            for (int i = 0; i < num_temps; i++) {
                if (temps[i].pin == address[7]) {
                    temps[i].temp_value = ds.getTempC();
                    send_state(temps[i].name, String(temps[i].temp_value));
                    temps[i].last_update = millis();
                    continue;
                }
                Serial.println("WARNING: DS Address unknown.");
            }
        }
    }
    //ensure we got a recent update, or set NAN
    for (int i = 0; i < num_temps; i++) {
        if (temps[i].last_update + period_t * 2 < millis()) {
            Serial.println("WARNING: Temperature too old: " + temps[i].name);
            temps[i].temp_value = NAN;
            send_state(temps[i].name, String(temps[i].temp_value));
        }
    }
}

#endif