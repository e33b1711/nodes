#ifdef __temp_sh31__

#include "node.h"
#include "temp.h"
#include "Wire.h"
#include "SHT31.h"

#define SHT31_ADDRESS 0x44

SHT31 sht;

void setup_temps() {
    Serial.println("INFO: setup temps sht31");
    if (num_temps == 0)
        return;
    if (num_temps > 1) {
        Serial.print("ERROR:only one sht31 supported");
        while (true) {
        }
    }
    Wire.begin();
    Wire.setClock(100000);
    sht.begin();
    uint16_t stat = sht.readStatus();
    Serial.print("INFO: status: ");
    Serial.print(stat, HEX);
    Serial.println();
    update_temps();
}

void update_temps() {
    static unsigned long s_time_t;
    static bool initial = true;
    if (num_temps == 0)
        return;
    if (((s_time_t + period_t) < millis()) or initial) {
        initial = false;
        s_time_t = millis();

        if (sht.read()) {
            temps[0].temp_value = sht.getTemperature();
            temps[0].humi_value = sht.getHumidity();
        } else {
            temps[0].temp_value = NAN;
            temps[0].humi_value = NAN;
            Serial.println("ERROR: reading temp failed");
        }
        send_state("TI_" + temps[0].name, String(temps[0].temp_value));
        send_state("HI_" + temps[0].name, String(temps[0].humi_value));
    }
}

#endif