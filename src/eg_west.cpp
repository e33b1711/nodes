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
uint8_t mac[] = {0xDE, 0xBB, 0x7E, 0xE1, 0xAA, 0x03};
#else
uint8_t mac[] = {0xDE, 0xBB, 0x7E, 0xE1, 0xAB, 0x03};
#endif

node_t node_info = {
        "eg_west",
        server,
        port,
        mac,
};

const int num_switches = 16;
switch_t switches[num_switches] = {
        {"", false, 4, 0, 0, 0, false, false, false, false},   // 0 sü hoch
        {"", false, 5, 0, 0, 0, false, false, false, false},   // 1 we hoch
        {"", false, 6, 0, 0, 0, false, false, false, false},   // 2 sü runter
        {"", false, 7, 0, 0, 0, false, false, false, false},   // 3 sü unten
        {"", false, 8, 0, 0, 0, false, false, false, false},   // 4 ez hoch
        {"", false, 9, 0, 0, 0, false, false, false, false},   // 5 gang keller oben
        {"", false, 10, 0, 0, 0, false, false, false, false},  // 6 " unten
        {"", false, 11, 0, 0, 0, false, false, false, false},  // 7
        {"", false, 25, 0, 0, 0, false, false, false, false},  // 8 ez runter
        {"", false, 24, 0, 0, 0, false, false, false, false},  // 9 ez unten
        {"", false, 23, 0, 0, 0, false, false, false, false},  // 10 gang ez
        {"", false, 22, 0, 0, 0, false, false, false, false},  // 11 ez mitte
        {"", false, 17, 0, 0, 0, false, false, false, false},  // 12
        {"", false, 16, 0, 0, 0, false, false, false, false},  // 13 küche
        {"", false, 3, 0, 0, 0, false, false, false, false},   // 14 we unten
        {"", false, 2, 0, 0, 0, false, false, false, false},   // 15 we runter
};

const int num_temps = 4;
const long period_t = 60000;
temp_t temps[num_temps] = {
        {"EG_GA", 58, 0, 0, 0},  //
        {"EG_KU", 59, 0, 0, 0},  //
        {"EG_WZ", 60, 0, 0, 0},  //
        {"EG_EZ", 61, 0, 0, 0},  //
};

const int num_thermos = 4;
thermos_t thermos[num_thermos] = {
        {"EG_GA", "V_EG_GA", 5, 50, 0.1, 60000, 22, 0.0, 0},
        {"EG_KU", "V_EG_KU", 5, 50, 0.1, 60000, 22, 0.0, 0},
        {"EG_WZ", "V_EG_WZ", 5, 50, 0.1, 60000, 21, 0.0, 0},
        {"EG_EZ", "V_EG_EZ", 5, 50, 0.1, 60000, 22, 0.0, 0},
};

const int num_outputs = 16;
output_t outputs[num_outputs] = {
        {"LI_EG_EZ_KU", 34, 1, 0, 0},  //
        {"LI_EG_SP", 35, 1, 0, 0},     //
        {"LI_EG_AS", 36, 1, 0, 0},     //
        {"LI_EG_KU_L1", 37, 1, 0, 0},  //
        {"LI_EG_EZ_L1", 38, 1, 0, 0},  //
        {"LI_EG_AO", 39, 1, 0, 0},     //
        {"LI_EG_EZ", 40, 1, 0, 0},     //
        {"LI_EG_AW", 41, 1, 0, 0},     //
        {"RO_EG_WE_UP", 42, 1, 0, 0},  //
        {"LI_GA_L1", 43, 1, 0, 0},     //
        {"RO_EG_SU_UP", 44, 1, 0, 0},  //
        {"RO_EG_SU_DO", 45, 1, 0, 0},  //
        {"LI_EG_EZ_KU", 46, 1, 0, 0},  //
        {"LI_EG_EZ_L3", 47, 1, 0, 0},  //
        {"RO_EG_WE_DO", 48, 1, 0, 0},  //
        {"LI_EG_GA", 49, 1, 0, 0},     //
};

const int num_rollos = 2;
rollo_t rollos[num_rollos] = {
        {"RO_EG_SU", 31000, true, false, "", "", 50, 50, 0, false},
        {"RO_EG_WE", 31000, true, false, "", "", 50, 50, 0, false},
};

const int num_valves = 0;
valve_t valves[num_valves] = {};

const int num_timers = 0;
ntimer_t timers[num_timers] = {};

const int num_pwms = 0;
pwm_t pwms[num_pwms] = {};

void user_logic() {
    toggle_couple(10, "LI_EG_GA");
    simple(9, 3, "LI_EG_EZ_KU");
    long_short(11, "LI_EG_EZ_L1", 3, "LI_EG_EZ", 3);
    simple(4, 0, "RO_EG_SU");
    simple(4, 0, "RO_EG_WE");
    simple(8, 100, "RO_EG_SU");
    simple(8, 100, "RO_EG_WE");
    long_short(3, "none", 1, "LI_EG_AS", 3);
    long_short_extern(3, "PUMP", 3, "none", 3);
    rollo_lock(0, 0, "RO_EG_SU");
    rollo_lock(2, 100, "RO_EG_SU");
    simple(14, 3, "LI_EG_AW");
    rollo_lock(1, 0, "RO_EG_WE");
    rollo_lock(15, 100, "RO_EG_WE");
    simple(13, 3, "LI_EG_KU_L1");
    simple(13, 3, "LI_EG_SP");
    simple_extern(6, 3, "LI_UG_GA");
    toggle_couple(5, "LI_EG_GA");
}

void user_init() {}