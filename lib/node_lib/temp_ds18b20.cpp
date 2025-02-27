#ifdef __ds18b20__

#include "node.h"
#include "temp.h"
#include <DS18B20.h>

DS18B20 ds(ds18b_pin);

void setup_temps() {
    Serial.println("INFO: setup temps ds18b");
    alloc_pin(ds18b_pin);
    update_temps();
}

void update_temps() {
    static unsigned long s_time_t;
    static bool initial = true;
    if (((s_time_t + period_t) < millis()) or initial) {
        initial = false;
        s_time_t = millis();
        // ensure we got a recent update, or set NAN
        for (int i = 0; i < num_temps; i++) {
            temps[i].temp_value = NAN;
        }
        while (ds.selectNext()) {
            uint8_t address[8];
            ds.getAddress(address);
            for (int i = 0; i < num_temps; i++) {
                if (temps[i].pin == address[7]) {
                    temps[i].temp_value = ds.getTempC();
                    temps[i].last_update = millis();
                    continue;
                } else {
                    Serial.println("WARNING: DS Address unknown.");
                }
            }
        }
        for (int i = 0; i < num_temps; i++) {
            send_state("TI_" + temps[i].name, String(temps[i].temp_value));
        }
    }
}

#endif