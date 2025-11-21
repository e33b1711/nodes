#include "node.h"
#include "temp.h"

#ifdef __esp32__

#include <OneWire.h>
#include <DallasTemperature.h>

OneWire oneWire(temps[0].pin);
// Pass our oneWire reference to Dallas Temperature sensor
DallasTemperature sensors(&oneWire);

void discoverOneWireDevices(void) {
    byte i;
    byte present = 0;
    byte data[12];
    byte addr[8];

    Serial.print("INFO: Looking for 1-Wire devices...\n\r");
    while (oneWire.search(addr)) {
        Serial.print("\n\rINFO: Found \'1-Wire\' device with address:\n\r");
        Serial.print("INFO: ");
        for (i = 0; i < 8; i++) {
            Serial.print("0x");
            if (addr[i] < 16) {
                Serial.print('0');
            }
            Serial.print(addr[i], HEX);
            if (i < 7) {
                Serial.print(", ");
            }
        }
        if (OneWire::crc8(addr, 7) != addr[7]) {
            Serial.print("WARNING: CRC is not valid!\n");
            return;
        }
    }
    Serial.print("\n\r");
    oneWire.reset_search();
    return;
}

void handle_ds18b20(int this_temp) {
    static boolean init = true;
    if (init) {
        sensors.begin();
        init = false;
    }
    sensors.requestTemperatures();
    float temperatureC = sensors.getTempC(temps[this_temp].addr);

    temps[this_temp].temp_value = temperatureC;
    temps[this_temp].last_update = millis();

    send_state("TI_" + temps[this_temp].name, String(temps[this_temp].temp_value));
}

#else

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

#endif