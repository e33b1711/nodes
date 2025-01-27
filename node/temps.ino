#include "SDHT.h"

SDHT dht;

long s_time_t;

void setup_temps() {
  Serial.println("setup_temps");
  s_time_t = millis();
  for (int i = 0; i < num_temps; i++) {
    handle_one_temp(i);
  }
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

void handle_one_temp(int i) {
  if (!(i < num_temps)) return;
  bool good = dht.read(DHT22, temps[i].pin);
  if (good) {
    send_state("TI_" + temps[i].name, String(dht.celsius));
    send_state("HI_" + temps[i].name, String(dht.humidity));
    Serial.print("handle_one_temp: value_t: ");
    Serial.print(dht.celsius);
    Serial.print(" value_h: ");
    Serial.print(dht.humidity);
    Serial.print("  i: ");
    Serial.println(i);
  } else {
    send_state("TI_" + temps[i].name, "NaN");
    send_state("HI_" + temps[i].name, "NaN");
    //time TODO values store / redo
    Serial.print("handle_one_temp: error reading from dth22. ");
    Serial.print("  i: ");
    Serial.println(i);
  }
}
