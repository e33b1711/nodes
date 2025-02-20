#include "node.h"
#include "thermos.h"
#include "temp.h"

const int half_valve = 128;
const int full_valve = 256;
const float base_valve = 32.0;

bool get_thermos(String name, String &value) {
    for (int i = 0; i < num_thermos; i++) {
        if (name == ("TS_" + thermos[i].name)) {
            value = String(thermos[i].target_temp);
            return true;
        }
    }
    return false;
}

float cutoff(float &in, const float max, const float min) {
    if (in > max) {
        return max;
    }
    if (in < min) {
        return min;
    }
    return in;
}

void update_one_thermos(int i) {
    if (!(i < num_thermos))
        return;

    float temperature;
    if (!get_temp("TI_" + thermos[i].name, temperature)) {
        Serial.println("ERROR: no such temp: " + (thermos[i].name));
        return;
    }

    if (temperature > 50 or temperature < -20 or isnan(temperature)) {
        Serial.println("ERROR: temp out of range. Setting 50%.");
        send_command(thermos[i].valve, half_valve);
        return;
    }

    float offset = thermos[i].target_temp * thermos[i].abs_weight;
    float linear = (thermos[i].target_temp - temperature) * thermos[i].abs_weight;
    thermos[i].int_value += (thermos[i].target_temp - temperature) * thermos[i].int_weight;
    thermos[i].int_value = cutoff(thermos[i].int_value, half_valve, half_valve * -1.0);

    float setpoint = base_valve + offset + linear + thermos[i].int_value;
    int i_setpoint = (int)cutoff(setpoint, full_valve, 0.0);
    send_command(thermos[i].valve, i_setpoint);

    Serial.println("INFO: update thermos " + thermos[i].name);
    Serial.println("INFO: temperature:   " + String(temperature));
    Serial.println("INFO: target_temp:   " + String(thermos[i].target_temp));
    Serial.println("INFO: offset:        " + String(offset));
    Serial.println("INFO: linear:        " + String(linear));
    Serial.println("INFO: integral:      " + String(thermos[i].int_value));
    Serial.println("INFO: setpoint:      " + String(i_setpoint, DEC));
}

void setup_thermos() {
    Serial.println("INFO: setup thermos");
    update_thermos();
}

void update_thermos() {
    static unsigned long s_time_t;
    static bool initial = true;
    if (((s_time_t + period_t) < millis()) or initial) {
        initial = false;
        s_time_t = millis();
        for (int i = 0; i < num_temps; i++) update_one_thermos(i);
    }
}


bool write_thermos(String name, int value, bool silent) {
    for (int i = 0; i < num_thermos; i++) {
        if ("TS_" + thermos[i].name == name) {
            float f_value = value;
            thermos[i].target_temp = cutoff(f_value, 30.0, 10.0);  //TODO how to write .5 ?
            if (!silent)
                send_state(name, String(thermos[i].target_temp));
            return true;
        }
    }
    return false;
}