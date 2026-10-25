// just some global type definitions
#ifndef __node__
#define __node__

#include <Ethernet.h>
const int num_pins = PIN_A15 + 1;
const int pseudo_pin = 99;

#define COUNT_OF(a) (sizeof(a) / sizeof(a[0]))

#include <Arduino.h>

const IPAddress server(192, 168, 178, 73);
const int port = 1883;

// home assistant entity types for mqtt discovery
enum ha_t {
    HA_NONE,  // not announced
    HA_LIGHT,
    HA_SWITCH,
    HA_SIREN,
    HA_COVER,
    HA_COVER_POSITION,  // cover that can be set to a position
    HA_BINARY_SENSOR,
    HA_SENSOR,  // plain value, e.g. an input announced as sensor
    HA_TEMPERATURE,
    HA_HUMIDITY,
    HA_NUMBER,
};

void restart();
void init_debug();
void setup_comm();
void handle_comm();
void handle_debug();

void user_logic();
void user_init();

bool send_command(String name, int value);
bool send_command(String name, String value);
void send_state(String name, int value);
void send_state(String name, String value);
bool send_retained(String topic, String payload);

void send_discovery();
void user_discovery();
void announce(ha_t type, String id, String label, int max_value = 255);
void announce_diagnostic(const char *key, String label, const __FlashStringHelper *extra);
void announce_event(String id, String label, const __FlashStringHelper *event_types);
void send_all_states();

void setup_diagnostics();
void handle_diagnostics();
void send_diagnostics();
void announce_diagnostics();
String revision();
extern unsigned int mqtt_connects;
bool ha_lost();

void long_short(const int b_index,
                const String l_long,
                const int l_value,
                const String l_short,
                const int s_value);

void simple(const int b_index, const int value, const String l);
void rollo_lock(const int b_index, const int value, const String l);
void toggle_couple(const int b_index, const String output);

void alloc_pin(int pin);

void write_any(String address, String val_str);
void write_any_internal(String address, String val_str);
bool get_any(String name, String &value);

void execute_message(String type, String name, String val_str);
void parse_message(String buffer, String &type, String &name, String &val_str);
int convert_value(String value_string);

struct node_t {
    const String unit_name;
    uint8_t *mac;
};

extern node_t node_info;

extern const String auto_version;
extern const bool dirty;

#endif