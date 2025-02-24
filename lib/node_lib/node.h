// just some global type definitions
#ifndef __node__
#define __node__

#ifdef __esp32__
#include <WiFi.h>
#include <ESPmDNS.h>
const int num_pins = 62;  // TODO esp32 pin alloc?
#else
#include <Ethernet.h>
const int num_pins = 62;
#endif
#include <Arduino.h>

#ifndef __TEST__
const IPAddress server(192, 168, 178, 23);
const int port = 8888;
#else
const IPAddress server(192, 168, 178, 95);
const int port = 8888;
#endif

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

void long_short_extern(const int b_index,
                       const String l_long,
                       const int l_value,
                       const String l_short,
                       const int s_value);

void simple(const int b_index, const int value, const String l);
void rollo_lock(const int b_index, const int value, const String l);
void toggle_couple(const int b_index, const String output);

void alloc_pin(int pin);

bool write_any(String address, int value, bool silent);
bool get_any(String name, String &value);

int covert_value(String value_string);
void execute_message(String type, String name, int value);
void parse_message(String buffer, String &type, String &name, int &value);

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
extern node_t node_info;

extern const String auto_version;

#endif