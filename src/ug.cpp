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

#ifndef __TEST__
uint8_t mac[] = {0xDE, 0xBB, 0x7E, 0xE1, 0xCC, 0x01};
#else
uint8_t mac[] = {0xDE, 0xBB, 0x7E, 0xE1, 0xAB, 0x01};
#endif

node_t node_info = {
        "ug",
        server,
        port,
        mac,
};

const int num_switches = 11;
switch_t switches[num_switches] = {
        {"", false, 2, 0, 0, 0, false, false, false, false},             // 0
        {"", false, 3, 0, 0, 0, false, false, false, false},             // 1
        {"", false, 16, 0, 0, 0, false, false, false, false},            // 2 HK
        {"", false, 17, 0, 0, 0, false, false, false, false},            // 3 GA_HK
        {"", false, 22, 0, 0, 0, false, false, false, false},            // 4 GA
        {"", false, 23, 0, 0, 0, false, false, false, false},            // 5 HN
        {"", false, 24, 0, 0, 0, false, false, false, false},            // 6 WK
        {"", false, 25, 0, 0, 0, false, false, false, false},            // 7 HS
        {"F_HE", true, PIN_A7, 0, 0, 0, false, false, false, false},     // gastherme anforderung
        {"F_WW_NO", true, PIN_A6, 0, 0, 0, false, false, false, false},  // pumpensumpf NC
        {"F_WW_NC", true, PIN_A5, 0, 0, 0, false, false, false, false},  // pumepensumpf NO
};

const int num_temps = 7;
const long period_t = 60000;
temp_t temps[num_temps] = {
        {"PU_O", 8, DS18B20_T, 0, 0, 0},  //
        {"PU_U", 9, DS18B20_T, 0, 0, 0},  //
        {"UG_WK", 26, DHT22_T, 0, 0, 0},  //
        {"UG_HO", 27, DHT22_T, 0, 0, 0},  //
        {"UG_LA", 28, DHT22_T, 0, 0, 0},  //
        {"UG_GA", 29, DHT22_T, 0, 0, 0},  //
        {"UG_HK", 30, DHT22_T, 0, 0, 0},  //

};

const int num_thermos = 4;
const int full_valve = 254;
const int closed_valve = 2;
const int default_valve = 128;
thermos_t thermos[num_thermos] = {
        {"UG_WK", "V_UG_WK", 120, 0.2, 30.0, 10.0, 22, 0.0, 0},
        {"UG_HO", "V_UG_HO", 120, 0.2, 30.0, 10.0, 20, 0.0, 0},
        {"UG_LA", "V_UG_LA", 120, 0.2, 30.0, 10.0, 18, 0.0, 0},
        {"UG_GA", "V_UG_GA", 120, 0.2, 30.0, 10.0, 18, 0.0, 0},
};

const int num_outputs = 8;
output_t outputs[num_outputs] = {
        {"LI_UG_HO", 42, 1, 0, 0},  //
        {"LI_UG_HK", 43, 1, 0, 0},  //
        {"LI_UG_TR", 44, 1, 0, 0},  //
        {"BELL", 45, 1, 0, 0},      //
        {"PUMP", 46, 1, 0, 0},      //
        {"LI_UG_GA", 47, 1, 0, 0},  //
        {"LI_UG_WK", 48, 1, 0, 0},  //
        {"LI_UG_HN", 49, 1, 0, 0},  //
};

const int num_rollos = 0;
rollo_t rollos[num_rollos] = {};

const int num_valves = 6;
valve_t valves[num_valves] = {
        {"V_UG_HO", "V_UG_H2", 34, false, 0},  //
        {"V_UG_LA", "", 35, false, 0},         //
        {"V_UG_WK", "", 36, false, 0},         //
        {"V_UG_H2", "V_UG_H3", 37, false, 0},  //
        {"V_UG_GA", "", 38, false, 0},         //
        {"V_UG_H3", "", 39, false, 0},         //
};

const int num_timers = 1;
ntimer_t timers[num_timers] = {{"ZE_BELL", "BELL", false, false, 0, 5}};

const int num_pwms = 1;
pwm_t pwms[num_pwms] = {{"U_EL", 6, 210, 30000, 0, 210, 0}};

void overtemp() {
    // overtemp for elo heating, 85°
    const int max_temp = 90;
    const int thresh_temp = 85;
    const int max_p = 210;
    const int temp_coeff = 40;
    float temp;
    get_temp("TI_PU_O", temp);
    if (temp < thresh_temp)
        set_pwm_max("U_EL", max_p);
    if ((temp > thresh_temp) and (temp < max_temp))
        set_pwm_max("U_EL", (max_temp - temp) * temp_coeff);
    if (temp > max_temp)
        set_pwm_max("U_EL", 0);
}

void water_warning() {
    static unsigned long last_time;
    const unsigned long period = 3600000;
    static bool initial = true;
    if (((last_time + period) < millis()) or initial) {
        initial = false;
        last_time = millis();

        int nc = get_switch("F_WW_NC");
        int no = get_switch("F_WW_NO");

        if (nc == no) {
            Serial.println("ERROR: water warning nc = no");
            send_state("F_WW", "1");
            return;
        }
        if (nc == 0 and no == 1) {
            Serial.println("INFO: water warning!!!");
            send_state("F_WW", "1");
            return;
        }
        Serial.println("INFO: water warning: all good.");
        send_state("F_WW", "0");
        return;
    }
}

void user_logic() {
    simple(2, 3, "LI_UG_HK");
    simple(3, 3, "LI_UG_GA");
    simple(4, 3, "LI_UG_GA");
    long_short(7, "LI_UG_HO", 0, "LI_UG_HO", 3);
    long_short(7, "LI_UG_HN", 0, "none", 3);
    long_short(5, "LI_UG_HN", 0, "LI_UG_HN", 3);
    long_short(5, "LI_UG_HO", 0, "none", 3);
    simple(6, 3, "LI_UG_WK");
    overtemp();
    water_warning();
}

void user_init() {}