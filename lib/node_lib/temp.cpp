#include "node.h"
#include "temp.h"

unsigned long s_time_t;
int this_temp = 0;

bool get_temp(String name, String &value) {
    for (int i = 0; i < num_temps; i++) {
        if (name == ("TI_" + temps[i].name)) {
            value = String(temps[i].temp_value);
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

void send_all_temps() {
    for (int i = 0; i < num_temps; i++) {
        send_state("TI_" + temps[i].name, String(temps[i].temp_value));
        if (temps[i].sensor == DHT22_T)
            send_state("HI_" + temps[i].name, String(temps[i].humi_value));
    }
}

// reads every sensor once at startup instead of waiting up to a whole period,
// the ds18b20 conversions run in parallel while the dht22 are read
static void read_all_temps() {
    const unsigned long conversion_time = 800;
    unsigned long start = millis();
    for (int i = 0; i < num_temps; i++)
        if (temps[i].sensor == DS18B20_T)
            start_ds18b20(i);
    for (int i = 0; i < num_temps; i++)
        if (temps[i].sensor == DHT22_T)
            handle_dht22(i);
    while (millis() - start < conversion_time) {
    }
    for (int i = 0; i < num_temps; i++)
        if (temps[i].sensor == DS18B20_T)
            handle_ds18b20(i);
}

void setup_temps() {
    Serial.println("INFO: setup temps");
    for (int i = 0; i < num_temps; i++) {
        alloc_pin(temps[i].pin);
        temps[i].temp_value = NAN;
        temps[i].humi_value = NAN;
    }
    read_all_temps();
    s_time_t = millis();
}

void update_temps() {
    static bool converting = false;
    const unsigned long conversion_time = 800;

    if (num_temps > 0) {
        if (converting) {
            if (millis() - s_time_t > conversion_time) {
                converting = false;
                handle_ds18b20(this_temp);
                this_temp = (this_temp + 1) % num_temps;
            }
            return;
        }
        // one sensor at a time
        if (millis() - s_time_t > (unsigned long)period_t / num_temps) {
            s_time_t = millis();
            switch (temps[this_temp].sensor) {
                case DHT22_T:
                    handle_dht22(this_temp);
                    break;

                case DS18B20_T:
                    start_ds18b20(this_temp);
                    converting = true;
                    return;

                default:
                    break;
            }
            this_temp = (this_temp + 1) % num_temps;
        }
    }
}
