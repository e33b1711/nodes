#include "node.h"
#include "switch.h"
#include "output.h"
#include "temp.h"
#include "rollo.h"
#include "timer.h"
#include "pwm.h"
#include "pinio.h"

#include <avr/wdt.h>

void setup() {
    wdt_enable(WDTO_8S);
    init_debug();
    setup_diagnostics();
    setup_pinio();
    setup_comm();
    wdt_reset();
    setup_switches();
    setup_temps();
    setup_timers();
    setup_outputs();
    setup_rollos();  // after the outputs, a calibration drives them
    setup_pwm();
    user_init();
}

void loop() {
    wdt_reset();
    handle_comm();
    update_switches();
    update_temps();
    update_rollos();
    handle_timers();
    update_outputs();
    update_pwm();
    handle_debug();
    handle_diagnostics();
}
