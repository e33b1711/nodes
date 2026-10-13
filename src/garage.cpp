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
        server,
        port,
        mac,
};

switch_t switches[] = {
        {"innen", false, 25},      //
        {"GR_CLOSED", false, 24},  //
        {"GR_OPEN", false, 23},    //
        {"F_RAIN", true, PIN_A7, "Regen"},      //
};
const int num_switches = COUNT_OF(switches);

const long period_t = 60000;
temp_t temps[] = {
        {"GR", 8, DHT22_T, "Garage"},  //
        {"AU", 9, DHT22_T, "Außen"},  //
};
const int num_temps = COUNT_OF(temps);

output_t outputs[] = {
        {"LI_GR", 48, 1, HA_LIGHT, "Garage"},     //
        {"LI_GR_L1", 49, 1, HA_LIGHT, "Garage Außen"},  //
        {"DO_GR_DO", 33, 0},  //
        {"DO_GR_UP", 32, 0},  //
        {"DO_GR_DO", 31, 0},  //
        {"DO_GR_UP", 30, 0},  //
        {"DO_GR_DO", 29, 0},  //
        {"DO_GR_UP", 28, 0},  //
        {"DO_GR_DO", 27, 0},  //
        {"DO_GR_UP", 26, 0},  //
};
const int num_outputs = COUNT_OF(outputs);


rollo_t rollos[] = {
        {"DO_GR", 500, false, "Garagentor", true, "GR_CLOSED", "GR_OPEN"},
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
    pinio_mode(4, OUTPUT);
    pinio_mode(5, OUTPUT);
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

    if ((millis() - last_press) > 5000 and not(code_in == "")) {
        // Serial.println("DEBUG: timeout");
        error_signal();
        code_in = "";
    }
    if ((millis() - buzz_set) > 1000)
        digitalWrite(4, 1);
    if ((millis() - led_set) > 1000)
        digitalWrite(5, 1);
}

void user_logic() {
    simple(0, 3, "ZE_GR_1");
    on_door_open();
    code_lock();
}

void user_init() { init_code_lock(); }
