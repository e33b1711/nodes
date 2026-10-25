#ifndef _rollo_h__
#define _rollo_h__
#include <Arduino.h>

void update_rollos();
void setup_rollos();

bool write_rollo(String name, String val_str);
bool write_rollo(String name, int value);
bool get_rollo(String name, String &value);
int get_rollo(String name);

struct rollo_t {
    const String name;
    const int duration;
    const bool is_rollo;
    const char *label;     // friendly name in home assistant, none: not announced
    const bool has_stops;  // uses stop switches
    const String switch_open;
    const String switch_closed;
    int value;
    int last_value;
    unsigned long start_time;
    boolean stop_pending;
    bool trapped;  // both stop switches active, already reported
    // is_rollo: position tracked over the travel time, 0 open, duration closed
    long pos_ms;
    bool known;   // position calibrated by a full travel
    int dir;      // -1 up, 0 stopped, 1 down
    int target;   // percent
    int pending;  // percent to go to after calibration, -1: none
    unsigned long last_tick;
    unsigned long last_report;

    rollo_t(String name,
            int duration,
            bool is_rollo,
            const char *label = nullptr,
            bool has_stops = false,
            String switch_open = "",
            String switch_closed = "")
            : name(name),
              duration(duration),
              is_rollo(is_rollo),
              label(label),
              has_stops(has_stops),
              switch_open(switch_open),
              switch_closed(switch_closed),
              value(50),
              last_value(50),
              start_time(0),
              stop_pending(false),
              trapped(false),
              pos_ms(0),
              known(false),
              dir(0),
              target(0),
              pending(-1),
              last_tick(0),
              last_report(0) {}
};

extern const int num_rollos;
extern rollo_t rollos[];

#endif