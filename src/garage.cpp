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

#ifndef __TEST__
const IPAddress ip(1, 1, 1, 1);
const int port = 8880;
#else
const IPAddress ip(1, 1, 1, 1);
const int port = 8889;
#endif

const IPAddress server(192, 168, 178, 23);
node_t node_info = {
        "garage", ip, server, port, 53, 12, {0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA},
};

const int num_switches = 4;
switch_t switches[num_switches] = {
        {"innen", false, 14, 0, 0, 0, false, false, false, false},  //
        {"", false, 27, 0, 0, 0, false, false, false, false},       //
        {"", false, 26, 0, 0, 0, false, false, false, false},       //
        {"", false, 25, 0, 0, 0, false, false, false, false},       //
};

const int num_temps = 0;
const long period_t = 60000;
const int ds18b_pin = 61;
temp_t temps[num_temps] = {
        //{"GR", 22, 0, 0, 0},  // does dht22 work on esp32?
};

const int num_thermos = 0;
thermos_t thermos[num_thermos] = {};

const int num_outputs = 7;
output_t outputs[num_outputs] = {
        {"LI_GR", 4, 0, 0, 0},      //
        {"LI_GR_L1", 21, 0, 0, 0},  //
        {"LI_GR_L2", 16, 0, 0, 0},  //
        {"LI_GR_L3", 17, 0, 0, 0},  //
        {"LI_GR_L4", 5, 0, 0, 0},   //
        {"DO_GR_UP", 18, 0, 0, 0},  //
        {"DO_GR_DO", 19, 0, 0, 0},  //
};

const int num_rollos = 1;

rollo_t rollos[num_rollos] = {
        {"DO_GR", "DO_GR_UP", "DO_GR_DO", 500, false, 50, 50, 0, false},
};

const int num_valves = 0;
valve_t valves[num_valves] = {};

const int num_timers = 3;
ntimer_t timers[num_timers] = {
        {"ZE_GR_0", "LI_GR", false, false, 0, 120},
        {"ZE_GR_1", "LI_GR", false, false, 0, 600},
        {"ZE_GR_2", "ZE_GR_2", false, false, 0, 180},
};

const int num_pwms = 0;
pwm_t pwms[num_pwms] = {};

void user_logic() {
        simple(0, 3, "ZE_GR_0");
        //TODO coupling tor => innenbeleuchtung
        //TODO endlagen tor
        //TODO coupling vh => aussen
}
