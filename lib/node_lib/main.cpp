#include "node.h"
#include "switch.h"
#include "output.h"
#include "temp.h"
#include "rollo.h"
#include "timer.h"
#include "valve.h"
#include "pwm.h"
#include "thermos.h"
#include "pinio.h"

void setup() {
    init_debug();
    setup_pinio();
    setup_switches();
    setup_comm();
    setup_temps();
    setup_thermos();
    setup_valves();
    setup_rollos();
    setup_timers();
    setup_outputs();
    setup_pwm();
    user_init();
}

void loop() {
    handle_comm();
    update_switches();
    update_temps();
    update_thermos();
    update_valves();
    update_rollos();
    handle_timers();
    update_outputs();
    update_pwm();
    handle_debug();
}
