#include "node.h"
#include "temp.h"

#include <DS18B20.h>

// the conversion takes up to 750 ms, so it is started here and read later
void start_ds18b20(int this_temp) {
    DS18B20 ds(temps[this_temp].pin);

    while (ds.selectNext()) {
        ds.startConversion();
    }
}

void handle_ds18b20(int this_temp) {
    DS18B20 ds(temps[this_temp].pin);

    temps[this_temp].temp_value = NAN;
    temps[this_temp].humi_value = NAN;

    while (ds.selectNext()) {
        temps[this_temp].temp_value = ds.readTempC();
        temps[this_temp].last_update = millis();
    }

    send_state("TI_" + temps[this_temp].name, String(temps[this_temp].temp_value));
}
