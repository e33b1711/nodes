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
        write_any(thermos[i].valve, half_valve, false);
        return;
    }

    float offset = base_valve + thermos[i].target_temp * thermos[i].abs_weight;
    float linear = (thermos[i].target_temp - temperature) * thermos[i].lin_weight;
    thermos[i].int_value += (thermos[i].target_temp - temperature) * thermos[i].int_weight;
    thermos[i].int_value = cutoff(thermos[i].int_value, half_valve, half_valve * -1.0);

    float setpoint = offset + linear + thermos[i].int_value;
    int i_setpoint = (int)cutoff(setpoint, full_valve, 0.0);
    send_command(thermos[i].valve, i_setpoint);
    write_any(thermos[i].valve, i_setpoint, false);
    send_state("IT_" + thermos[i].name, String(thermos[i].int_value));

    Serial.println("DEBUG: update thermos " + thermos[i].name);
    Serial.println("DEBUG: temperature:   " + String(temperature));
    Serial.println("DEBUG: target_temp:   " + String(thermos[i].target_temp));
    Serial.println("DEBUG: offset:        " + String(offset));
    Serial.println("DEBUG: linear:        " + String(linear));
    Serial.println("DEBUG: integral:      " + String(thermos[i].int_value));
    Serial.println("DEBUG: setpoint:      " + String(i_setpoint, DEC));
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
            thermos[i].target_temp = cutoff(f_value, 30.0, 10.0);
            thermos[i].int_value = 0;
            if (!silent)
                send_state(name, String(thermos[i].target_temp));
            return true;
        }
    }
    return false;
}