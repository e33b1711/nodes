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
uint8_t mac[] = {0xDE, 0xBB, 0x7E, 0xE1, 0xCC, 0x04};
#else
uint8_t mac[] = {0xDE, 0xBB, 0x7E, 0xE1, 0xAB, 0x04};
#endif

node_t node_info = {
        "og_ost",
        server,
        port,
        mac,
};

const int num_temps = 2;
const long period_t = 60000;
temp_t temps[num_temps] = {
        {"OG_BA", 27, {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, DHT22_T, 0, 0, 0},
        {"OG_SZ", 26, {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, DHT22_T, 0, 0, 0},
};

const int num_thermos = 2;
const int full_valve = 254;
const int closed_valve = 2;
const int default_valve = 128;
thermos_t thermos[num_thermos] = {
        {"OG_BA", "V_OG_BA", 120, 0.2, 30.0, 10.0, 23, 0.0},
        {"OG_SZ", "V_OG_SZ", 120, 0.2, 30.0, 10.0, 16, 0.0},
};

const int num_outputs = 12;
output_t outputs[num_outputs] = {
        {"RO_OG_SZ_UP", 41, 1, 0, 0},  //
        {"LI_OG_BA", 42, 1, 0, 0},     //
        {"RO_OG_BA_DO", 43, 1, 0, 0},  //
        {"LI_OG_SZ", 44, 1, 0, 0},     //
        {"LI_OG_SZ_L2", 45, 1, 0, 0},  //
        {"RO_OG_SZ_DO", 46, 1, 0, 0},  //
        {"LI_OG_SZ_L1", 47, 1, 0, 0},  //
        {"RO_OG_BA_UP", 49, 1, 0, 0},  //
        {"VD_OG_SZ_DO", 58, 0, 0, 0},  //
        {"DF_OG_SZ_DO", 59, 0, 0, 0},  //
        {"VD_OG_SZ_UP", 60, 0, 0, 0},  //
        {"DF_OG_SZ_UP", 61, 0, 0, 0},  //
};

const int num_rollos = 4;
rollo_t rollos[num_rollos] = {
        {"RO_OG_SZ", 31000, true, false, "", "", 50, 50, 0, false},
        {"RO_OG_BA", 31000, true, false, "", "", 50, 50, 0, false},
        {"DF_OG_SZ", 500, false, false, "", "", 50, 50, 0, false },
        {"VD_OG_SZ", 500, false, false, "", "", 50, 50, 0, false },
};

  const int num_valves
  = 5;
valve_t valves[num_valves] = {
        {"V_OG_GA", "", 34, false, 0, 0},  // blau
        {"V_OG_KS", "", 35, false, 0, 0},  // weiß
        {"V_OG_KN", "", 36, false, 0, 0},  // grün
        {"V_OG_BA", "", 37, false, 0, 0},  // rosa
        {"V_OG_SZ", "", 38, false, 0, 0},  // grau
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
    long_short(3, "LI_OG_SZ", 0, "LI_OG_SZ", 3);
    long_short(3, "LI_OG_SZ_L1", 0, "none", 3);
    long_short(3, "LI_OG_SZ_L2", 0, "none", 3);
    long_short(0, "LI_OG_SZ", 0, "LI_OG_SZ", 3);
    long_short(0, "LI_OG_SZ_L1", 0, "none", 3);
    long_short(0, "LI_OG_SZ_L2", 0, "none", 3);
    long_short(15, "VD_OG_SZ", 100, "DF_OG_SZ", 100);
    long_short(14, "VD_OG_SZ", 0, "DF_OG_SZ", 0);
    rollo_lock(6, 0, "RO_OG_SZ");
    rollo_lock(2, 100, "RO_OG_SZ");
    long_short(5, "LI_OG_SZ", 0, "LI_OG_SZ_L1", 3);
    long_short(5, "LI_OG_SZ_L1", 0, "none", 0);
    long_short(5, "LI_OG_SZ_L2", 0, "none", 0);
    long_short(1, "LI_OG_SZ", 0, "LI_OG_SZ_L2", 3);
    long_short(1, "LI_OG_SZ_L1", 0, "none", 0);
    long_short(1, "LI_OG_SZ_L2", 0, "none", 0);
    long_short(7, "LI_OG_GA", 3, "LI_OG_GA_L1", 3);
    long_short(9, "LI_OG_GA", 3, "LI_OG_GA_L1", 3);
    simple(10, 3, "LI_OG_BA");
    rollo_lock(11, 0, "RO_OG_BA");
    rollo_lock(8, 100, "RO_OG_BA");
    rollo_lock(13, 0, "RO_OG_BA");
    rollo_lock(12, 100, "RO_OG_BA");
    simple(4, 1, "ZE_EG_VH");
    simple(4, 1, "ZE_GR_2");
}

void user_init() {}