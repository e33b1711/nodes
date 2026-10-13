#include "node.h"
#include "switch.h"
#include "output.h"
#include "temp.h"
#include "timer.h"
#include "rollo.h"
#include "pwm.h"

#include "version.h"

uint8_t mac[] = {0xDE, 0xBB, 0x7E, 0xE1, 0xCC, 0x04};

node_t node_info = {
        "og_ost",
        server,
        port,
        mac,
};

const long period_t = 60000;
temp_t temps[] = {
        {"OG_BA", 27, DHT22_T, "Badezimmer"},
        {"OG_SZ", 26, DHT22_T, "Schlafzimmer"},
};
const int num_temps = COUNT_OF(temps);


output_t outputs[] = {
        {"RO_OG_SZ_UP", 41, 1},  //
        {"LI_OG_BA", 42, 1, HA_LIGHT, "Badezimmer"},     //
        {"RO_OG_BA_DO", 43, 1},  //
        {"LI_OG_SZ", 44, 1, HA_LIGHT, "Schlafzimmer"},     //
        {"LI_OG_SZ_L2", 45, 1, HA_LIGHT, "Leselampe Anselm"},  //
        {"RO_OG_SZ_DO", 46, 1},  //
        {"LI_OG_SZ_L1", 47, 1, HA_LIGHT, "Leselampe Melli"},  //
        {"RO_OG_BA_UP", 49, 1},  //
        {"VD_OG_SZ_DO", 58, 0},  //
        {"DF_OG_SZ_DO", 59, 0},  //
        {"VD_OG_SZ_UP", 60, 0},  //
        {"DF_OG_SZ_UP", 61, 0},  //
        {"V_OG_GA", 34, true, HA_SWITCH, "Heizventil Gang OG"},  // blau
        {"V_OG_KS", 35, true, HA_SWITCH, "Heizventil Bini"},  // weiß
        {"V_OG_KN", 36, true, HA_SWITCH, "Heizventil Leo"},  // grün
        {"V_OG_BA", 37, true, HA_SWITCH, "Heizventil Badezimmer"},  // rosa
        {"V_OG_SZ", 38, true, HA_SWITCH, "Heizventil Schlafzimmer"},  // grau
};
const int num_outputs = COUNT_OF(outputs);

rollo_t rollos[] = {
        {"RO_OG_SZ", 31000, true, "Rollo Schlafzimmer"},
        {"RO_OG_BA", 31000, true, "Rollo Bad"},
        {"DF_OG_SZ", 500, false, "Dachfenster Schlafzimmer"},
        {"VD_OG_SZ", 500, false, "Verdunkelung Schlafzimmer"},
};
const int num_rollos = COUNT_OF(rollos);

switch_t switches[] = {
        {"", false, 4},   // 0 sz unten
        {"", false, 5},   // 1 ans
        {"", false, 6},   // 2 sz rollo runter
        {"", false, 7},   // 3 sz oben
        {"", false, 8},   // 4 bewegung
        {"", false, 9},   // 5 mell
        {"", false, 10},  // 6 sz rollohoch
        {"", false, 11},  // 7 gang sz
        {"", false, 25},  // 8 bad runter tür
        {"", false, 24},  // 9 gang bad
        {"", false, 23},  // 10 bad
        {"", false, 22},  // 11 bad hoch tür
        {"", false, 17},  // 12 bad fenster runter
        {"", false, 16},  // 13 bad fenster hoch
        {"", false, 3},   // 14 df sz hoch
        {"", false, 2},   // 15 df sz runter
};
const int num_switches = COUNT_OF(switches);

ntimer_t timers[] = {};
const int num_timers = COUNT_OF(timers);

pwm_t pwms[] = {};
const int num_pwms = COUNT_OF(pwms);

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