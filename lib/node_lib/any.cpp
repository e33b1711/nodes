#include "node.h"
#include "switch.h"
#include "output.h"
#include "temp.h"
#include "timer.h"
#include "rollo.h"
#include "valve.h"
#include "pwm.h"
#include "thermos.h"
#include "switch.h"

void write_any_internal(String name, String val_str) {
    if (write_valve(name, val_str))
        return;
    if (write_output(name, val_str))
        return;
    if (write_rollo(name, val_str))
        return;
    if (write_timer(name, val_str))
        return;
    if (write_pwm(name, val_str))
        return;
    if (write_thermos_mode(name, val_str))
        return;
    write_thermos(name, val_str);
}

void write_any(String name, String val_str) {
    write_any_internal(name, val_str);
    send_command(name, val_str);
}

bool get_any(String name, String &value_string) {
    Serial.println("INFO: get_any: " + name + " " + value_string);
    if (get_temp(name, value_string))
        return true;
    if (get_humi(name, value_string))
        return true;
    if (get_valve(name, value_string))
        return true;
    if (get_rollo(name, value_string))
        return true;
    if (get_output(name, value_string))
        return true;
    if (get_timer(name, value_string))
        return true;
    if (get_pwm(name, value_string))
        return true;
    if (get_thermos(name, value_string))
        return true;
    if (get_thermos_mode(name, value_string))
        return true;
    if (get_switch(name, value_string))
        return true;
    Serial.println("INFO: get_any: got none");
    return false;
}

bool pin_alloc[num_pins] = {false};

void alloc_pin(int pin) {
    if (pin == pseudo_pin) return;
    if (pin >= num_pins) {
        Serial.println("FAILURE: pin undefinied: " + String(pin, DEC));
        Serial.println("Exit...");
        while (true) delay(100);
    }
    if (pin_alloc[pin]) {
        Serial.println("FAILURE: allready allocated: " + String(pin, DEC));
        Serial.println("Exit...");
        while (true) delay(100);
    }
    pin_alloc[pin] = true;
}

int convert_value(String value_string) {
    if (value_string == "STOP")
        return 50;
    if (value_string == "UP")
        return 0;
    if (value_string == "DOWN")
        return 100;
    if (value_string == "ON")
        return 1;
    if (value_string == "OFF")
        return 0;
    return value_string.toInt();
}

void parse_message(String buffer, String &type, String &name, String &val_str) {
    int index1 = buffer.indexOf('!');
    int index2 = buffer.indexOf('!', index1 + 1);
    int index3 = buffer.indexOf('!', index2 + 1);
    int index4 = buffer.indexOf('$');
    type = buffer.substring(index1 + 1, index2);
    name = buffer.substring(index2 + 1, index3);
    val_str = buffer.substring(index3 + 1, index4);
}
