#include "node.h"
#include "switch.h"
#include "output.h"
#include "temp.h"
#include "timer.h"
#include "rollo.h"
#include "pwm.h"

#include "version.h"

uint8_t mac[] = {0xDE, 0xBB, 0x7E, 0xE1, 0xCC, 0x05};

node_t node_info = {
        "og_west",
        mac,
};

const long period_t = 60000;
temp_t temps[] = {
        {"OG_KN", 28, DHT22_T, "Leo"},
        {"OG_KS", 27, DHT22_T, "Bini"},
        {"OG_GA", 26, DHT22_T, "Gang OG"},
};
const int num_temps = COUNT_OF(temps);

output_t outputs[] = {
        {"LI_34", 34, 1},                                   //
        {"RO_OG_KS_DO", 35, 1},                             //
        {"LI_OG_KS", 36, 1, HA_LIGHT, "Bini"},              //
        {"RO_OG_KS_UP", 37, 1},                             //
        {"LI_OG_KS_L1", 42, 1, HA_LIGHT, "Bini Spots"},     //
        {"LI_OG_KN_L1", 43, 1, HA_LIGHT, "Leo Spots"},      //
        {"LI_OG_KN", 44, 1, HA_LIGHT, "Leo"},               //
        {"RO_OG_KN_UP", 45, 1},                             //
        {"LI_GA_L1", 46, 1},                                //
        {"RO_OG_KN_DO", 47, 1},                             //
        {"LI_OG_GA_L1", 48, 1, HA_LIGHT, "Gang OG Spots"},  //
        {"LI_OG_GA", 49, 1, HA_LIGHT, "Gang OG"},           //
        {"VD_OG_KS_DO", 54, 0},                             //
        {"VD_OG_KS_UP", 55, 0},                             //
        {"DF_OG_KS_DO", 56, 0},                             //
        {"DF_OG_KS_UP", 57, 0},                             //
        {"VD_OG_KN_DO", 58, 0},                             //
        {"VD_OG_KN_UP", 59, 0},                             //
        {"DF_OG_KN_UP", 60, 0},                             //
        {"DF_OG_KN_DO", 61, 0},                             //
        {"VD_OG_GA_UP", 30, 0},                             //
        {"DF_OG_GA_DO", 31, 0},                             //
        {"VD_OG_GA_DO", 32, 0},                             //
        {"DF_OG_GA_UP", 33, 0},                             //
};
const int num_outputs = COUNT_OF(outputs);

rollo_t rollos[] = {
        {"RO_OG_KN", 31000, true, "Rollo Leo"},       {"RO_OG_KS", 31000, true, "Rollo Bini"},
        {"DF_OG_KN", 500, false, "Dachfenster Leo"},  {"DF_OG_KS", 500, false, "Dachfenster Bini"},
        {"DF_OG_GA", 500, false, "Dachfenster Gang"}, {"VD_OG_KS", 500, false, "Verdunkelung Bini"},
        {"VD_OG_KN", 500, false, "Verdunkelung Leo"}, {"VD_OG_GA", 500, false, "Verdunkelung Gang"},
};
const int num_rollos = COUNT_OF(rollos);

switch_t switches[] = {
        {"", false, 4},   // bini oben
        {"", false, 5},   // gang bini
        {"", false, 6},   // bini hoch
        {"", false, 7},   // bini runter
        {"", false, 8},   // leo a hoch
        {"", false, 9},   // bini a runter
        {"", false, 10},  // leo a runter
        {"", false, 11},  // bini a hoch
        {"", false, 25},  // leo hoch
        {"", false, 24},  // leo unten
        {"", false, 23},  // gang runter
        {"", false, 22},  // leo runter
        {"", false, 17},  // bini unten
        {"", false, 16},  // gang hoch
        {"", false, 3},   // leo oben
        {"", false, 2},   // gang leo
};
const int num_switches = COUNT_OF(switches);

ntimer_t timers[] = {};
const int num_timers = COUNT_OF(timers);

pwm_t pwms[] = {};
const int num_pwms = COUNT_OF(pwms);

void user_logic() {
    long_short(15, "LI_OG_GA", 3, "LI_OG_GA_L1", 3);
    long_short(13, "VD_OG_GA", 0, "DF_OG_GA", 0);
    long_short(10, "VD_OG_GA", 100, "DF_OG_GA", 100);
    long_short(1, "LI_OG_GA", 3, "LI_OG_GA_L1", 3);
    simple(14, 3, "LI_OG_KN");
    simple(9, 3, "LI_OG_KN_L1");
    long_short(11, "VD_OG_KN", 100, "DF_OG_KN", 100);
    long_short(8, "VD_OG_KN", 0, "DF_OG_KN", 0);
    rollo_lock(4, 0, "RO_OG_KN");
    rollo_lock(6, 100, "RO_OG_KN");
    simple(0, 3, "LI_OG_KS");
    simple(12, 3, "LI_OG_KS_L1");
    long_short(2, "VD_OG_KS", 0, "DF_OG_KS", 0);
    long_short(3, "VD_OG_KS", 100, "DF_OG_KS", 100);
    rollo_lock(5, 100, "RO_OG_KS");
    rollo_lock(7, 0, "RO_OG_KS");
}

void user_init() {}