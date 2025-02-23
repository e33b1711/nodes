#include "node.h"
#include "temp.h"

bool get_temp(String name, String &value) {
    for (int i = 0; i < num_temps; i++) {
        if (name == ("TI_" + temps[i].name)) {
            value = String(temps[i].temp_value);
            return true;
        }
    }
    return false;
}

bool get_temp(String name, int &value) {
    for (int i = 0; i < num_temps; i++) {
        if (name == ("TI_" + temps[i].name)) {
            if (isnan(temps[i].temp_value))
                value = 9999;
            else
                value = (int)temps[i].temp_value;
            return true;
        }
    }
    return false;
}

bool get_temp(String name, float &value) {
    for (int i = 0; i < num_temps; i++) {
        if (name == ("TI_" + temps[i].name)) {
            value = temps[i].temp_value;
            return true;
        }
    }
    return false;
}

bool get_humi(String name, String &value) {
    for (int i = 0; i < num_temps; i++) {
        if (name == ("HI_" + temps[i].name)) {
            value = String(temps[i].humi_value);
            return true;
        }
    }
    return false;
}

#ifndef __ds18b20__
#ifndef __sht31__

#include <DHT22.h>

unsigned long s_time_t;
int this_temp = 0;

String divide10(int in) { return String(in / 10) + "." + String(in % 10); }

void handle_one_temp(int i) {
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

void setup_temps() {
    Serial.println("INFO: setup temps dht22");
    s_time_t = millis();
    for (int i = 0; i < num_temps; i++) {
        alloc_pin(temps[i].pin);
        handle_one_temp(i);
    }
}

void update_temps() {
    if (num_temps > 0) {
        if (s_time_t + (period_t) < millis()) {
            s_time_t = millis();
            handle_one_temp(this_temp);
            this_temp = (this_temp + 1) % num_temps;
        }
    }
}

#endif
#endif
