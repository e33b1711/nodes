#include "node.h"
#include "temp.h"

#include <DallasTemperature.h>

// the conversion takes up to 750 ms, so it is started here and read later
void start_ds18b20(int this_temp) {
    OneWire one_wire(temps[this_temp].pin);
    DallasTemperature ds(&one_wire);

    ds.begin();
    ds.setWaitForConversion(false);
    ds.requestTemperatures();
}

// one sensor per pin
void handle_ds18b20(int this_temp) {
    OneWire one_wire(temps[this_temp].pin);
    DallasTemperature ds(&one_wire);

    ds.begin();
    float t = ds.getTempCByIndex(0);

    temps[this_temp].humi_value = NAN;
    if (t == DEVICE_DISCONNECTED_C) {
        temps[this_temp].temp_value = NAN;
        Serial.println("ERROR: reading from ds18b20 failed: " + temps[this_temp].name);
    } else {
        temps[this_temp].temp_value = t;
        temps[this_temp].last_update = millis();
    }

    send_state("TI_" + temps[this_temp].name, String(temps[this_temp].temp_value));
}
