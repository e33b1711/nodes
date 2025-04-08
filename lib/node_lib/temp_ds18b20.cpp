#include "node.h"
#include "temp.h"

#ifdef __ard_temp__

#include <DS18B20.h>

void handle_ds18b20(int this_temp) {
    DS18B20 ds(temps[this_temp].pin);

    temps[this_temp].temp_value = NAN;
    temps[this_temp].humi_value = NAN;

    while (ds.selectNext()) {
        temps[this_temp].temp_value = ds.getTempC();
        temps[this_temp].last_update = millis();
    }

    send_state("TI_" + temps[this_temp].name, String(temps[this_temp].temp_value));
}

#else

void handle_ds18b20(int i) {
    Serial.println("FAILURE: ds18b20 not implemented.");
    while (true) {} 
}

#endif