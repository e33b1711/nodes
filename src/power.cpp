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
uint8_t mac[] =  {0xDE, 0xBB, 0x7E, 0xE1, 0xAA, 0x00};
#else
uint8_t mac[] =  {0xDE, 0xBB, 0x7E, 0xE1, 0xAB, 0x00};
#endif


node_t node_info = {
        "power", server, port, mac,
};

const int num_temps = 2;
const long period_t = 100000;
const int ds18b_pin = 61;
temp_t temps[num_temps] = {
        {"PU_O", 51, 0, 0, 0},   //
        {"PU_U", 189, 0, 0, 0},  //
};

const int num_thermos = 0;
thermos_t thermos[num_thermos] = {};

const int num_switches = 1;
switch_t switches[num_switches] = {
        {"F_HE", true, 57, 0, 0, 0, false, false, false, false},   // 0
};

const int num_outputs = 0;
output_t outputs[num_outputs] = {};

const int num_rollos = 0;
rollo_t rollos[num_rollos] = {};

const int num_valves = 0;
valve_t valves[num_valves] = {};

const int num_timers = 0;
ntimer_t timers[num_timers] = {};

const int num_pwms = 1;
pwm_t pwms[num_pwms] = {{"U_EL", 5, 210, 30000, 0, 210, 0}};

void overtemp() {
    // overtemp for elo heating, 85°
    const int max_temp = 90;
    const int thresh_temp = 85;
    const int max_p = 210;
    const int temp_coeff = 40;
    float temp;
    get_temp("TI_PU_O", temp);
    if (temp < thresh_temp)
        set_pwm_max("U_EL", max_p);
    if ((temp > thresh_temp) and (temp < max_temp))
        set_pwm_max("U_EL", (max_temp - temp) * temp_coeff);
    if (temp > max_temp)
        set_pwm_max("U_EL", 0);
}

void user_logic() {
    overtemp();
}