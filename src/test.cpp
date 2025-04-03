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
uint8_t mac[] = {0xDE, 0xBB, 0x7E, 0xE1, 0xCC, 0xAA};
#else
uint8_t mac[] = {0xDE, 0xBB, 0x7E, 0xE1, 0xAB, 0x04};
#endif

node_t node_info = {
        "test",
        server,
        port,
        mac,
};


const int num_temps = 0;
const long period_t = 100000;
const int ds18b_pin = 61;
temp_t temps[num_temps] = {};

const int num_thermos = 0;
thermos_t thermos[num_thermos] = {};

const int num_switches = 0;
switch_t switches[num_switches] = {};

const int num_outputs = 48;
output_t outputs[num_outputs] = {
        
        {"LI_TEST_04",  4, 1, 0, 0},  //
        {"LI_TEST_05",  5, 1, 0, 0},  //
        {"LI_TEST_06",  6, 1, 0, 0},  //
        {"LI_TEST_07",  7, 1, 0, 0},  //
        {"LI_TEST_08",  8, 1, 0, 0},  //
        {"LI_TEST_09",  9, 1, 0, 0},  //
        {"LI_TEST_10", 10, 1, 0, 0},  //
        {"LI_TEST_11", 11, 1, 0, 0},  //

        {"LI_TEST_25", 25, 1, 0, 0},  //
        {"LI_TEST_24", 24, 1, 0, 0},  //
        {"LI_TEST_23", 23, 1, 0, 0},  //
        {"LI_TEST_22", 22, 1, 0, 0},  //
        {"LI_TEST_17", 17, 1, 0, 0},  //
        {"LI_TEST_16", 16, 1, 0, 0},  //
        {"LI_TEST_03",  3, 1, 0, 0},  //
        {"LI_TEST_02",  2, 1, 0, 0},  //

        {"LI_TEST_33", 33, 1, 0, 0},  //
        {"LI_TEST_32", 32, 1, 0, 0},  //
        {"LI_TEST_31", 31, 1, 0, 0},  //
        {"LI_TEST_30", 30, 1, 0, 0},  //
        {"LI_TEST_29", 29, 1, 0, 0},  //
        {"LI_TEST_28", 28, 1, 0, 0},  //
        {"LI_TEST_27", 27, 1, 0, 0},  //
        {"LI_TEST_26", 26, 1, 0, 0},  //

        {"LI_TEST_34", 34, 1, 0, 0},  //
        {"LI_TEST_35", 35, 1, 0, 0},  //
        {"LI_TEST_36", 36, 1, 0, 0},  //
        {"LI_TEST_37", 37, 1, 0, 0},  //
        {"LI_TEST_38", 38, 1, 0, 0},  //
        {"LI_TEST_39", 39, 1, 0, 0},  //
        {"LI_TEST_40", 40, 1, 0, 0},  //
        {"LI_TEST_41", 41, 1, 0, 0},  //

        {"LI_TEST_42", 42, 1, 0, 0},  //
        {"LI_TEST_43", 43, 1, 0, 0},  //
        {"LI_TEST_44", 44, 1, 0, 0},  //
        {"LI_TEST_45", 45, 1, 0, 0},  //
        {"LI_TEST_46", 46, 1, 0, 0},  //
        {"LI_TEST_47", 47, 1, 0, 0},  //
        {"LI_TEST_48", 48, 1, 0, 0},  //
        {"LI_TEST_49", 49, 1, 0, 0},  //

        {"LI_TEST_62", PIN_A0, 1, 0, 0},  //PIN_A8  
        {"LI_TEST_63", PIN_A1, 1, 0, 0},  //PIN_A9  
        {"LI_TEST_64", PIN_A2, 1, 0, 0},  //PIN_A10 
        {"LI_TEST_65", PIN_A3, 1, 0, 0},  //PIN_A11 
        {"LI_TEST_66", PIN_A4, 1, 0, 0},  //PIN_A12 
        {"LI_TEST_67", PIN_A5, 1, 0, 0},  //PIN_A13 
        {"LI_TEST_68", PIN_A6, 1, 0, 0},  //PIN_A14 
        {"LI_TEST_69", PIN_A7, 1, 0, 0},  //PIN_A15 
};

const int num_rollos = 0;
rollo_t rollos[num_rollos] = {};

const int num_valves = 0;
valve_t valves[num_valves] = {};

const int num_timers = 0;
ntimer_t timers[num_timers] = {};

const int num_pwms = 0;
pwm_t pwms[num_pwms] = {};

void toggle_all(int even_odd){
        for (int i=0; i<num_outputs; i++){
                write_output(outputs[i].name, (i+even_odd)%2);
        }
}


void user_logic() {
        static unsigned long last_time = millis();
        static int even_odd = 0;
        if (last_time  < millis()){
                toggle_all(even_odd);
                even_odd++;
                last_time += 60000;
                Serial.println("INFO: toggling all.");
        }
}



void user_init() {}