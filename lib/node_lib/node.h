//just some global type definitions
#ifndef __node__
#define __node__

#include <Ethernet.h>
#include <Arduino.h>

void init_debug();
void setup_switches();
void setup_comm();
void setup_temps();
void setup_valves();
void setup_outputs();
void setup_watchdog();
void setup_timers();

void handle_watchdog();
void handle_comm();
void update_switches();
void update_temps();
void update_valves();
void update_rollos();
void update_outputs();
void handle_debug();
void handle_timers();

void user_logic();

bool write_valve(String name, int value);
bool get_valve(String name, int &value);

void send_command(String name, int value);
void send_state(String name, int value);
void send_state(String name, String value);

bool write_output(String name, int value);
bool get_output(String name, int &value);

bool write_rollo(String name, int value);
bool get_rollo(String name, int &value);

bool write_timer(String name, int value);
bool get_timer(String name, int &value);

void long_short(const int b_index, const String l_long, const int l_value, const String l_short, const int s_value);
void simple(const int b_index, const int value, const String l);
void rollo_lock(const int b_index, const int value, const String l);

void alloc_pin(int pin);

bool get_temp(String name, int &value);
bool get_humi(String name, int &value);

bool write_any(String address, int value);
bool get_any(String name, int &value);


const int num_pins = 62;


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
  float temp_value;
  float humi_value;
  long last_update;
};

struct output_t {
  const String name;
  const int pin;
  const bool invert;
  bool value;
  long set_time;
};

struct timer_t {
  const String name;
  const String slave;
  bool value;
  bool running;
  long set_time;
  const long duration;
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




void user_logic();


//this is the node specific config
extern const IPAddress ip;
extern const IPAddress server;
extern node_t node_info;

extern const int num_temps;
extern const long period_t;
extern temp_t temps[];

extern const int num_outputs;
extern output_t outputs[];

extern const int num_rollos;
extern rollo_t rollos[];

extern const int num_valves;
extern valve_t valves[];

extern const int num_switches;
extern switch_t switches[];

extern const int num_timers;
extern timer_t timers[];

#endif