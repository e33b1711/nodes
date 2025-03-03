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
uint8_t mac[] =  {0xDE, 0xBB, 0x7E, 0xE1, 0xAA, 0x06};
#else
uint8_t mac[] =  {0xDE, 0xBB, 0x7E, 0xE1, 0xAB, 0x06};
#endif

node_t node_info = {
        "gr",
        server,
        port,
        mac,
};

const int num_switches = 3;
switch_t switches[num_switches] = {
        {"innen", false, 25, 0, 0, 0, false, false, false, false},      //
        {"GR_CLOSED", false, 24, 0, 0, 0, false, false, false, false},  //
        {"GR_OPEN", false, 23, 0, 0, 0, false, false, false, false},    //
};

const int num_temps = 1;
const long period_t = 60000;
const int ds18b_pin = 61;
temp_t temps[num_temps] = {
        {"GR", 17, 0, 0, 0},  // does dht22 work on esp32?
};

const int num_thermos = 0;
thermos_t thermos[num_thermos] = {};

const int num_outputs = 10;
output_t outputs[num_outputs] = {
        {"LI_GR", 48, 1, 0, 0},      //
        {"LI_GR_L2", 49, 1, 0, 0},  //
        {"DO_GR_DO", 33, 0, 0, 0},  //
        {"DO_GR_UP", 32, 0, 0, 0},  //
        {"DO_GR_DO", 31, 0, 0, 0},  //
        {"DO_GR_UP", 30, 0, 0, 0},  //
        {"DO_GR_DO", 29, 0, 0, 0},  //
        {"DO_GR_UP", 28, 0, 0, 0},  //
        {"DO_GR_DO", 27, 0, 0, 0},  //
        {"DO_GR_UP", 26, 0, 0, 0},  //
};

const int num_rollos = 1;

rollo_t rollos[num_rollos] = {
        {"DO_GR", 500, false, true, "GR_CLOSED", "GR_OPEN", 50, 50, 0, false},
};

const int num_valves = 0;
valve_t valves[num_valves] = {};

const int num_timers = 3;
ntimer_t timers[num_timers] = {
        {"ZE_GR_0", "LI_GR", false, false, 0, 120},
        {"ZE_GR_1", "LI_GR", false, false, 0, 600},
        {"ZE_GR_2", "LI_GR_L2", false, false, 0, 120},
};

const int num_pwms = 0;
pwm_t pwms[num_pwms] = {};

void on_door_open() {
    static int prev_value;
    static bool init = true;
    if (init){
        prev_value = get_rollo("DO_GR");
        init = false;
    }
    if ((prev_value == 100) & (prev_value != get_rollo("DO_GR"))) {
        write_any("ZE_GR_0", "ON");
        write_any("ZE_GR_2", "ON");
        send_command("ZE_EG_VH",1);
        Serial.println("DEBUG: trigger einfahrt licht.");
    }
    prev_value = get_rollo("DO_GR");
}

void user_logic() {
    simple(0, 3, "ZE_GR_1");
    on_door_open();
}
