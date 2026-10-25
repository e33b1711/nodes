#include "node.h"
#include "output.h"

#include <avr/wdt.h>
#include <avr/interrupt.h>

// the mega bootloader clears MCUSR, so the reset cause is kept in ram that survives a reset
const uint16_t boot_magic_value = 0xB007;
const uint8_t cause_none = 0;
const uint8_t cause_watchdog = 1;
const uint8_t cause_restart = 2;

volatile uint16_t boot_magic __attribute__((section(".noinit")));
volatile uint8_t boot_cause __attribute__((section(".noinit")));

const char *reset_reason = "";
unsigned int mqtt_connects = 0;
unsigned long uptime_s = 0;

// the watchdog interrupts first and resets on the next timeout
ISR(WDT_vect) {
    if (boot_cause != cause_restart)
        boot_cause = cause_watchdog;
}

// restart by letting the watchdog expire
void restart() {
    Serial.println("INFO: restarting...");
    boot_cause = cause_restart;
    while (true) {
    }
}

void setup_diagnostics() {
    if (boot_magic != boot_magic_value)
        reset_reason = "power_on";  // ram content is random after power loss
    else if (boot_cause == cause_watchdog)
        reset_reason = "watchdog";
    else if (boot_cause == cause_restart)
        reset_reason = "restart";
    else
        reset_reason = "reset";  // reset button, upload, brown-out
    boot_magic = boot_magic_value;
    boot_cause = cause_none;
    WDTCSR |= _BV(WDIE);
    Serial.println("INFO: reset reason: " + String(reset_reason));
}

String revision() { return (dirty ? "dirty_" : "") + auto_version.substring(0, 8); }

int free_ram() {
    extern char __heap_start, *__brkval;
    char top;
    return &top - (__brkval ? __brkval : &__heap_start);
}

void send_diagnostic(const char *key, String value) {
    send_state(node_info.unit_name + "/" + key, value);
}

String ip_address() {
    IPAddress ip = Ethernet.localIP();
    return String(ip[0]) + "." + String(ip[1]) + "." + String(ip[2]) + "." + String(ip[3]);
}

String mac_address() {
    String mac;
    for (int i = 0; i < 6; i++) {
        if (i > 0)
            mac += ':';
        if (node_info.mac[i] < 0x10)
            mac += '0';
        mac += String(node_info.mac[i], HEX);
    }
    mac.toUpperCase();
    return mac;
}

void send_diagnostics() {
    send_retained("nodes/state/" + node_info.unit_name + "/git_revision", revision());
    send_diagnostic("reset_reason", reset_reason);
    send_diagnostic("uptime", String(uptime_s));
    send_diagnostic("free_ram", String(free_ram()));
    send_diagnostic("mqtt_connects", String(mqtt_connects));
    send_diagnostic("ip", ip_address());
    send_diagnostic("mac", mac_address());
    send_diagnostic("failsafe", failsafe_active() ? "1" : "0");
}

void announce_diagnostics() {
    announce_diagnostic("git_revision", "Revision", F(""));
    announce_diagnostic("reset_reason", "Letzter Reset", F(""));
    announce_diagnostic("uptime", "Uptime", F(",\"dev_cla\":\"duration\",\"unit_of_meas\":\"s\""));
    announce_diagnostic("free_ram", "Freier RAM", F(",\"unit_of_meas\":\"B\""));
    announce_diagnostic("mqtt_connects", "MQTT-Verbindungen", F(""));
    announce_diagnostic("ip", "IP-Adresse", F(""));
    announce_diagnostic("mac", "MAC-Adresse", F(""));
    announce_diagnostic("failsafe", "Notbetrieb Ventile", F(""));
}

void handle_diagnostics() {
    static unsigned long last_tick = millis();
    static unsigned long last_post = millis();
    // counted in seconds, survives the millis() overflow after 49 days
    while (millis() - last_tick >= 1000) {
        last_tick += 1000;
        uptime_s++;
    }
    if (millis() - last_post > 60000) {
        last_post = millis();
        send_diagnostics();
    }
}
