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

#ifndef __TEST__
const IPAddress ip(192, 168, 178, 211);
const int port = 8880;
#else
const IPAddress ip(192, 168, 178, 231);
const int port = 8889;
#endif

const IPAddress server(192, 168, 178, 23);
node_t node_info = {
        "ug", ip, server, port, 53, 12, {0xDE, 0xBB, 0x7E, 0xE1, 0x1E, 0x10},
};

const int num_switches = 8;
switch_t switches[num_switches] = {
        {"", false, 2, 0, 0, 0, false, false, false, false},   // 0
        {"", false, 3, 0, 0, 0, false, false, false, false},   // 1
        {"", false, 16, 0, 0, 0, false, false, false, false},  // 2 HK
        {"", false, 17, 0, 0, 0, false, false, false, false},  // 3 GA_HK
        {"", false, 22, 0, 0, 0, false, false, false, false},  // 4 GA
        {"", false, 23, 0, 0, 0, false, false, false, false},  // 5 HN
        {"", false, 24, 0, 0, 0, false, false, false, false},  // 6 WK
        {"", false, 25, 0, 0, 0, false, false, false, false},  // 7 HS
};

const int num_temps = 6;
const long period_t = 60000;
const int ds18b_pin = 61;
temp_t temps[num_temps] = {
        {"UG_WK", 26, 0, 0, 0},  //
        {"UG_HN", 27, 0, 0, 0},  //
        {"UG_LA", 28, 0, 0, 0},  //
        {"UG_GA", 29, 0, 0, 0},  //
        {"UG_HK", 36, 0, 0, 0},  //TODO switched with 30 for testimg
        {"UG_HS", 31, 0, 0, 0},  //
};

const int num_thermos = 4;
thermos_t thermos[num_thermos] = {
        {"UG_WK", "V_UG_WK", 5, 50, 0.1, 60000, 24, 0.0, 0},
        {"UG_HN", "V_UG_HN", 5, 50, 0.1, 60000, 20, 0.0, 0},
        {"UG_LA", "V_UG_LA", 5, 50, 0.1, 60000, 18, 0.0, 0},
        {"UG_GA", "V_UG_GA", 5, 50, 0.1, 60000, 18, 0.0, 0},
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
        {"V_UG_H1", 34, 0},  // 
        {"V_UG_LA", 35, 0},  //
        {"V_UG_WK", 30, 0},  //TODO was 36
        {"V_UG_H2", 37, 0},  //
        {"V_UG_GA", 38, 0},  //
        {"V_UG_H3", 39, 0},  //
};

const int num_timers = 1;
ntimer_t timers[num_timers] = {{"ZE_BELL", "BELL", false, false, 0, 1}};

const int num_pwms = 0;
pwm_t pwms[num_pwms] = {};

void user_logic() {
    simple(2, 3, "LI_UG_HK");
    simple(3, 3, "LI_UG_GA");
    simple(4, 3, "LI_UG_GA");
    long_short(7, "LI_UG_HO", 0, "LI_UG_HO", 3);
    long_short(7, "LI_UG_HN", 0, "none", 3);
    long_short(5, "LI_UG_HN", 0, "LI_UG_HN", 3);
    long_short(5, "LI_UG_HO", 0, "none", 3);
    simple(6, 3, "LI_UG_WK");
    //TOD couple V_UG_H1 V_UG_H2 V_UG_H3
}
