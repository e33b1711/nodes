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

bool write_any(String name, int value) {
    if (write_valve(name, value))
        return true;
    if (write_output(name, value))
        return true;
    if (write_rollo(name, value))
        return true;
    if (write_timer(name, value))
        return true;
    if (write_pwm(name, value))
        return true;
    if (write_thermos(name, value))
        return true;
    return false;
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
    if (get_switch(name, value_string))
        return true;
    Serial.println("INFO: get_any: got none");
    return false;
}

bool pin_alloc[num_pins] = {false};

void alloc_pin(int pin) {
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

void parse_message(String buffer, String &type, String &name, int &value) {
    int index1 = buffer.indexOf('!');
    int index2 = buffer.indexOf('!', index1 + 1);
    int index3 = buffer.indexOf('!', index2 + 1);
    int index4 = buffer.indexOf('$');
    type = buffer.substring(index1 + 1, index2);
    name = buffer.substring(index2 + 1, index3);
    String value_string = buffer.substring(index3 + 1, index4);
    value = convert_value(value_string);
}
