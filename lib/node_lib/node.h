// just some global type definitions
#ifndef __node__
#define __node__

#include <Ethernet.h>
#include <Arduino.h>

#define __TEST__

void init_debug();
void setup_comm();
void setup_watchdog();

void handle_watchdog();
void handle_comm();
void handle_debug();

void user_logic();

void send_command(String name, int value);
void send_state(String name, int value);
void send_state(String name, String value);

void long_short(const int b_index,
                const String l_long,
                const int l_value,
                const String l_short,
                const int s_value);
void simple(const int b_index, const int value, const String l);
void rollo_lock(const int b_index, const int value, const String l);

void alloc_pin(int pin);

bool write_any(String address, int value, bool silent);
bool get_any(String name, int &value);

int covert_value(String value_string);
void execute_message(String type, String name, int value);
void parse_message(String buffer, String &type, String &name, int &value);

const int num_pins = 62;

struct couple_t {
    const String name;
};

struct node_t {
    const String unit_name;
    const IPAddress ip;
    const IPAddress server;
    const int port;
    const int ethernet_sc_pin;
    const int ethernet_reset_pin;
    const byte mac[];
};

extern const IPAddress ip;
extern const IPAddress server;
extern node_t node_info;

extern const int num_couples;
extern couple_t couples[];

#endif