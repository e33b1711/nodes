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
        {"", false, 4, "Taster Bini oben"},                    // 0
        {"", false, 5, "Taster Gang OG bei Bini"},             // 1
        {"", false, 6, "Taster Bini Dachfenster hoch"},        // 2
        {"", false, 7, "Taster Bini Dachfenster runter"},      // 3
        {"", false, 8, "Taster Leo Rollo hoch"},               // 4
        {"", false, 9, "Taster Bini Rollo runter"},            // 5
        {"", false, 10, "Taster Leo Rollo runter"},            // 6
        {"", false, 11, "Taster Bini Rollo hoch"},             // 7
        {"", false, 25, "Taster Leo Dachfenster hoch"},        // 8
        {"", false, 24, "Taster Leo unten"},                   // 9
        {"", false, 23, "Taster Gang OG Dachfenster runter"},  // 10
        {"", false, 22, "Taster Leo Dachfenster runter"},      // 11
        {"", false, 17, "Taster Bini unten"},                  // 12
        {"", false, 16, "Taster Gang OG Dachfenster hoch"},    // 13
        {"", false, 3, "Taster Leo oben"},                     // 14
        {"", false, 2, "Taster Gang OG bei Leo"},              // 15
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