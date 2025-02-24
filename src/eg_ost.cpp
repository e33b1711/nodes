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
uint8_t mac[] =  {0xDE, 0xBB, 0x7E, 0xE1, 0xAA, 0x02};
#else
uint8_t mac[] =  {0xDE, 0xBB, 0x7E, 0xE1, 0xAB, 0x02};
#endif


node_t node_info = {
        "eg_ost", server, port, mac,
};

const int num_switches = 16;
switch_t switches[num_switches] = {
        {"", false, 4, 0, 0, 0, false, false, false, false},  // 0   ht innen, unten  / treppe unten
        {"", false, 5, 0, 0, 0, false, false, false, false},  // 1   ht innen, oben
        {"", false, 6, 0, 0, 0, false, false, false, false},  // 2
        {"", false, 7, 0, 0, 0, false, false, false, false},  // 3
        {"", false, 8, 0, 0, 0, false, false, false, false},  // 4   gang bei wz
        {"", false, 9, 0, 0, 0, false, false, false, false},  // 5   wz bei gang
        {"", false, 10, 0, 0, 0,false, false, false, false},  // 6   garda
        {"", false, 11, 0, 0, 0,false, false, false, false},  // 7
        {"", false, 25, 0, 0, 0,false, false, false, false},  // 8   treppe auf
        {"", false, 24, 0, 0, 0,false, false, false, false},  // 9   wc
        {"", false, 23, 0, 0, 0,false, false, false, false},  // 10  kachelofen
        {"", false, 22, 0, 0, 0,false, false, false, false},  // 11  treppe runter
        {"", false, 17, 0, 0, 0,false, false, false, false},  // 12  wz bei ez
        {"", false, 16, 0, 0, 0,false, false, false, false},  // 13  klingel
        {"", false, 3, 0, 0, 0, false, false, false, false},  // 14
        {"", false, 2, 0, 0, 0, false, false, false, false},  // 15  treppe oben
};

const int num_temps = 0;
const long period_t = 60000;
temp_t temps[num_temps] = {};

const int num_outputs = 8;
output_t outputs[num_outputs] = {
        {"LI_EG_WZ", 41, 1, 0, 0},     //
        {"LI_EG_WZ_L1", 42, 1, 0, 0},  //
        {"LI_EG_WC", 43, 1, 0, 0},     //
        {"LI_GA_L1", 44, 1, 0, 0},     //
        {"LI_EG_GR", 45, 1, 0, 0},     //
        {"LI_EG_GA", 46, 1, 0, 0},     //
        {"LI_EG_WZ_L2", 47, 1, 0, 0},  //
        {"LI_EG_VH", 49, 1, 0, 0},     //
}; 

const int num_thermos = 0;
thermos_t thermos[num_thermos] = {};

const int num_rollos = 0;
rollo_t rollos[num_rollos] = {};

const int num_valves = 7;
valve_t valves[num_valves] = {
        {"V_EG_WZ", "", 34, 0},  // 34 rot    Wohnzimmer
        {"V_EG_GR", "V_EG_WC", 35, 0},  // 35 rosa   Garda
        {"V_EG_EZ", "V_EG_E2", 36, 0},  // 36 grau   EZ
        {"V_EG_E2", "", 37, 0},  // 37 gelb   EZ
        {"V_EG_WC", "", 38, 0},  // 38 blau   WC
        {"V_EG_GA", "V_EG_GR", 39, 0},  // 39 grün   Gang
        {"V_EG_KU", "", 40, 0},  // 40 weiß   Küche
};

const int num_timers = 1;
ntimer_t timers[num_timers] = {{"ZE_EG_VH", "LI_EG_VH", false, false, 0, 180}};

const int num_pwms = 0;
pwm_t pwms[num_pwms] = {};

void user_logic() {
    long_short(6, "LI_EG_GR", 3, "LI_EG_AO", 3);
    toggle_couple(0, "LI_EG_GA");
    simple(0, 1, "ZE_EG_VH");
    simple(0, 1, "ZE_GR_2");
    simple(4, 3, "LI_EG_GA");
    long_short(5, "LI_EG_WZ", 0, "LI_EG_WZ", 3);
    long_short(5, "LI_EG_WZ_L1", 1, "LI_EG_WZ_L1", 3);
    long_short(5, "LI_EG_WZ_L2", 0, "LI_EG_WZ_L2", 3);
    long_short(12, "LI_EG_WZ", 0, "LI_EG_WZ", 3);
    long_short(12, "LI_EG_WZ_L1", 1, "LI_EG_WZ_L1", 3);
    long_short(12, "LI_EG_WZ_L2", 0, "LI_EG_WZ_L2", 3);
    long_short_extern(10, "LI_EG_EZ_KU", 3, "LI_EG_EZ_L3", 3);
    simple(9, 3, "LI_EG_WC");
    long_short(15, "LI_OG_GA_L1", 3, "LI_OG_GA", 3);
    long_short_extern(11, "DF_OG_GA", 100, "VD_OG_GA", 100);
    long_short_extern(8, "DF_OG_GA", 0, "VD_OG_GA", 0);
    simple(13, 1, "ZE_BELL");
}
