#include "node.h"
#include "switch.h"
#include "output.h"
#include "temp.h"
#include "timer.h"
#include "rollo.h"
#include "valve.h"
#include "pwm.h"
#include "thermos.h"

#include "version.h"
#include "server.h"

#include "Wiegand.h"

#ifndef __TEST__
uint8_t mac[] = {0xDE, 0xBB, 0x7E, 0xE1, 0xCC, 0x06};
#else
uint8_t mac[] = {0xDE, 0xBB, 0x7E, 0xE1, 0xAB, 0x06};
#endif

node_t node_info = {
        "gr",
        server,
        port,
        mac,
};

const int num_switches = 3;
switch_t switches[num_switches] = {
        {"innen", false, 25, 0, 0, 0, false, false, false, false},      //
        {"GR_CLOSED", false, 24, 0, 0, 0, false, false, false, false},  //
        {"GR_OPEN", false, 23, 0, 0, 0, false, false, false, false},    //
};

const int num_temps = 1;
const long period_t = 60000;
const int ds18b_pin = 61;
temp_t temps[num_temps] = {
        {"GR", 8, 0, 0, 0},  // does dht22 work on esp32?
};

const int num_thermos = 0;
thermos_t thermos[num_thermos] = {};

const int num_outputs = 10;
output_t outputs[num_outputs] = {
        {"LI_GR", 48, 1, 0, 0},     //
        {"LI_GR_L1", 49, 1, 0, 0},  //
        {"DO_GR_DO", 33, 0, 0, 0},  //
        {"DO_GR_UP", 32, 0, 0, 0},  //
        {"DO_GR_DO", 31, 0, 0, 0},  //
        {"DO_GR_UP", 30, 0, 0, 0},  //
        {"DO_GR_DO", 29, 0, 0, 0},  //
        {"DO_GR_UP", 28, 0, 0, 0},  //
        {"DO_GR_DO", 27, 0, 0, 0},  //
        {"DO_GR_UP", 26, 0, 0, 0},  //
};

const int num_rollos = 1;

rollo_t rollos[num_rollos] = {
        {"DO_GR", 500, false, true, "GR_CLOSED", "GR_OPEN", 50, 50, 0, false},
};

const int num_valves = 0;
valve_t valves[num_valves] = {};

const int num_timers = 3;
ntimer_t timers[num_timers] = {
        {"ZE_GR_0", "LI_GR", false, false, 0, 120},
        {"ZE_GR_1", "LI_GR", false, false, 0, 600},
        {"ZE_GR_2", "LI_GR_L1", false, false, 0, 120},
};

const int num_pwms = 0;
pwm_t pwms[num_pwms] = {};

void on_door_open() {
    static int prev_value;
    static bool init = true;
    if (init) {
        prev_value = get_rollo("DO_GR");
        init = false;
    }
    if ((prev_value == 100) & (prev_value != get_rollo("DO_GR"))) {
        write_any("ZE_GR_0", "ON");
        Serial.println("DEBUG: trigger einfahrt licht.");
    }
    prev_value = get_rollo("DO_GR");
}

WIEGAND wg;

const int num_codes = 4;
const String code[num_codes] = {"1171", "2105", "6539", "5473"};
unsigned long buzz_set, led_set;

void init_code_lock() {
    Serial.println("INFO: init wiegand keypad");
    alloc_pin(2);  // wiegand
    alloc_pin(3);  // wiegand
    alloc_pin(4);  // led
    alloc_pin(5);  // buzz
    wg.begin(2, 3);

    digitalWrite(4, 1);
    digitalWrite(5, 1);
    pinMode(4, OUTPUT);
    pinMode(5, OUTPUT);
    buzz_set = millis();
    led_set = millis();
}

void toggle_door() {
    if (get_rollo("DO_GR") == 0) {
        write_rollo("DO_GR", 100);
        Serial.println("DEBUG: Door down");
        return;
    }
    write_rollo("DO_GR", 0);
    Serial.println("DEBUG: Door up");
}

void error_signal() {
    digitalWrite(4, 0);
    buzz_set = millis();
}

void success_signal() {
    digitalWrite(5, 0);
    led_set = millis();
}

void code_lock() {
    static String code_in = "";
    static unsigned long last_press = 0;

    if (wg.available()) {
        unsigned long button_in = wg.getCode();
        last_press = millis();
        Serial.print("DEBUG: Wiegand");
        Serial.println(button_in);
        if (button_in <= 9) {
            code_in += String(button_in);
            Serial.println("DEBUG:code_in += " + code_in);
        }
        if (button_in == 27) {
            code_in = "";
            // Serial.println("DEBUG: code clear");
        }
        if (button_in == 13) {
            bool found = false;
            Serial.println("DEBUG: code to check: " + code_in);
            for (int i = 0; i < num_codes; i++) {
                if (code_in == code[i]) {
                    // Serial.println("DEBUG: code match: " + String(i));
                    success_signal();
                    toggle_door();
                    found = true;
                    break;
                }
            }
            code_in = "";
            if (!found)
                error_signal();
        }
        if (code_in.length() > 8) {
            code_in = "";
        }
    }

    if (last_press + 5000 < millis() and not(code_in == "")) {
        // Serial.println("DEBUG: timeout");
        error_signal();
        code_in = "";
    }
    if (buzz_set + 1000 < millis())
        digitalWrite(4, 1);
    if (led_set + 1000 < millis())
        digitalWrite(5, 1);
}

void user_logic() {
    simple(0, 3, "ZE_GR_1");
    on_door_open();
    code_lock();
}

void user_init() { init_code_lock(); }
