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

#ifndef __TEST__
uint8_t mac[] =  {0xDE, 0xBB, 0x7E, 0xE1, 0xBA, 0x01};
#else
uint8_t mac[] =  {0xDE, 0xBB, 0x7E, 0xE1, 0xBB, 0x01};
#endif


node_t node_info = {
        "ch", server, port, mac,
};

const int num_switches = 0;
switch_t switches[num_switches] = {
};

const int num_temps = 1;
const long period_t = 60000;
const int ds18b_pin = 61;
temp_t temps[num_temps] = {
        {"CH", 22, 0, 0, 0},
};

const int num_thermos = 0;
thermos_t thermos[num_thermos] = {};

const int num_outputs = 8;
output_t outputs[num_outputs] = {
        {"DO_CH_UP", 17, 0, 0, 0},  //
        {"DO_CH_DO", 19, 0, 0, 0},  //
        {"PUMP", 18, 0, 0, 0},      //
        {"LI_CH", 5, 0, 0, 0},      //
        {"LI_CH_L2", 25, 0, 0, 0},  //
        {"LI_CH_L3", 26, 0, 0, 0},  //
        {"LI_CH_L4", 27, 0, 0, 0},  //
};

const int num_rollos = 1;

rollo_t rollos[num_rollos] = {
        {"DO_CH", 8000, true, false, "", "", 50, 50, 0, false},
};

const int num_valves = 0;
valve_t valves[num_valves] = {};

const int num_timers = 0;
ntimer_t timers[num_timers] = {};

const int num_pwms = 0;
pwm_t pwms[num_pwms] = {};

void user_logic() {}
