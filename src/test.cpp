#include "node.h"
#include "switch.h"
#include "output.h"
#include "temp.h"
#include "timer.h"
#include "rollo.h"
#include "pwm.h"

#include "version.h"

uint8_t mac[] = {0xDE, 0xBB, 0x7E, 0xE1, 0xCC, 0xAA};

node_t node_info = {
        "test",
        mac,
};

const long period_t = 100000;
temp_t temps[] = {};
const int num_temps = COUNT_OF(temps);

switch_t switches[] = {};
const int num_switches = COUNT_OF(switches);

output_t outputs[] = {

        {"LI_TEST_04", 4, 1},   //
        {"LI_TEST_05", 5, 1},   //
        {"LI_TEST_06", 6, 1},   //
        {"LI_TEST_07", 7, 1},   //
        {"LI_TEST_08", 8, 1},   //
        {"LI_TEST_09", 9, 1},   //
        {"LI_TEST_10", 10, 1},  //
        {"LI_TEST_11", 11, 1},  //

        //{"LI_TEST_25", 25, 1},  //
        //{"LI_TEST_24", 24, 1},  //
        //{"LI_TEST_23", 23, 1},  //
        //{"LI_TEST_22", 22, 1},  //
        //{"LI_TEST_17", 17, 1},  //
        //{"LI_TEST_16", 16, 1},  //
        //{"LI_TEST_03", 3, 1},  //
        //{"LI_TEST_02", 2, 1},  //
        //
        //{"LI_TEST_33", 33, 1},  //
        //{"LI_TEST_32", 32, 1},  //
        //{"LI_TEST_31", 31, 1},  //
        //{"LI_TEST_30", 30, 1},  //
        //{"LI_TEST_29", 29, 1},  //
        //{"LI_TEST_28", 28, 1},  //
        //{"LI_TEST_27", 27, 1},  //
        //{"LI_TEST_26", 26, 1},  //
        //
        //{"LI_TEST_34", 34, 1},  //
        //{"LI_TEST_35", 35, 1},  //
        //{"LI_TEST_36", 36, 1},  //
        //{"LI_TEST_37", 37, 1},  //
        //{"LI_TEST_38", 38, 1},  //
        //{"LI_TEST_39", 39, 1},  //
        //{"LI_TEST_40", 40, 1},  //
        //{"LI_TEST_41", 41, 1},  //
        //
        //{"LI_TEST_42", 42, 1},  //
        //{"LI_TEST_43", 43, 1},  //
        //{"LI_TEST_44", 44, 1},  //
        //{"LI_TEST_45", 45, 1},  //
        //{"LI_TEST_46", 46, 1},  //
        //{"LI_TEST_47", 47, 1},  //
        //{"LI_TEST_48", 48, 1},  //
        //{"LI_TEST_49", 49, 1},  //
        //
        //{"LI_TEST_62", PIN_A0, 1},  //PIN_A8
        //{"LI_TEST_63", PIN_A1, 1},  //PIN_A9
        //{"LI_TEST_64", PIN_A2, 1},  //PIN_A10
        //{"LI_TEST_65", PIN_A3, 1},  //PIN_A11
        //{"LI_TEST_66", PIN_A4, 1},  //PIN_A12
        //{"LI_TEST_67", PIN_A5, 1},  //PIN_A13
        //{"LI_TEST_68", PIN_A6, 1},  //PIN_A14
        //{"LI_TEST_69", PIN_A7, 1},  //PIN_A15
};
const int num_outputs = COUNT_OF(outputs);

rollo_t rollos[] = {};
const int num_rollos = COUNT_OF(rollos);

ntimer_t timers[] = {};
const int num_timers = COUNT_OF(timers);

pwm_t pwms[] = {};
const int num_pwms = COUNT_OF(pwms);

void toggle_all(int even_odd) {
    for (int i = 0; i < num_outputs; i++) {
        write_output(outputs[i].name, (i + even_odd) % 2);
    }
}

void user_logic() {
    const unsigned long period = 60000;
    static unsigned long last_time = millis() - period;
    static int even_odd = 0;
    if ((millis() - last_time) > period) {
        toggle_all(even_odd);
        even_odd++;
        last_time += period;
        Serial.println("INFO: toggling all.");
    }
}

void user_init() {}