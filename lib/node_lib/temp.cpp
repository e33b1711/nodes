#ifndef __ds18b20__

#include "node.h"
#include "temp.h"
#include "SDHT.h"

SDHT dht;

unsigned long s_time_t;
int this_temp = 0;

String divide10(int in){
    return String(in/10) + "." + String(in % 10);
}

void handle_one_temp(int i) {
    if (!(i < num_temps))
        return;
    bool good = dht.read(DHT22, temps[i].pin);
    if (good) {
        send_state("TI_" + temps[i].name, divide10(dht.celsius));
        send_state("HI_" + temps[i].name, divide10(dht.humidity));
        temps[i].temp_value = dht.humidity;
        temps[i].humi_value = dht.humidity;
    } else {
        send_state("TI_" + temps[i].name, "NaN");
        send_state("HI_" + temps[i].name, "NaN");
        temps[i].temp_value = 9999;
        temps[i].humi_value = 9999;
        Serial.println("ERROR: reading from dth22 failed: " + temps[i].name);
    }

}

void setup_temps() {
    Serial.println("setup_temps");
    s_time_t = millis();
    for (int i = 0; i < num_temps; i++) {
        alloc_pin(temps[i].pin);
        handle_one_temp(i);
    }
}

void update_temps() {
    if (s_time_t + (period_t) < millis()) {
        s_time_t = millis();
        handle_one_temp(this_temp);
        this_temp = (this_temp + 1) % num_temps;
    }
}

bool get_temp(String name, int &value) {
    //cant handle float
    //wait for it!
    return false;
}

bool get_humi(String name, int &value) {
    //cant handle float
    //wait for it!
    return false;
}

#endif