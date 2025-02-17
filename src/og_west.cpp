#include "node.h"

void user_logic() {
    long_short(15, "LI_OG_GA_L1", 3, "LI_OG_GA", 3);
    long_short(13, "DF_OG_GA", 0, "VD_OG_GA", 0);
    long_short(10, "DF_OG_GA", 100, "VD_OG_GA", 100);
    long_short(1, "LI_OG_GA_L1", 3, "LI_OG_GA", 3);
    simple(14, 3, "LI_OG_KN");
    simple(9, 3, "LI_OG_KN_L1");
    long_short(11, "DF_OG_KN", 100, "VD_OG_KN", 100);
    long_short(8, "DF_OG_KN", 0, "VD_OG_KN", 0);
    rollo_lock(4, 0, "RO_OG_KN");
    rollo_lock(6, 100, "RO_OG_KN");
    simple(0, 3, "LI_OG_KS");
    simple(12, 3, "LI_OG_KS_L1");
    long_short(2, "DF_OG_KS", 0, "VD_OG_KS", 0);
    long_short(3, "DF_OG_KS", 100, "VD_OG_KS", 100);
    rollo_lock(5, 100, "RO_OG_KS");
    rollo_lock(7, 0, "RO_OG_KS");
  }

  //this is the node specific config
const IPAddress ip(192, 168, 178, 217);
const IPAddress server(192, 168, 178, 23);
node_t node_info = {
  "og_west",
  ip,
  server,
  8888,
  53,
  12,
  { 0xDE, 0xAA, 0x7E, 0xE1, 0x1E, 0x16 },
};

const int num_temps = 3;
const long period_t = 60000;

temp_t temps[num_temps] = {
  { "OG_KN", 28, 0, 0, 0 },
  { "OG_KS", 27, 0, 0, 0 },
  { "OG_GA", 26, 0, 0, 0 },
};

const int num_outputs = 24;

output_t outputs[num_outputs] = {
  { "LI_34", 34, 1, 0, 0 },
  { "RO_OG_KS_DO", 35, 1, 0, 0 },
  { "LI_OG_KS", 36, 1, 0, 0 },
  { "RO_OG_KS_ON", 37, 1, 0, 0 },
  { "LI_OG_KS_L1", 42, 1, 0, 0 },
  { "LI_OG_KN_L1", 43, 1, 0, 0 },
  { "LI_OG_KN", 44, 1, 0, 0 },
  { "RO_OG_KN_ON", 45, 1, 0, 0 },
  { "LI_GA_L1", 46, 1, 0, 0 },
  { "RO_OG_KN_DO", 47, 1, 0, 0 },
  { "LI_OG_GA_L1", 48, 1, 0, 0 },
  { "LI_OG_GA", 49, 1, 0, 0 },
  { "DF_OG_GA_UP", 54, 0, 0, 0 },
  { "DF_OG_GA_DO", 55, 0, 0, 0 },
  { "DF_OG_KS_UP", 56, 0, 0, 0 },
  { "DF_OG_KS_DO", 57, 0, 0, 0 },
  { "VD_OG_GA_UP", 58, 0, 0, 0 },
  { "DF_OG_KN_UP", 59, 0, 0, 0 },
  { "VD_OG_GA_DO", 60, 0, 0, 0 },
  { "DF_OG_KN_DO", 61, 0, 0, 0 },
  { "VD_OG_KN_DO", 30, 0, 0, 0 },
  { "VD_OG_KS_UP", 31, 0, 0, 0 },
  { "VD_OG_KN_UP", 32, 0, 0, 0 },
  { "VD_OG_KS_DO", 33, 0, 0, 0 },
};

const int num_rollos = 8;

rollo_t rollos[num_rollos] = {
  { "RO_OG_KN", "RO_OG_KN_ON", "RO_OG_KN_DO", 31000, true, 50, 50, 0, false },
  { "RO_OG_KS", "RO_OG_KS_ON", "RO_OG_KS_DO", 31000, true, 50, 50, 0, false },
  { "DF_OG_KN", "DF_OG_KN_UP", "DF_OG_KN_DO", 500, false, 50, 50, 0, false },
  { "DF_OG_KS", "DF_OG_KS_UP", "DF_OG_KS_DO", 500, false, 50, 50, 0, false },
  { "DF_OG_GA", "DF_OG_GA_UP", "DF_OG_GA_DO", 500, false, 50, 50, 0, false },
  { "VD_OG_KS", "VD_OG_KS_UP", "VD_OG_KS_DO", 500, false, 50, 50, 0, false },
  { "VD_OG_KN", "VD_OG_KN_UP", "VD_OG_KN_DO", 500, false, 50, 50, 0, false },
  { "VD_OG_GA", "VD_OG_GA_UP", "VD_OG_GA_DO", 500, false, 50, 50, 0, false },
};

const int num_valves = 0;

valve_t valves[num_valves] = {
  //{ "U_34", "34", 0 },
  //{ "U_34", "35", 0},
  //{ "U_34", "36", 0},
  //{ "U_34", "37", 0},
  //{ "U_34", "38", 0},
  //{ "U_34", "39", 0},
  //{ "U_34", "40", 0},
  //{ "U_34", "41", 0},
};

const int num_switches = 16;

switch_t switches[num_switches] = {
  { 4, 0, 0, 0, 0, 0 },   //bini oben
  { 5, 0, 0, 0, 0, 0 },   //gang bini
  { 6, 0, 0, 0, 0, 0 },   //bini hoch
  { 7, 0, 0, 0, 0, 0 },   //bini runter
  { 8, 0, 0, 0, 0, 0 },   //leo a hoch
  { 9, 0, 0, 0, 0, 0 },   //bini a runter
  { 10, 0, 0, 0, 0, 0 },  //leo a runter
  { 11, 0, 0, 0, 0, 0 },  //bini a hoch
  { 25, 0, 0, 0, 0, 0 },  //leo hoch
  { 24, 0, 0, 0, 0, 0 },  //leo unten
  { 23, 0, 0, 0, 0, 0 },  //gang runter
  { 22, 0, 0, 0, 0, 0 },  //leo runter
  { 17, 0, 0, 0, 0, 0 },  //bini unten
  { 16, 0, 0, 0, 0, 0 },  //gang hoch
  { 3, 0, 0, 0, 0, 0 },   //leo oben
  { 2, 0, 0, 0, 0, 0 },   //gang leo
};

const int num_timers = 0;
timer_t timers[num_timers] = {};