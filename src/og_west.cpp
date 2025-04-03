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
uint8_t mac[] = {0xDE, 0xBB, 0x7E, 0xE1, 0xCC, 0x05};
#else
uint8_t mac[] = {0xDE, 0xBB, 0x7E, 0xE1, 0xAB, 0x05};
#endif

node_t node_info = {
        "og_west",
        server,
        port,
        mac,
};

const int num_temps = 3;
const long period_t = 60000;
temp_t temps[num_temps] = {
        {"OG_KN", 28, 0, 0, 0},
        {"OG_KS", 27, 0, 0, 0},
        {"OG_GA", 26, 0, 0, 0},
};

const int num_thermos = 3;
thermos_t thermos[num_thermos] = {
        {"OG_KN", "V_OG_KN", 120, 0.2, 20, 0.0},
        {"OG_KS", "V_OG_KS", 120, 0.2, 21, 0.0},
        {"OG_GA", "V_OG_GA", 120, 0.2, 21, 0.0},
};

const int num_outputs = 24;
output_t outputs[num_outputs] = {
        {"LI_34", 34, 1, 0, 0},        //
        {"RO_OG_KS_DO", 35, 1, 0, 0},  //
        {"LI_OG_KS", 36, 1, 0, 0},     //
        {"RO_OG_KS_UP", 37, 1, 0, 0},  //
        {"LI_OG_KS_L1", 42, 1, 0, 0},  //
        {"LI_OG_KN_L1", 43, 1, 0, 0},  //
        {"LI_OG_KN", 44, 1, 0, 0},     //
        {"RO_OG_KN_UP", 45, 1, 0, 0},  //
        {"LI_GA_L1", 46, 1, 0, 0},     //
        {"RO_OG_KN_DO", 47, 1, 0, 0},  //
        {"LI_OG_GA_L1", 48, 1, 0, 0},  //
        {"LI_OG_GA", 49, 1, 0, 0},     //
        {"VD_OG_KS_DO", 54, 0, 0, 0},  //
        {"VD_OG_KS_UP", 55, 0, 0, 0},  //
        {"DF_OG_KS_DO", 56, 0, 0, 0},  //
        {"DF_OG_KS_UP", 57, 0, 0, 0},  //
        {"VD_OG_KN_DO", 58, 0, 0, 0},  //
        {"VD_OG_KN_UP", 59, 0, 0, 0},  //
        {"DF_OG_KN_UP", 60, 0, 0, 0},  //
        {"DF_OG_KN_DO", 61, 0, 0, 0},  //
        {"VD_OG_GA_UP", 30, 0, 0, 0},  //
        {"DF_OG_GA_DO", 31, 0, 0, 0},  //
        {"VD_OG_GA_DO", 32, 0, 0, 0},  //
        {"DF_OG_GA_UP", 33, 0, 0, 0},  //
};

const int num_rollos = 8;
rollo_t rollos[num_rollos] = {
        {"RO_OG_KN", 31000, true, false, "", "", 50, 50, 0, false},
        {"RO_OG_KS", 31000, true, false, "", "", 50, 50, 0, false},
        {"DF_OG_KN", 500, false, false, "", "", 50, 50, 0, false},
        {"DF_OG_KS", 500, false, false, "", "", 50, 50, 0, false},
        {"DF_OG_GA", 500, false, false, "", "", 50, 50, 0, false},
        {"VD_OG_KS", 500, false, false, "", "", 50, 50, 0, false},
        {"VD_OG_KN", 500, false, false, "", "", 50, 50, 0, false},
        {"VD_OG_GA", 500, false, false, "", "", 50, 50, 0, false},
};

const int num_valves = 0;
valve_t valves[num_valves] = {};

const int num_switches = 16;
switch_t switches[num_switches] = {
        {"", false, 4, 0, 0, 0, false, false, false, false},   // bini oben
        {"", false, 5, 0, 0, 0, false, false, false, false},   // gang bini
        {"", false, 6, 0, 0, 0, false, false, false, false},   // bini hoch
        {"", false, 7, 0, 0, 0, false, false, false, false},   // bini runter
        {"", false, 8, 0, 0, 0, false, false, false, false},   // leo a hoch
        {"", false, 9, 0, 0, 0, false, false, false, false},   // bini a runter
        {"", false, 10, 0, 0, 0, false, false, false, false},  // leo a runter
        {"", false, 11, 0, 0, 0, false, false, false, false},  // bini a hoch
        {"", false, 25, 0, 0, 0, false, false, false, false},  // leo hoch
        {"", false, 24, 0, 0, 0, false, false, false, false},  // leo unten
        {"", false, 23, 0, 0, 0, false, false, false, false},  // gang runter
        {"", false, 22, 0, 0, 0, false, false, false, false},  // leo runter
        {"", false, 17, 0, 0, 0, false, false, false, false},  // bini unten
        {"", false, 16, 0, 0, 0, false, false, false, false},  // gang hoch
        {"", false, 3, 0, 0, 0, false, false, false, false},   // leo oben
        {"", false, 2, 0, 0, 0, false, false, false, false},   // gang leo
};

const int num_timers = 0;
ntimer_t timers[num_timers] = {};

const int num_pwms = 0;
pwm_t pwms[num_pwms] = {};

void user_logic() {
    long_short(15, "LI_OG_GA", 3, "LI_OG_GA_L1", 3);
    long_short(13, "VD_OG_GA", 0, "DF_OG_GA", 0);
    long_short(10, "VD_OG_GA", 100, "DF_OG_GA", 100);
    long_short(1, "LI_OG_GA", 3, "LI_OG_GA_L1", 3);
    simple(14, 3, "LI_OG_KN");
    simple(9, 3, "LI_OG_KN_L1");
    long_short(11, "VD_OG_KN", 100, "DF_OG_KN", 100);
    long_short(8, "VD_OG_KN", 0, "DF_OG_KN", 0);
    rollo_lock(4, 0, "RO_OG_KN");
    rollo_lock(6, 100, "RO_OG_KN");
    simple(0, 3, "LI_OG_KS");
    simple(12, 3, "LI_OG_KS_L1");
    long_short(2, "VD_OG_KS", 0, "DF_OG_KS", 0);
    long_short(3, "VD_OG_KS", 100, "DF_OG_KS", 100);
    rollo_lock(5, 100, "RO_OG_KS");
    rollo_lock(7, 0, "RO_OG_KS");
}

void user_init() {}