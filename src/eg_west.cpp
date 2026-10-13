#include "node.h"
#include "switch.h"
#include "output.h"
#include "temp.h"
#include "timer.h"
#include "rollo.h"
#include "pwm.h"

#include "version.h"

uint8_t mac[] = {0xDE, 0xBB, 0x7E, 0xE1, 0xCC, 0x03};

node_t node_info = {
        "eg_west",
        server,
        port,
        mac,
};

switch_t switches[] = {
        {"", false, 4},   // 0 sü hoch
        {"", false, 5},   // 1 we hoch
        {"", false, 6},   // 2 sü runter
        {"", false, 7},   // 3 sü unten
        {"", false, 8},   // 4 ez hoch
        {"", false, 9},   // 5 gang keller oben
        {"", false, 10},  // 6 " unten
        {"", false, 11},  // 7
        {"", false, 25},  // 8 ez runter
        {"", false, 24},  // 9 ez unten
        {"", false, 23},  // 10 gang ez
        {"", false, 22},  // 11 ez mitte
        {"", false, 17},  // 12
        {"", false, 16},  // 13 küche
        {"", false, 3},   // 14 we unten
        {"", false, 2},   // 15 we runter
};
const int num_switches = COUNT_OF(switches);

const long period_t = 60000;
temp_t temps[] = {
        {"EG_GA", 58, DHT22_T, "Gang EG"},  //
        {"EG_KU", 59, DHT22_T, "Küche"},  //
        {"EG_WZ", 60, DHT22_T, "Wohnzimmer"},  //
        {"EG_EZ", 61, DHT22_T, "Esszimmer"},  //
};
const int num_temps = COUNT_OF(temps);


output_t outputs[] = {
        {"LI_EG_EZ_KU", 34, 1, HA_LIGHT, "Esszimmer Grundbeleuchtung"},  //
        {"LI_EG_SP", 35, 1, HA_LIGHT, "Speisekammer"},     //
        {"LI_EG_AS", 36, 1, HA_LIGHT, "Außen Süd"},     //
        {"LI_EG_KU_L1", 37, 1, HA_LIGHT, "Küche Arbeitsbeleuchtung"},  //
        {"LI_EG_EZ_L1", 38, 1, HA_LIGHT, "Esszimmer Wandlampen"},  //
        {"LI_EG_AO", 39, 1, HA_LIGHT, "Außen Ost"},     //
        {"LI_EG_EZ", 40, 1, HA_LIGHT, "Esstisch"},     //
        {"LI_EG_AW", 41, 1, HA_LIGHT, "Außen West"},     //
        {"RO_EG_WE_UP", 42, 1},  //
        {"LI_GA_L1", 43, 1},     //
        {"RO_EG_SU_UP", 44, 1},  //
        {"RO_EG_SU_DO", 45, 1},  //
        {"LI_EG_EZ_KU", 46, 1},  //
        {"LI_EG_EZ_L3", 47, 1, HA_LIGHT, "Esszimmer Leselampe"},  //
        {"RO_EG_WE_DO", 48, 1},  //
        {"LI_EG_GA", 49, 1},     //
};
const int num_outputs = COUNT_OF(outputs);

rollo_t rollos[] = {
        {"RO_EG_SU", 31000, true, "Rollo Esszimmer Süd"},
        {"RO_EG_WE", 31000, true, "Rollo Esszimmer West"},
};
const int num_rollos = COUNT_OF(rollos);

ntimer_t timers[] = {};
const int num_timers = COUNT_OF(timers);

pwm_t pwms[] = {};
const int num_pwms = COUNT_OF(pwms);

void user_logic() {
    toggle_couple(10, "LI_EG_GA");
    simple(9, 3, "LI_EG_EZ_KU");
    long_short(11, "LI_EG_EZ_L1", 3, "LI_EG_EZ", 3);
    simple(4, 0, "RO_EG_SU");
    simple(4, 0, "RO_EG_WE");
    simple(8, 100, "RO_EG_SU");
    simple(8, 100, "RO_EG_WE");
    long_short(3, "none", 1, "LI_EG_AS", 3);
    long_short(3, "PUMP", 3, "none", 3);
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

void user_init() {}