
#include "node.h"
#include "switch.h"
#include "output.h"
#include "temp.h"
#include "timer.h"
#include "rollo.h"
#include "valve.h"
#include "pwm.h"
#include "thermos.h"


#ifndef __TEST__
const IPAddress ip(1, 1, 1, 1); 
const int port = 8880;
#else
const IPAddress ip(1, 1, 1, 1);
const int port = 8889;
#endif

const IPAddress server(192, 168, 178, 23);
node_t node_info = {
        "au", ip, server, port, 53, 12, {0xDE, 0xBB, 0x7E, 0xE1, 0x1E, 0x10},
};

const int num_switches = 0;
switch_t switches[num_switches] = {};

const int num_temps = 1;
const long period_t = 60000;
const int ds18b_pin = 61;
temp_t temps[num_temps] = {{"AU", 0, 0, 0, 0},};

const int num_thermos = 1;
thermos_t thermos[num_thermos] = {{"AU", "VALVE", 5, 50, 0.1, 20000, 20.5, 0.0, 0},};

const int num_outputs = 0;
output_t outputs[num_outputs] = {};

const int num_couples = 0;
couple_t couples[num_couples] = {};

const int num_rollos = 0;
rollo_t rollos[num_rollos] = {};

const int num_valves = 0;
valve_t valves[num_valves] = {};

const int num_timers = 0;
ntimer_t timers[num_timers] = {};

const int num_pwms = 0;
pwm_t pwms[num_pwms] = {};

void user_logic() {
}

const char *ssid = "DRGREENTUMB";
const char *pass = "JUMPAROUND2000";