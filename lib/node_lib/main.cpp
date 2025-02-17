//setup and loop functions
#include "node.h"

void setup() {

  //init debug interface
  init_debug();

  //init states
  
  setup_switches();
  setup_comm();
  setup_temps();
  setup_valves();
  setup_timers();
  setup_outputs();
  setup_watchdog();
}

void loop() {

  //watchdog
  handle_watchdog();

  //react to messages
  handle_comm();

  //update states
  //user logic is called from update_a_b_c
  update_switches();
  update_temps();
  update_valves();


  //states mit ausgängen zum Schluss
  update_rollos();
  handle_timers();
  update_outputs();
  //update_a_b_c();

  //debug Interface
  handle_debug();
}
