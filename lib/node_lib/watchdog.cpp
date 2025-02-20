#include "node.h"

#ifndef __esp32__

void setup_watchdog(void) {
    Serial.println("INFO: setting up watchdog...");
    cli();  // disable all interrupts
    asm("WDR");
    WDTCSR |= (1 << WDCE) | (1 << WDE);
    WDTCSR = (1 << WDIE) | (1 << WDE) | (1 << WDP3)
             | (1 << WDP0);  // 8s / no interrupt, system reset
    sei();
}

void watchdog_off(void) {
    cli();  // disable all interrupts
    asm("WDR");
    WDTCSR |= (0 << WDCE) | (0 << WDE);
    WDTCSR = (0 << WDIE) | (0 << WDE) | (0 << WDP3)
             | (0 << WDP0);  // 8s / no interrupt, system reset
    sei();
}

ISR(WDT_vect) { Serial.println("WARNING: HW Watchdog Interrupt"); }

void handle_watchdog() { asm("WDR"); }

#endif

#ifdef __esp32__


void setup_watchdog(void) {}

void watchdog_off(void) {}

void handle_watchdog() {}

#endif
