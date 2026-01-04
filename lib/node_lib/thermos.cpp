#include "node.h"
#include "thermos.h"
#include "temp.h"

bool get_thermos(String name, String &value) {
    for (int i = 0; i < num_thermos; i++) {
        if (name == ("TS_" + thermos[i].name)) {
            value = String(thermos[i].target_temp);
            return true;
        }
    }
    return false;
}

bool get_thermos_mode(String name, String &value) {
    for (int i = 0; i < num_thermos; i++) {
        if (name == ("TM_" + thermos[i].name)) {
            value = String(thermos[i].mode);
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

    if (thermos[i].mode == 0) {
        // off set valve closed, reset integrator
        write_any(thermos[i].valve, String(closed_valve, DEC));
        thermos[i].int_value = 0.0;
        return;
    }

    if (thermos[i].mode == 2) {
        //bypass dont do anything
        return;
    }

    float temperature;
    if (!get_temp("TI_" + thermos[i].name, temperature)) {
        Serial.println("ERROR: no such temp: " + (thermos[i].name));
        return;
    }

    if (temperature > 100 or temperature < -20 or isnan(temperature)) {
        Serial.println("ERROR: temp out of range. Setting 50%.");
        write_any(thermos[i].valve, String(default_valve, DEC));
        return;
    }

    float linear = (thermos[i].target_temp - temperature) * thermos[i].lin_weight;
    thermos[i].int_value += (thermos[i].target_temp - temperature) * thermos[i].int_weight;
    thermos[i].int_value = cutoff(thermos[i].int_value, full_valve, 0.0);

    float setpoint = linear + thermos[i].int_value;
    int i_setpoint = (int)cutoff(setpoint, full_valve, closed_valve);
    write_any(thermos[i].valve, String(i_setpoint, DEC));
    send_state("IT_" + thermos[i].name, String(thermos[i].int_value));

    Serial.println("DEBUG: update thermos " + thermos[i].name);
    Serial.println("DEBUG: temperature:   " + String(temperature));
    Serial.println("DEBUG: target_temp:   " + String(thermos[i].target_temp));
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
        for (int i = 0; i < num_thermos; i++) {
            send_state("TS_" + thermos[i].name, String(thermos[i].target_temp));
            send_state("TM_" + thermos[i].name, String(thermos[i].mode));
            update_one_thermos(i);
        }
    }
}

bool write_thermos(String name, String val_str) {
    for (int i = 0; i < num_thermos; i++) {
        if ("TS_" + thermos[i].name == name) {
            float f_value = val_str.toFloat();
            thermos[i].target_temp = cutoff(f_value, thermos[i].max_temp, thermos[i].min_temp);
            send_state(name, String(thermos[i].target_temp));
            return true;
        }
    }
    return false;
}

bool write_thermos_mode(String name, String val_str) {
    for (int i = 0; i < num_thermos; i++) {
        if ("TM_" + thermos[i].name == name) {
            int mode = 0;
            if ((val_str == "heat") or (val_str == "on") or (val_str == "1"))
                mode = 1;
            if ((val_str == "bypass") or (val_str == "2"))
                mode = 2;
            thermos[i].mode = mode;
            Serial.println("INFO: Write thermos mode " + name + " / " + String(mode));
            send_state(name, String(thermos[i].mode));
            return true;
        }
    }
    return false;
}