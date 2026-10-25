#include "node.h"
#include "switch.h"
#include "output.h"
#include "temp.h"
#include "timer.h"
#include "rollo.h"
#include "pwm.h"

#include "version.h"

uint8_t mac[] = {0xDE, 0xBB, 0x7E, 0xE1, 0xCC, 0x01};

node_t node_info = {
        "ug",
        mac,
};

switch_t switches[] = {
        {"", false, 2},                       // 0
        {"", false, 3},                       // 1
        {"", false, 16},                      // 2 HK
        {"", false, 17},                      // 3 GA_HK
        {"", false, 22},                      // 4 GA
        {"", false, 23},                      // 5 HN
        {"", false, 24},                      // 6 WK
        {"", false, 25},                      // 7 HS
        {"F_HE", true, PIN_A7, "Gastherme"},  // gastherme anforderung
        {"F_WW_NO", true, PIN_A6},            // pumpensumpf NC
        {"F_WW_NC", true, PIN_A5},            // pumepensumpf NO
};
const int num_switches = COUNT_OF(switches);

const long period_t = 60000;
temp_t temps[] = {
        {"PU_O", 8, DS18B20_T, "Puffer oben"},   //
        {"PU_U", 9, DS18B20_T, "Puffer unten"},  //
        {"UG_WK", 26, DHT22_T, "Waschküche"},    //
        {"UG_HO", 27, DHT22_T, "Hobby"},         //
        {"UG_LA", 28, DHT22_T, "Lager"},         //
        {"UG_GA", 29, DHT22_T, "Gang UG"},       //
        {"UG_HK", 30, DHT22_T, "Technik"},       //

};
const int num_temps = COUNT_OF(temps);

output_t outputs[] = {
        {"LI_UG_HO", 42, 1, HA_LIGHT, "Hobby Schreibtisch"},        //
        {"LI_UG_HK", 43, 1, HA_LIGHT, "Technik"},                   //
        {"LI_UG_TR", 44, 1, HA_LIGHT, "UG Treppe"},                 //
        {"BELL", 45, 1, HA_SIREN, "Klingel (dauer)"},               //
        {"PUMP", 46, 1},                                            //
        {"LI_UG_GA", 47, 1, HA_LIGHT, "Gang UG"},                   //
        {"LI_UG_WK", 48, 1, HA_LIGHT, "Waschküche"},                //
        {"LI_UG_HN", 49, 1, HA_LIGHT, "Hobby Hinten"},              //
        {"V_UG_HO", 34, true, HA_SWITCH, "Heizventil Hobby"},       //
        {"V_UG_LA", 35, true, HA_SWITCH, "Heizventil Lager"},       //
        {"V_UG_WK", 36, true, HA_SWITCH, "Heizventil Waschküche"},  //
        {"V_UG_H2", 37, true, HA_SWITCH, "Heizventil Hobby 2"},     //
        {"V_UG_GA", 38, true, HA_SWITCH, "Heizventil Gang UG"},     //
        {"V_UG_H3", 39, true},                                      //
};
const int num_outputs = COUNT_OF(outputs);

rollo_t rollos[] = {};
const int num_rollos = COUNT_OF(rollos);

ntimer_t timers[] = {{"ZE_BELL", "BELL", 5, HA_SIREN, "Klingel"}};
const int num_timers = COUNT_OF(timers);

pwm_t pwms[] = {{"U_EL", 6, 210, 30000, "Elektrische Heizung"}};
const int num_pwms = COUNT_OF(pwms);

void overtemp() {
    // overtemp for elo heating, 85°
    const int max_temp = 90;
    const int thresh_temp = 85;
    const int max_p = 210;
    const int temp_coeff = 40;
    float temp = NAN;
    get_temp("TI_PU_O", temp);
    // no valid temperature: no heating
    if (isnan(temp) or (temp >= max_temp))
        set_pwm_max("U_EL", 0);
    else if (temp > thresh_temp)
        set_pwm_max("U_EL", (max_temp - temp) * temp_coeff);
    else
        set_pwm_max("U_EL", max_p);
}

void water_warning() {
    static unsigned long last_time;
    const unsigned long period = 3600000;
    static bool initial = true;
    if (((millis() - last_time) > period) or initial) {
        initial = false;
        last_time = millis();

        int nc = get_switch("F_WW_NC");
        int no = get_switch("F_WW_NO");

        if (nc == no) {
            Serial.println("ERROR: water warning nc = no");
            send_state("F_WW", "1");
            return;
        }
        if (nc == 0 and no == 1) {
            Serial.println("INFO: water warning!!!");
            send_state("F_WW", "1");
            return;
        }
        Serial.println("INFO: water warning: all good.");
        send_state("F_WW", "0");
        return;
    }
}

void user_logic() {
    simple(2, 3, "LI_UG_HK");
    simple(3, 3, "LI_UG_GA");
    simple(4, 3, "LI_UG_GA");
    long_short(7, "LI_UG_HO", 0, "LI_UG_HO", 3);
    long_short(7, "LI_UG_HN", 0, "none", 3);
    long_short(5, "LI_UG_HN", 0, "LI_UG_HN", 3);
    long_short(5, "LI_UG_HO", 0, "none", 3);
    simple(6, 3, "LI_UG_WK");
    overtemp();
    water_warning();
}

void user_init() {}

void user_discovery() { announce(HA_SENSOR, "F_WW", "Warnung Pumpensumpf"); }
