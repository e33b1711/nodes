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

uint8_t mac[] =  {0x00, 0x00, 0x00, 0x00, 0x00, 0x00}; //unused on esp32
const byte bssid[] = {0x3C, 0xa6, 0x2f, 0xd8, 0x8e, 0xfd};
const int channel = 6;
node_t node_info = {
        "ch", server, port, mac,
};

const int num_switches = 0;
switch_t switches[num_switches] = {
};

const int num_temps = 1;
const long period_t = 60000;
temp_t temps[num_temps] = {
        {"CH", 22, {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, SHT31_T, 0, 0, 0},
};

const int num_thermos = 0;
thermos_t thermos[num_thermos] = {};

const int num_outputs = 7;
output_t outputs[num_outputs] = {
        {"DO_CH_UP", 18, 0, 0, 0},  //
        {"DO_CH_DO", 5, 0, 0, 0},  //
        {"PUMP", 17, 0, 0, 0},      //
        {"LI_CH", 19, 0, 0, 0},      //
        {"LI_CH_L2", 27, 0, 0, 0},  //
        {"LI_CH_L3", 26, 0, 0, 0},  //
        {"LI_CH_L4", 25, 0, 0, 0},  //
};

const int num_rollos = 1;

rollo_t rollos[num_rollos] = {
        {"DO_CH", 8000, false, false, "", "", 50, 50, 0, false},
};

const int num_valves = 0;
valve_t valves[num_valves] = {};

const int num_timers = 0;
ntimer_t timers[num_timers] = {};

const int num_pwms = 0;
pwm_t pwms[num_pwms] = {};

void user_logic() {}

void user_init() {}
