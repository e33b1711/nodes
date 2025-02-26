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
node_t node_info = {
        "gr",
        server,
        port,
        mac,
};

const int num_switches = 3;
switch_t switches[num_switches] = {
        {"innen", false, 14, 0, 0, 0, false, false, false, false},      //
        {"GR_CLOSED", false, 27, 0, 0, 0, false, false, false, false},  //
        {"GR_OPEN", false, 26, 0, 0, 0, false, false, false, false},    //
};

const int num_temps = 1;
const long period_t = 60000;
const int ds18b_pin = 61;
temp_t temps[num_temps] = {
        {"GR", 22, 0, 0, 0},  // does dht22 work on esp32?
};

const int num_thermos = 0;
thermos_t thermos[num_thermos] = {};

const int num_outputs = 7;
output_t outputs[num_outputs] = {
        {"LI_GR", 0, 0, 0, 0},      //
        {"LI_GR_L1", 21, 0, 0, 0},  //
        {"LI_GR_L2", 16, 0, 0, 0},  //
        {"LI_GR_L3", 17, 0, 0, 0},  //
        {"LI_GR_L4", 5, 0, 0, 0},   //
        {"DO_GR_DO", 19, 0, 0, 0},  //
        {"DO_GR_UP", 18, 0, 0, 0},  //
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
