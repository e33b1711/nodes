#include "node.h"
#include "switch.h"
#include "output.h"
#include "temp.h"
#include "timer.h"
#include "rollo.h"
#include "pwm.h"

#include "version.h"

#include "Wiegand.h"
#include "pinio.h"

// door codes are not in git, see include/example_secrets.h
#if __has_include("secrets.h")
#include "secrets.h"
#else
#error "include/secrets.h is missing: copy include/example_secrets.h and set the door codes"
#endif

uint8_t mac[] = {0xDE, 0xBB, 0x7E, 0xE1, 0xCC, 0x06};

node_t node_info = {
        "gr",
        mac,
};

switch_t switches[] = {
        {"innen", false, 25, "Taster Garage innen"},  //
        {"GR_CLOSED", false, 24},                     //
        {"GR_OPEN", false, 23},                       //
        {"F_RAIN", true, PIN_A7, "Regen"},            //
};
const int num_switches = COUNT_OF(switches);

const long period_t = 60000;
temp_t temps[] = {
        {"GR", 8, DHT22_T, "Garage"},  //
        {"AU", 9, DHT22_T, "Außen"},   //
};
const int num_temps = COUNT_OF(temps);

output_t outputs[] = {
        {"LI_GR", 48, 1, HA_LIGHT, "Garage"},           //
        {"LI_GR_L1", 49, 1, HA_LIGHT, "Garage Außen"},  //
        {"DO_GR_DO", 33, 0},                            //
        {"DO_GR_UP", 32, 0},                            //
        {"DO_GR_DO", 31, 0},                            //
        {"DO_GR_UP", 30, 0},                            //
        {"DO_GR_DO", 29, 0},                            //
        {"DO_GR_UP", 28, 0},                            //
        {"DO_GR_DO", 27, 0},                            //
        {"DO_GR_UP", 26, 0},                            //
};
const int num_outputs = COUNT_OF(outputs);

rollo_t rollos[] = {
        {"DO_GR", 500, false, "Garagentor", true, "GR_OPEN", "GR_CLOSED"},
};
const int num_rollos = COUNT_OF(rollos);

ntimer_t timers[] = {
        {"ZE_GR_1", "LI_GR", 600, HA_LIGHT, "Garage Timer 10 min"},
        {"ZE_GR_2", "LI_GR_L1", 120, HA_LIGHT, "Garage Timer außen"},
};
const int num_timers = COUNT_OF(timers);

pwm_t pwms[] = {};
const int num_pwms = COUNT_OF(pwms);

void on_door_open() {
    static int prev_value;
    static bool init = true;
    if (init) {
        prev_value = get_rollo("DO_GR");
        init = false;
    }
    if ((prev_value == 100) & (prev_value != get_rollo("DO_GR"))) {
        write_any("ZE_GR_1", "ON");
        Serial.println("DEBUG: trigger einfahrt licht.");
    }
    prev_value = get_rollo("DO_GR");
}

WIEGAND wg;

// keypad feedback, active low
const int error_pin = 4;
const int success_pin = 5;
unsigned long error_set, success_set;

void init_code_lock() {
    Serial.println("INFO: init wiegand keypad");
    alloc_pin(2);  // wiegand
    alloc_pin(3);  // wiegand
    alloc_pin(error_pin);
    alloc_pin(success_pin);
    wg.begin(2, 3);

    digitalWrite(error_pin, 1);
    digitalWrite(success_pin, 1);
    pinio_mode(error_pin, OUTPUT);
    pinio_mode(success_pin, OUTPUT);
    error_set = millis();
    success_set = millis();
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
    digitalWrite(error_pin, 0);
    error_set = millis();
}

void success_signal() {
    digitalWrite(success_pin, 0);
    success_set = millis();
}

const int max_wrong_codes = 5;
const unsigned long lock_time = 60000;

void code_event(const char *type) {
    send_state("GR_CODE", String("{\"event_type\":\"") + type + "\"}");
}

void code_lock() {
    static String code_in = "";
    static unsigned long last_press = 0;
    static int wrong_codes = 0;
    static unsigned long locked_since = 0;

    bool locked = wrong_codes >= max_wrong_codes;
    if (locked and (millis() - locked_since) > lock_time) {
        wrong_codes = 0;
        locked = false;
        Serial.println("INFO: keypad unlocked");
    }

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
        if (button_in == 13 and locked) {
            code_in = "";
            error_signal();
        } else if (button_in == 13) {
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
            if (found) {
                wrong_codes = 0;
                code_event("code_ok");
            } else {
                error_signal();
                wrong_codes++;
                code_event("code_wrong");
                if (wrong_codes >= max_wrong_codes) {
                    locked_since = millis();
                    code_event("locked");
                    Serial.println("WARNING: keypad locked after wrong codes");
                }
            }
        }
        if (code_in.length() > 8) {
            code_in = "";
        }
    }

    if ((millis() - last_press) > 5000 and not(code_in == "")) {
        // Serial.println("DEBUG: timeout");
        error_signal();
        code_in = "";
    }
    if ((millis() - error_set) > 1000)
        digitalWrite(error_pin, 1);
    if ((millis() - success_set) > 1000)
        digitalWrite(success_pin, 1);
}

void user_logic() {
    simple(0, 3, "ZE_GR_1");
    on_door_open();
    code_lock();
}

void user_discovery() {
    announce_event("GR_CODE", "Codeschloss", F("[\"code_ok\",\"code_wrong\",\"locked\"]"));
}

void user_init() { init_code_lock(); }
