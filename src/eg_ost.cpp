#include "node.h"
#include "switch.h"
#include "output.h"
#include "temp.h"
#include "timer.h"
#include "rollo.h"
#include "valve.h"

#ifndef __TEST__

const IPAddress ip(192, 168, 178, 214);
const IPAddress server(192, 168, 178, 23);
node_t node_info = {
    "eg_ost",
    ip,
    server,
    8888,
    53,
    12,
    {0xDE, 0xAA, 0x7E, 0xE1, 0x1E, 0x13},
};

#else

const IPAddress ip(192, 168, 178, 224);
const IPAddress server(192, 168, 178, 23);
node_t node_info = {
    "eg_ost",
    ip,
    server,
    8889,
    53,
    12,
    {0xDE, 0xBB, 0x7E, 0xE1, 0x1E, 0x13},
};

#endif

const int num_switches = 16;
switch_t switches[num_switches] = {
    {4, 0, 0, 0, 0, 0},  // 0   ht innen, unten  / treppe unten
    {5, 0, 0, 0, 0, 0},  // 1   ht innen, oben
    {6, 0, 0, 0, 0, 0},  // 2
    {7, 0, 0, 0, 0, 0},  // 3
    {8, 0, 0, 0, 0, 0},  // 4   gang bei wz
    {9, 0, 0, 0, 0, 0},  // 5   wz bei gang
    {10, 0, 0, 0, 0, 0}, // 6   garda
    {11, 0, 0, 0, 0, 0}, // 7
    {25, 0, 0, 0, 0, 0}, // 8   treppe auf
    {24, 0, 0, 0, 0, 0}, // 9   wc
    {23, 0, 0, 0, 0, 0}, // 10  kachelofen
    {22, 0, 0, 0, 0, 0}, // 11  treppe runter
    {17, 0, 0, 0, 0, 0}, // 12  wz bei ez
    {16, 0, 0, 0, 0, 0}, // 13  klingel
    {3, 0, 0, 0, 0, 0},  // 14
    {2, 0, 0, 0, 0, 0},  // 15  treppe oben
};

const int num_temps = 0;
const long period_t = 60000;
temp_t temps[num_temps] = {};

const int num_outputs = 8;
output_t outputs[num_outputs] = {
    {"LI_EG_WZ", 41, 1, 0, 0},
    {"LI_EG_WZ_L1", 42, 1, 0, 0},
    {"LI_EG_WC", 43, 1, 0, 0},
    {"LI_GA_L1", 44, 1, 0, 0},
    {"LI_EG_GR", 45, 1, 0, 0},
    {"LI_EG_GA", 46, 1, 0, 0},
    {"LI_EG_WZ_L2", 47, 1, 0, 0},
    {"LI_EG_VH", 49, 1, 0, 0},
};

const int num_couples = 2;
couple_t couples[num_couples] = {
    {"LI_GA_L1"},
    {"LI_EG_GA"},
};

const int num_rollos = 0;
rollo_t rollos[num_rollos] = {};

const int num_valves = 0;
valve_t valves[num_valves] = {
    //{"U_EG_WZ", 34, 0}, // 34 rot    Wohnzimmer
    //{"U_EG_GR", 35, 0}, // 35 rosa   Garda
    //{"U_EG_E1", 36, 0}, // 36 grau   EZ
    //{"U_EG_E2", 37, 0}, // 37 gelb   EZ
    //{"U_EG_WC", 38, 0}, // 38 blau   WC
    //{"U_EG_GA", 39, 0}, // 39 grün   Gang
    //{"U_EG_KU", 40, 0}, // 40 weiß   Küche
};

const int num_timers = 0;
timer_t timers[num_timers] = {
    //{ "ZE_EG_VH", "LI_EG_VH", false, false, 0, 180 }
};

void user_logic()
{
    long_short(6, "LI_EG_GR", 3, "LI_EG_AO", 3);
    simple(0, 3, "LI_EG_GA"); // coupling TODO
    simple(0, 1, "ZE_EG_VH");
    simple(0, 1, "ZE_GR_2");
    simple(4, 3, "LI_EG_GA");
    long_short(5, "LI_EG_WZ", 0, "LI_EG_WZ", 3);
    long_short(5, "LI_EG_WZ_L1", 1, "LI_EG_WZ_L1", 3);
    long_short(5, "LI_EG_WZ_L2", 0, "LI_EG_WZ_L2", 3);
    long_short(12, "LI_EG_WZ", 0, "LI_EG_WZ", 3);
    long_short(12, "LI_EG_WZ_L1", 1, "LI_EG_WZ_L1", 3);
    long_short(12, "LI_EG_WZ_L2", 0, "LI_EG_WZ_L2", 3);
    long_short(10, "LI_EG_EZ_KU", 3, "LI_EG_EZ_L3", 3);
    simple(9, 3, "LI_EG_WC");
    long_short(15, "LI_OG_GA_L1", 3, "LI_OG_GA", 3);
    long_short(11, "DF_OG_GA", 100, "VD_OG_GA", 100);
    long_short(8, "DF_OG_GA", 0, "VD_OG_GA", 0);
    simple(13, 1, "ZE_BELL");
}
