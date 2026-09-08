// just some global type definitions
#ifndef __node__
#define __node__


#include <Ethernet.h>
const int num_pins = PIN_A15;
const int pseudo_pin = 99;

#include <Arduino.h>

#ifndef __TEST__
const IPAddress server(192, 168, 178, 73);
const int port = 1883;
#else
const IPAddress server(192, 168, 178, 73);
const int port = 1883;
#endif

void init_debug();
void setup_comm();
void handle_comm();
void handle_debug();

void user_logic();
void user_init();

void send_command(String name, int value);
void send_command(String name, String value);
void send_state(String name, int value);
void send_state(String name, String value);

void long_short(const int b_index,
                const String l_long,
                const int l_value,
                const String l_short,
                const int s_value);

void simple(const int b_index, const int value, const String l);
void rollo_lock(const int b_index, const int value, const String l);
void toggle_couple(const int b_index, const String output);

void alloc_pin(int pin);

void write_any_internal(String address, String val_str);
bool get_any(String name, String &value);

void execute_message(String type, String name, String val_str);
void parse_message(String buffer, String &type, String &name, String &val_str);
int convert_value(String value_string);

struct node_t {
    const String unit_name;
    const IPAddress server;
    const int port;
    uint8_t *mac;
};

extern const IPAddress ip;
extern const IPAddress server;
extern const char *ssid;
extern const char *pass;
extern const byte bssid[];
extern node_t node_info;

extern const String auto_version;
extern const bool dirty;

#endif