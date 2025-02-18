#include "node.h"
#include "temp.h"

#include <AM2302-Sensor.h>

long s_time_t;

AM2302::AM2302_Sensor *sensor_array[6];

void setup_temps() {
  Serial.println("setup_temps");
  s_time_t = millis();
  for (int i = 0; i < num_temps; i++) {
    alloc_pin(temps[i].pin);
    sensor_array[i] = new AM2302::AM2302_Sensor{ temps[i].pin };
    sensor_array[i]->begin();
  }
}


void handle_one_temp(int i) {
  if (!(i < num_temps)) return;
  Serial.print(temps[i].name + " ");
  auto status = sensor_array[i]->read();
  if (status != AM2302::AM2302_READ_OK) {
    Serial.print("error: ");
    Serial.println(status);
    if (temps[i].last_update + period_t*10 > millis()) 
      send_state("TI_" + temps[i].name, "NaN");
      send_state("HI_" + temps[i].name, status);
    return;
  }
  temps[i].temp_value = sensor_array[i]->get_Temperature();
  temps[i].humi_value = sensor_array[i]->get_Humidity();
  Serial.print("\t");
  Serial.print(temps[i].temp_value);
  Serial.print("\t");
  Serial.print(temps[i].humi_value);
  Serial.println("succes");
  send_state("TI_" + temps[i].name, temps[i].temp_value);
  send_state("HI_" + temps[i].name, temps[i].humi_value);
  temps[i].last_update = millis();
}


int this_temp = 0;

void update_temps() {
  if (s_time_t + (period_t) < millis()) {
    s_time_t = millis();
    handle_one_temp(this_temp);
    this_temp = (this_temp + 1) % num_temps;
  }
}

bool get_temp(String name, int &value) {
  for (int i = 0; i < num_temps; i++) {
    if ("TI_" + temps[i].name == name) {
      value = temps[i].temp_value;
      return true;
    }
  }
  return false;
}

bool get_humi(String name, int &value) {
  for (int i = 0; i < num_temps; i++) {
    if ("HI_" + temps[i].name == name) {
      value = temps[i].humi_value;
      return true;
    }
  }
  return false;
}
