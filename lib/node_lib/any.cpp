#include "node.h"

bool write_any(String name, int value, bool silent) {
  Serial.print("write any: " + name + " ");
  Serial.println(value);
  if (write_valve(name, value, silent)) return true;
  if (write_output(name, value, silent)) return true;
  if (write_rollo(name, value, silent)) return true;
  if (write_timer(name, value, silent)) return true;
  Serial.println("Found no internal.");
  return true;
}

bool get_any(String name, int &value) {
  if (get_temp(name, value)) return true;
  if (get_humi(name, value)) return true;
  if (get_valve(name, value)) return true;
  if (get_rollo(name, value)) return true;
  if (get_output(name, value)) return true;
  if (get_timer(name, value)) return true;
  return false;
}

bool pin_alloc[num_pins] = { false };

void alloc_pin(int pin) {
  if (pin >= num_pins) {
    Serial.println("alloc_pin: undefinied: " + String(pin, DEC));
    Serial.println("Exit...");
    while (true) delay(100);
  }
  if (pin_alloc[pin]) {
    Serial.println("alloc_pin: allready allocated: " + String(pin, DEC));
    Serial.println("Exit...");
    while (true) delay(100);
  }
  pin_alloc[pin] = true;
}

