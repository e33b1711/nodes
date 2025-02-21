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

#ifndef __TEST__
const IPAddress ip(192, 168, 178, 213);
const int port = 8880;
#else
const IPAddress ip(192, 168, 178, 233);
const int port = 8889;
#endif

const IPAddress server(192, 168, 178, 23);
node_t node_info = {
        "power", ip, server, port, 53, 12, {0x4E, 0xBB, 0x7E, 0xEF, 0xFE, 0x04},
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

const int num_switches = 8;
switch_t switches[num_switches] = {
        {57, 0, 0, 0, 0, 0},  // 0
        {3, 0, 0, 0, 0, 0},   // 1
        {16, 0, 0, 0, 0, 0},  // 2
        {17, 0, 0, 0, 0, 0},  // 3
        {22, 0, 0, 0, 0, 0},  // 4
        {23, 0, 0, 0, 0, 0},  // 5
        {24, 0, 0, 0, 0, 0},  // 6
        {25, 0, 0, 0, 0, 0},  // 7
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

void heat_info() {
    // send switch state on transistions
    static bool first = true;
    if ((switches[0].edge != 0) or first){
        send_state("F_HE", switches[0].value);
        first = false;
    }
}

void water_warning() {
    // repeat water after one hour
    static bool first = true;
    static unsigned long ww_time = 0;
    const unsigned long ww_interval = 3600000;
    bool edge = (switches[1].edge != 0);
    bool sometime_ago = ((ww_time + ww_interval) < millis()) and (switches[1].value != 0);
    if (edge or sometime_ago or first) {
        first = false;
        send_state("F_WW", switches[1].value);
        ww_time = millis();
    }
}

void user_logic() {
    heat_info();
    water_warning();
    overtemp();
}