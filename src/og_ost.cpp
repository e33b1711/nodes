#include "node.h"
#include "switch.h"
#include "output.h"
#include "temp.h"
#include "timer.h"
#include "rollo.h"
#include "valve.h"
#include "pwm.h"
#include "thermos.h"

#ifndef __TEST__
const IPAddress ip(192, 168, 178, 216);
const int port = 8880;
#else
const IPAddress ip(192, 168, 178, 226);
const int port = 8889;
#endif

const IPAddress server(192, 168, 178, 23);
node_t node_info = {
        "og_ost", ip, server, port, 53, 12, {0xDE, 0xBB, 0x7E, 0xE1, 0x1E, 0x15},
};

const int num_temps = 2;
const long period_t = 60000;
temp_t temps[num_temps] = {
        {"OG_BA", 27, 0, 0, 0},
        {"OG_SZ", 26, 0, 0, 0},
};

const int num_thermos = 0;
thermos_t thermos[num_thermos] = {};

const int num_outputs = 12;
output_t outputs[num_outputs] = {
        {"RO_OG_SZ_ON", 41, 1, 0, 0},  //
        {"LI_OG_BA", 42, 1, 0, 0},     //
        {"RO_OG_BA_DO", 43, 1, 0, 0},  //
        {"LI_OG_SZ", 44, 1, 0, 0},     //
        {"LI_OG_SZ_L2", 45, 1, 0, 0},  //
        {"RO_OG_SZ_DO", 46, 1, 0, 0},  //
        {"LI_OG_SZ_L1", 47, 1, 0, 0},  //
        {"RO_OG_BA_ON", 49, 1, 0, 0},  //
        {"VD_OG_SZ_DO", 58, 0, 0, 0},  //
        {"DF_OG_SZ_DO", 59, 0, 0, 0},  //
        {"VD_OG_SZ_UP", 60, 0, 0, 0},  //
        {"DF_OG_SZ_UP", 61, 0, 0, 0},  //
};

const int num_couples = 0;
couple_t couples[num_couples] = {};

const int num_rollos = 4;
rollo_t rollos[num_rollos] = {
        {"RO_OG_SZ", "RO_OG_SZ_ON", "RO_OG_SZ_DO", 31000, true, 50, 50, 0, false},
        {"RO_OG_BA", "RO_OG_BA_ON", "RO_OG_BA_DO", 31000, true, 50, 50, 0, false},
        {"DF_OG_SZ", "DF_OG_SZ_UP", "DF_OG_SZ_DO", 500, false, 50, 50, 0, false},
        {"VD_OG_SZ", "VD_OG_SZ_UP", "VD_OG_SZ_DO", 500, false, 50, 50, 0, false},
};

const int num_valves = 5;
valve_t valves[num_valves] = {
        {"U_OG_GA", 30, 0, 0},  // blau  //TODO was 34!!
        {"U_OG_KS", 35, 0, 0},  // weiß
        {"U_OG_KN", 36, 0, 0},  // grün
        {"U_OG_BA", 37, 0, 0},  // rosa
        {"U_OG_SZ", 38, 0, 0},  // grau
};

const int num_switches = 16;
switch_t switches[num_switches] = {
        {4, 0, 0, 0, 0, 0},   // 0 sz unten
        {5, 0, 0, 0, 0, 0},   // 1 ans
        {6, 0, 0, 0, 0, 0},   // 2 sz rollo runter
        {7, 0, 0, 0, 0, 0},   // 3 sz oben
        {8, 0, 0, 0, 0, 0},   // 4 bewegung
        {9, 0, 0, 0, 0, 0},   // 5 mell
        {10, 0, 0, 0, 0, 0},  // 6 sz rollohoch
        {11, 0, 0, 0, 0, 0},  // 7 gang sz
        {25, 0, 0, 0, 0, 0},  // 8 bad runter tür
        {24, 0, 0, 0, 0, 0},  // 9 gang bad
        {23, 0, 0, 0, 0, 0},  // 10 bad
        {22, 0, 0, 0, 0, 0},  // 11 bad hoch tür
        {17, 0, 0, 0, 0, 0},  // 12 bad fenster runter
        {16, 0, 0, 0, 0, 0},  // 13 bad fenster hoch
        {3, 0, 0, 0, 0, 0},   // 14 df sz hoch
        {2, 0, 0, 0, 0, 0},   // 15 df sz runter
};

const int num_timers = 0;
ntimer_t timers[num_timers] = {};

const int num_pwms = 0;
pwm_t pwms[num_pwms] = {};

void user_logic() {
    long_short(3, "LI_OG_SZ", 3, "LI_OG_SZ", 0);
    long_short(3, "none", 3, "LI_OG_SZ_L1", 0);
    long_short(3, "none", 3, "LI_OG_SZ_L2", 0);
    long_short(0, "LI_OG_SZ", 3, "LI_OG_SZ", 0);
    long_short(0, "none", 3, "LI_OG_SZ_L1", 0);
    long_short(0, "none", 3, "LI_OG_SZ_L2", 0);
    long_short(14, "DF_OG_SZ", 100, "VD_OG_SZ", 1000);
    long_short(15, "DF_OG_SZ", 0, "VD_OG_SZ", 0);
    rollo_lock(6, 0, "RO_OG_SZ");
    rollo_lock(2, 100, "RO_OG_SZ");
    long_short(5, "LI_OG_SZ_L1", 3, "LI_OG_SZ", 0);
    long_short(5, "none", 3, "LI_OG_SZ_L1", 0);
    long_short(5, "none", 3, "LI_OG_SZ_L2", 0);
    long_short(1, "LI_OG_SZ_L2", 3, "LI_OG_SZ", 0);
    long_short(1, "none", 3, "LI_OG_SZ_L1", 0);
    long_short(1, "none", 3, "LI_OG_SZ_L2", 0);
    long_short(7, "LI_OG_GA_L1", 3, "LI_OG_GA", 3);
    long_short(9, "LI_OG_GA_L1", 3, "LI_OG_GA", 3);
    simple(10, 3, "LI_OG_BA");
    rollo_lock(11, 0, "RO_OG_BA");
    rollo_lock(8, 100, "RO_OG_BA");
    rollo_lock(13, 0, "RO_OG_BA");
    rollo_lock(12, 100, "RO_OG_BA");
    simple(4, 1, "ZE_EG_VH");
    simple(4, 1, "ZE_GR_2");
}
