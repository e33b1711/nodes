#include "node.h"
#include "temp.h"

#ifdef __ard_temp__
#include <DHT22.h>

String divide10(int in) { return String(in / 10) + "." + String(in % 10); }

void handle_dht22(int i) {
    if (!(i < num_temps))
        return;
    DHT22 dht22(temps[i].pin);
    float t = dht22.getTemperature();
    float h = dht22.getHumidity();

    if (dht22.getLastError() == dht22.OK) {
        temps[i].temp_value = t;
        temps[i].humi_value = h;
    } else {
        temps[i].temp_value = NAN;
        temps[i].humi_value = NAN;
        Serial.println("ERROR: reading from dth22 failed: " + temps[i].name);
    }
    send_state("TI_" + temps[i].name, String(temps[i].temp_value));
    send_state("HI_" + temps[i].name, String(temps[i].humi_value));
}

#else

void handle_dht22(int i) {
    Serial.println("FAILURE: dth22 not implemented.");
    while (true) {} 
}

#endif
