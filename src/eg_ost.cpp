#include "node.h"
#include "switch.h"
#include "output.h"
#include "temp.h"
#include "timer.h"
#include "rollo.h"
#include "pwm.h"

#include "version.h"

uint8_t mac[] = {0xDE, 0xBB, 0x7E, 0xE1, 0xCC, 0x02};

node_t node_info = {
        "eg_ost",
        mac,
};

switch_t switches[] = {
        {"", false, 4, "Taster Haustür innen unten"},         // 0
        {"", false, 5, "Taster Haustür innen oben"},          // 1
        {"", false, 6},                                       // 2
        {"", false, 7},                                       // 3
        {"", false, 8, "Taster Gang EG beim Wohnzimmer"},     // 4
        {"", false, 9, "Taster Wohnzimmer beim Gang"},        // 5
        {"", false, 10, "Taster Garderobe"},                  // 6
        {"", false, 11},                                      // 7
        {"", false, 25, "Taster Treppe Dachfenster hoch"},    // 8
        {"", false, 24, "Taster WC"},                         // 9
        {"", false, 23, "Taster Kachelofen"},                 // 10
        {"", false, 22, "Taster Treppe Dachfenster runter"},  // 11
        {"", false, 17, "Taster Wohnzimmer beim Esszimmer"},  // 12
        {"", false, 16, "Klingeltaster"},                     // 13
        {"", false, 3},                                       // 14
        {"", false, 2, "Taster Treppe oben"},                 // 15
};
const int num_switches = COUNT_OF(switches);

const long period_t = 60000;
temp_t temps[] = {};
const int num_temps = COUNT_OF(temps);

output_t outputs[] = {
        {"LI_EG_WZ", 42, 1, HA_LIGHT, "Wohnzimmer Mitte"},           //
        {"LI_EG_WZ_L1", 43, 1, HA_LIGHT, "Wohnzimmer Vorn"},         //
        {"LI_EG_WC", 44, 1, HA_LIGHT, "WC"},                         //
        {"LI_GA_L1", 45, 1, HA_LIGHT, "Gang EG Wandlampen"},         //
        {"LI_EG_GR", 46, 1, HA_LIGHT, "Gardarobe"},                  //
        {"LI_EG_GA", 47, 1, HA_LIGHT, "Gang EG"},                    //
        {"LI_EG_WZ_L2", 48, 1, HA_LIGHT, "Wohnzimmer Hinten"},       //
        {"LI_EG_VH", 49, 1, HA_LIGHT, "Vorhaus"},                    //
        {"V_EG_WZ", 34, true, HA_SWITCH, "Heizventil Wohnzimmer"},   // 34 rot    Wohnzimmer
        {"V_EG_GR", 35, true, HA_SWITCH, "Heizventil Gardarobe"},    // 35 rosa   Garda
        {"V_EG_EZ", 36, true, HA_SWITCH, "Heizventil Esszimmer"},    // 36 grau   EZ
        {"V_EG_E2", 37, true, HA_SWITCH, "Heizventil Esszimmer 2"},  // 37 gelb   EZ
        {"V_EG_WC", 38, true, HA_SWITCH, "Heizventil WC"},           // 38 blau   WC
        {"V_EG_GA", 39, true, HA_SWITCH, "Heizventil Gang EG"},      // 39 grün   Gang
        {"V_EG_KU", 40, true, HA_SWITCH, "Heizventil Küche"},        // 40 weiß   Küche
};
const int num_outputs = COUNT_OF(outputs);

rollo_t rollos[] = {};
const int num_rollos = COUNT_OF(rollos);

ntimer_t timers[] = {{"ZE_EG_VH", "LI_EG_VH", 180, HA_LIGHT, "Vorhaus Timer"}};
const int num_timers = COUNT_OF(timers);

pwm_t pwms[] = {};
const int num_pwms = COUNT_OF(pwms);

void user_logic() {
    long_short(6, "LI_EG_AO", 3, "LI_EG_GR", 3);
    toggle_couple(0, "LI_EG_GA");
    simple(1, 1, "ZE_EG_VH");
    simple(1, 1, "ZE_GR_2");
    toggle_couple(4, "LI_EG_GA");

    int li_eg_wz_inv = 0;
    if (get_output("LI_EG_WZ") == 0)
        li_eg_wz_inv = 1;
    long_short(5, "LI_EG_WZ", 0, "LI_EG_WZ", li_eg_wz_inv);
    long_short(5, "LI_EG_WZ_L1", 1, "LI_EG_WZ_L1", li_eg_wz_inv);
    long_short(5, "LI_EG_WZ_L2", 0, "LI_EG_WZ_L2", li_eg_wz_inv);

    li_eg_wz_inv = 0;
    if (get_output("LI_EG_WZ") == 0)
        li_eg_wz_inv = 1;
    long_short(12, "LI_EG_WZ", 0, "LI_EG_WZ", li_eg_wz_inv);
    long_short(12, "LI_EG_WZ_L1", 1, "LI_EG_WZ_L1", li_eg_wz_inv);
    long_short(12, "LI_EG_WZ_L2", 0, "LI_EG_WZ_L2", li_eg_wz_inv);

    long_short(10, "LI_EG_EZ_L3", 3, "LI_EG_EZ_KU", 3);
    simple(9, 3, "LI_EG_WC");
    long_short(15, "LI_OG_GA", 3, "LI_OG_GA_L1", 3);
    long_short(11, "VD_OG_GA", 100, "DF_OG_GA", 100);
    long_short(8, "VD_OG_GA", 0, "DF_OG_GA", 0);
    simple(13, 1, "ZE_BELL");
}

void user_init() {}