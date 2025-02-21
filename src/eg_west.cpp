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
const IPAddress ip(192, 168, 178, 218);
const int port = 8880;
#else
const IPAddress ip(192, 168, 178, 228);
const int port = 8889;
#endif

const IPAddress server(192, 168, 178, 23);
node_t node_info = {
        "eg_west", ip, server, port, 53, 12, {0xDE, 0xBB, 0x7E, 0xE1, 0x1E, 0x17},
};

const int num_switches = 16;
switch_t switches[num_switches] = {
        {4, 0, 0, 0, 0, 0},   // 0 sü hoch
        {5, 0, 0, 0, 0, 0},   // 1 we hoch
        {6, 0, 0, 0, 0, 0},   // 2 sü runter
        {7, 0, 0, 0, 0, 0},   // 3 sü unten
        {8, 0, 0, 0, 0, 0},   // 4 ez hoch
        {9, 0, 0, 0, 0, 0},   // 5 gang keller oben
        {10, 0, 0, 0, 0, 0},  // 6 " unten
        {11, 0, 0, 0, 0, 0},  // 7
        {25, 0, 0, 0, 0, 0},  // 8 ez runter
        {24, 0, 0, 0, 0, 0},  // 9 ez unten
        {23, 0, 0, 0, 0, 0},  // 10 gang ez
        {22, 0, 0, 0, 0, 0},  // 11 ez mitte
        {17, 0, 0, 0, 0, 0},  // 12
        {16, 0, 0, 0, 0, 0},  // 13 küche
        {3, 0, 0, 0, 0, 0},   // 14 we unten
        {2, 0, 0, 0, 0, 0},   // 15 we runter
};

const int num_temps = 4;
const long period_t = 60000;
temp_t temps[num_temps] = {
        {"EG_GA", 58, 0, 0, 0},  //
        {"EG_KU", 59, 0, 0, 0},  //
        {"EG_WZ", 60, 0, 0, 0},  //
        {"EG_EZ", 61, 0, 0, 0},  //
};

const int num_thermos = 0;
thermos_t thermos[num_thermos] = {};

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
        {"RO_EG_WE_ON", 42, 1, 0, 0},  //
        {"LI_GA_L1", 43, 1, 0, 0},     //
        {"RO_EG_SU_ON", 44, 1, 0, 0},  //
        {"RO_EG_SU_DO", 45, 1, 0, 0},  //
        {"LI_EG_EZ_KU", 46, 1, 0, 0},  //
        {"LI_EG_EZ_L3", 47, 1, 0, 0},  //
        {"RO_EG_WE_DO", 48, 1, 0, 0},  //
        {"LI_EG_GA", 49, 0, 0, 0},     //
};

const int num_rollos = 2;
rollo_t rollos[num_rollos] = {
        {"RO_EG_SU", "RO_EG_SU_ON", "RO_EG_WE_ON", 31000, true, 50, 50, 0, false},
        {"RO_EG_WE", "RO_EG_SU_DO", "RO_EG_WE_DO", 31000, true, 50, 50, 0, false},
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
    long_short(3, "PUMP", 1, "LI_EG_AS", 3);
    rollo_lock(0, 0, "RO_EG_SU");
    rollo_lock(2, 100, "RO_EG_SU");
    simple(14, 3, "LI_EG_AW");
    rollo_lock(1, 0, "RO_EG_WE");
    rollo_lock(15, 100, "RO_EG_WE");
    simple(13, 3, "LI_EG_KU_L1");
    simple(13, 3, "LI_EG_SP");
    simple(6, 3, "LI_UG_GA");
    toggle_couple(5, "LI_EG_GA");
}
