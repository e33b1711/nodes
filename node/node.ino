//just some global type definitions
#include <Ethernet.h>

const int num_pins = 62;
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

struct node_t {
  const String unit_name;
  const IPAddress ip;
  const IPAddress server;
  const int port;
  const int ethernet_sc_pin;
  const int ethernet_reset_pin;
  const byte mac[];
};


struct switch_t {
  const int pin;
  int value;
  int prev_value;
  int edge;
  long last_falling_edge;
  long last_rising_edge;
};

struct temp_t {
  const String name;
  const int pin;
  int temp_value;
  int humi_value;
  long last_update;
};

struct output_t {
  const String name;
  const int pin;
  const bool invert;
  bool value;
  long set_time;
};

struct rollo_t {
  const String name;
  const String l_up;
  const String l_down;
  const int duration;
  const bool is_rollo;
  int value;
  int old_value;
  long stop_time;
  boolean stop_pending;
};

struct valve_t {
  const String name;
  const int pin;
  int value;
};
