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
        mac,
};

const long period_t = 60000;
temp_t temps[] = {
        {"OG_BA", 27, DHT22_T, "Badezimmer"},
        {"OG_SZ", 26, DHT22_T, "Schlafzimmer"},
};
const int num_temps = COUNT_OF(temps);

output_t outputs[] = {
        {"RO_OG_SZ_UP", 41, 1},                                          //
        {"LI_OG_BA", 42, 1, HA_LIGHT, "Badezimmer"},                     //
        {"RO_OG_BA_DO", 43, 1},                                          //
        {"LI_OG_SZ", 44, 1, HA_LIGHT, "Schlafzimmer"},                   //
        {"LI_OG_SZ_L2", 45, 1, HA_LIGHT, "Leselampe Anselm"},            //
        {"RO_OG_SZ_DO", 46, 1},                                          //
        {"LI_OG_SZ_L1", 47, 1, HA_LIGHT, "Leselampe Melli"},             //
        {"RO_OG_BA_UP", 49, 1},                                          //
        {"VD_OG_SZ_DO", 58, 0},                                          //
        {"DF_OG_SZ_DO", 59, 0},                                          //
        {"VD_OG_SZ_UP", 60, 0},                                          //
        {"DF_OG_SZ_UP", 61, 0},                                          //
        {"V_OG_GA", 34, true, HA_SWITCH, "Heizventil Gang OG", 1},       // blau
        {"V_OG_KS", 35, true, HA_SWITCH, "Heizventil Bini", 1},          // weiß
        {"V_OG_KN", 36, true, HA_SWITCH, "Heizventil Leo", 1},           // grün
        {"V_OG_BA", 37, true, HA_SWITCH, "Heizventil Badezimmer", 1},    // rosa
        {"V_OG_SZ", 38, true, HA_SWITCH, "Heizventil Schlafzimmer", 1},  // grau
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
        {"", false, 4, "Taster Schlafzimmer unten"},               // 0
        {"", false, 5, "Taster Anselm"},                           // 1
        {"", false, 6, "Taster Schlafzimmer Rollo runter"},        // 2
        {"", false, 7, "Taster Schlafzimmer oben"},                // 3
        {"", false, 8},                                            // 4 frei, war Bewegungsmelder
        {"", false, 9, "Taster Melli"},                            // 5
        {"", false, 10, "Taster Schlafzimmer Rollo hoch"},         // 6
        {"", false, 11, "Taster Gang OG beim Schlafzimmer"},       // 7
        {"", false, 25, "Taster Bad Tür Rollo runter"},            // 8
        {"", false, 24, "Taster Gang OG beim Bad"},                // 9
        {"", false, 23, "Taster Bad"},                             // 10
        {"", false, 22, "Taster Bad Tür Rollo hoch"},              // 11
        {"", false, 17, "Taster Bad Fenster Rollo runter"},        // 12
        {"", false, 16, "Taster Bad Fenster Rollo hoch"},          // 13
        {"", false, 3, "Taster Schlafzimmer Dachfenster hoch"},    // 14
        {"", false, 2, "Taster Schlafzimmer Dachfenster runter"},  // 15
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
}

void user_init() {}