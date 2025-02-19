#include "node.h"
#include "switch.h"
#include "output.h"
#include "temp.h"
#include "timer.h"
#include "rollo.h"
#include "valve.h"
#include "pwm.h"



#ifndef __TEST__

const IPAddress ip(192, 168, 178, 211);
const IPAddress server(192, 168, 178, 23);
node_t node_info = {
        "ug", ip, server, 8888, 53, 12, {0xDE, 0xAA, 0x7E, 0xE1, 0x1E, 0x10},
};

#else

const IPAddress ip(192, 168, 178, 231);
const IPAddress server(192, 168, 178, 23);
node_t node_info = {
        "ug", ip, server, 8889, 53, 12, {0xDE, 0xBB, 0x7E, 0xE1, 0x1E, 0x10},
};

#endif

const int num_switches = 8;
switch_t switches[num_switches] = {
        {2, 0, 0, 0, 0, 0},   // 0
        {3, 0, 0, 0, 0, 0},   // 1
        {16, 0, 0, 0, 0, 0},  // 2 HK
        {17, 0, 0, 0, 0, 0},  // 3 GA_HK
        {22, 0, 0, 0, 0, 0},  // 4 GA
        {23, 0, 0, 0, 0, 0},  // 5 HN
        {24, 0, 0, 0, 0, 0},  // 6 WK
        {25, 0, 0, 0, 0, 0},  // 7 HS
};

const int num_temps = 6;
const long period_t = 60000;
const int ds18b_pin = 61;
temp_t temps[num_temps] = {
        {"TI_UG_WK", 26, 0, 0, 0},  //
        {"TI_UG_HN", 27, 0, 0, 0},  //
        {"TI_UG_LA", 28, 0, 0, 0},  //
        {"TI_UG_GA", 29, 0, 0, 0},  //
        {"TI_UG_HK", 30, 0, 0, 0},  //
        {"TI_UG_HS", 31, 0, 0, 0},  //
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

const int num_couples = 0;
couple_t couples[num_couples] = {};

const int num_rollos = 0;
rollo_t rollos[num_rollos] = {};

const int num_valves = 6;
valve_t valves[num_valves] = {
        {"U_UG_H1", 34, 0}, {"U_UG_LA", 35, 0}, {"U_UG_WK", 36, 0},
        {"U_UG_H2", 37, 0}, {"U_UG_GA", 38, 0}, {"U_UG_H3", 39, 0},
};

const int num_timers = 1;
timer_t timers[num_timers] = {{"ZE_BELL", "BELL", false, false, 0, 1}};

const int num_pwms = 1;
pwm_t pwms[num_pwms] = {{"HEAT", 39, 220, 180000, 0, 220, 0}};

void user_logic() {
    simple(2, 3, "LI_UG_HK");
    simple(3, 3, "LI_UG_GA");
    simple(4, 3, "LI_UG_GA");
    long_short(7, "LI_UG_HO", 0, "LI_UG_HO", 3);
    long_short(7, "LI_UG_HN", 0, "none", 3);
    long_short(5, "LI_UG_HN", 0, "LI_UG_HN", 3);
    long_short(5, "LI_UG_HO", 0, "none", 3);
    simple(6, 3, "LI_UG_WK");
}
