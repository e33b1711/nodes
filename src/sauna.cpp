#include "node.h"
#include "switch.h"
#include "output.h"
#include "temp.h"
#include "timer.h"
#include "rollo.h"
#include "valve.h"
#include "pwm.h"
#include "thermos.h"

#include "secret.h"
#include "version.h"
#include "server.h"

uint8_t mac[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};  // unused on esp32
const byte bssid[] = {0x3C, 0xa6, 0x2f, 0xd8, 0x8e, 0xfd};
const int channel = 6;
node_t node_info = {
        "sauna",
        server,
        port,
        mac,
};

const int num_switches = 3;
switch_t switches[num_switches] = {
        {"", false, 16, 0, 0, 0, false, false, false, false},  // 0 sü hoch
        {"", false, 17, 0, 0, 0, false, false, false, false},  // 1 we hoch
        {"", false, 18, 0, 0, 0, false, false, false, false},  // 2 sü runter
};

const int num_temps = 1;
const long period_t = 60000;
temp_t temps[num_temps] = {
        {"CH", 22, DS18B20_T, 0, 0, 0},
};

const int num_thermos = 1;
thermos_t thermos[num_thermos] = {
        {"SA", "V_SA", 120, 0.2, 22, 0.0},
};

const int num_outputs = 3;
output_t outputs[num_outputs] = {
        {"LI_SA_L1", 0, 0, 0, 0},  //
        {"LI_SA_L2", 1, 0, 0, 0},  //
        {"LI_SA_L3", 2, 0, 0, 0},  //
};

const int num_rollos = 0;

rollo_t rollos[num_rollos] = {};

const int num_valves = 1;
valve_t valves[num_valves] = {
        {"V_SA", "", 4, 0},
};

const int num_timers = 0;
ntimer_t timers[num_timers] = {};

const int num_pwms = 0;
pwm_t pwms[num_pwms] = {};

void user_logic() {}

void user_init() {}
