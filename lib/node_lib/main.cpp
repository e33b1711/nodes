//setup and loop functions
#include "node.h"

void setup() {
  init_debug();
  setup_switches();
  setup_comm();
  setup_temps();
  setup_valves();
  //setup_timers();
  setup_outputs();
  setup_watchdog();
}

void loop() {
  handle_watchdog();
  handle_comm();
  update_switches();
  update_temps();
  update_valves();
  update_rollos();
  //handle_timers();
  update_outputs();
  handle_debug();
}
