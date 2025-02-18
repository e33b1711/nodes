#include "node.h"
#include "valve.h"

long valve_next_time;
int valve_phase;
const unsigned long valve_interval = 48 * 60 * 1000;
const int max_phase = 16;

void setup_valves() {
  Serial.println("setup_valve");
  for (int i = 0; i < num_valves; i++) {
    alloc_pin(valves[i].pin);
    send_state(valves[i].name, valves[i].value);
    digitalWrite(valves[i].pin, LOW);
    pinMode(valves[i].pin, OUTPUT);
  }
  for (int i = 0; i < num_valves; i++) {
    delay(400);
    digitalWrite(valves[i].pin, HIGH);
  }
  valve_next_time = millis() + valve_interval;
  valve_phase = 0;
}

void update_valves() {
  unsigned long next_time = millis() + valve_interval;
  int i;
  if (valve_next_time <= millis()) {
    valve_next_time += valve_interval;
    valve_phase = (valve_phase + 1) % max_phase;
    for (i = 0; i < num_valves; i++) {
      if (valves[i].value > ((valve_phase + 2 * i) % max_phase)) {
        digitalWrite(valves[i].pin, HIGH);
      } else {
        digitalWrite(valves[i].pin, LOW);
      }
    }
  }
}

bool write_valve(String name, int value, bool silent) {
  int i;
  for (i = 0; i < num_valves; i++) {
    if (valves[i].name == name) {
      if ((value >= 0) and (value < max_phase)) {
        valves[i].value = value;
      }
      if (!silent) send_state(name, valves[i].value);
      return true;
    }
  }
  return false;
}

bool get_valve(String name, int &value) {
  for (int i = 0; i < num_valves; i++) {
    if (valves[i].name == name) {
      value = valves[i].value;
      return true;
    }
  }
  return false;
}
