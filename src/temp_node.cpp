
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

//#define TEMP_NODE_NAME "AU"

uint8_t mac[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};  // unused on esp32
node_t node_info = {
        TEMP_NODE_NAME "AU",
        server,
        port,
        mac,
};

const int num_switches = 0;
switch_t switches[num_switches] = {};

const int num_temps = 1;
const long period_t = 60000;
const int ds18b_pin = 61;
temp_t temps[num_temps] = {
        {TEMP_NODE_NAME, 6, 0, 0, 0},
};

const int num_thermos = 0;
thermos_t thermos[num_thermos] = {};

const int num_outputs = 0;
output_t outputs[num_outputs] = {};

const int num_rollos = 0;
rollo_t rollos[num_rollos] = {};

const int num_valves = 0;
valve_t valves[num_valves] = {};

const int num_timers = 0;
ntimer_t timers[num_timers] = {};

const int num_pwms = 0;
pwm_t pwms[num_pwms] = {};

void user_logic() {}
