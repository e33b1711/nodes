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

#include "pinio.h"
#include <DallasTemperature.h>

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

const int num_temps = 4;
const long period_t = 6000;
const int ds18b_pin = 14;
temp_t temps[num_temps] = {
        {"SA",  14, {0x28, 0x16, 0xD0, 0xBD, 0x00, 0x00, 0x00, 0xA5}, DS18B20_T, 0, 0, 0},
        {"SA1", 99, {0x28, 0xBF, 0x28, 0x54, 0x00, 0x00, 0x00, 0x2D}, DS18B20_T, 0, 0, 0},
        {"SA2", 99, {0x28, 0xB4, 0xE8, 0xBD, 0x00, 0x00, 0x00, 0x2D}, DS18B20_T, 0, 0, 0},
        {"SA3", 99, {0x28, 0x10, 0x52, 0xBC, 0x00, 0x00, 0x00, 0xD0}, DS18B20_T, 0, 0, 0},
};

// gelb 0x28, 0x16, 0xD0, 0xBD, 0x00, 0x00, 0x00, 0xA5
// rot  0x28, 0xBF, 0x28, 0x54, 0x00, 0x00, 0x00, 0x2D
// grün 0x28, 0xB4, 0xE8, 0xBD, 0x00, 0x00, 0x00, 0x2D
// blau 0x28, 0x10, 0x52, 0xBC, 0x00, 0x00, 0x00, 0xD0
// schw 0x28, 0x82, 0x11, 0x54, 0x00, 0x00, 0x00, 0xFD


const int num_thermos = 1;
thermos_t thermos[num_thermos] = {
        {"SA", "V_SA", 120, 0.2, 95.0, 5.0, 6.0, 0.0},
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
        {"V_SA", "", 3, true, 0},
};

const int num_timers = 0;
ntimer_t timers[num_timers] = {};

const int num_pwms = 0;
pwm_t pwms[num_pwms] = {};

void user_logic() {
    delay(200);
    // TODO better ensure 200ms cycle
    simple(0, 3, "LI_SA_L1");
    simple(1, 3, "LI_SA_L2");
    simple(2, 3, "LI_SA_L3");
}

void user_init() {}
