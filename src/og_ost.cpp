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

const int num_thermos = 2;
thermos_t thermos[num_thermos] = {
        {"OG_BA", "V_OG_BA", 5, 50, 0.1, 60000, 20.5, 0.0, 0},
        {"OG_SZ", "V_OG_SZ", 5, 50, 0.1, 60000, 20.5, 0.0, 0},
};

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

const int num_rollos = 4;
rollo_t rollos[num_rollos] = {
        {"RO_OG_SZ", "RO_OG_SZ_ON", "RO_OG_SZ_DO", 31000, true, 50, 50, 0, false},
        {"RO_OG_BA", "RO_OG_BA_ON", "RO_OG_BA_DO", 31000, true, 50, 50, 0, false},
        {"DF_OG_SZ", "DF_OG_SZ_UP", "DF_OG_SZ_DO", 500, false, 50, 50, 0, false},
        {"VD_OG_SZ", "VD_OG_SZ_UP", "VD_OG_SZ_DO", 500, false, 50, 50, 0, false},
};

const int num_valves = 5;
valve_t valves[num_valves] = {
        {"V_OG_GA", 34, 0, 0},  // blau
        {"V_OG_KS", 35, 0, 0},  // weiß
        {"V_OG_KN", 36, 0, 0},  // grün
        {"V_OG_BA", 37, 0, 0},  // rosa
        {"V_OG_SZ", 38, 0, 0},  // grau
};

const int num_switches = 16;
switch_t switches[num_switches] = {
        {"", false, 4, 0, 0, 0, false, false, false, false},   // 0 sz unten
        {"", false, 5, 0, 0, 0, false, false, false, false},   // 1 ans
        {"", false, 6, 0, 0, 0, false, false, false, false},   // 2 sz rollo runter
        {"", false, 7, 0, 0, 0, false, false, false, false},   // 3 sz oben
        {"", false, 8, 0, 0, 0, false, false, false, false},   // 4 bewegung
        {"", false, 9, 0, 0, 0, false, false, false, false},   // 5 mell
        {"", false, 10, 0, 0, 0, false, false, false, false},  // 6 sz rollohoch
        {"", false, 11, 0, 0, 0, false, false, false, false},  // 7 gang sz
        {"", false, 25, 0, 0, 0, false, false, false, false},  // 8 bad runter tür
        {"", false, 24, 0, 0, 0, false, false, false, false},  // 9 gang bad
        {"", false, 23, 0, 0, 0, false, false, false, false},  // 10 bad
        {"", false, 22, 0, 0, 0, false, false, false, false},  // 11 bad hoch tür
        {"", false, 17, 0, 0, 0, false, false, false, false},  // 12 bad fenster runter
        {"", false, 16, 0, 0, 0, false, false, false, false},  // 13 bad fenster hoch
        {"", false, 3, 0, 0, 0, false, false, false, false},   // 14 df sz hoch
        {"", false, 2, 0, 0, 0, false, false, false, false},   // 15 df sz runter
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
