#include "node.h"

#include <avr/boot.h>
#include <avr/wdt.h>
#include <Ethernet.h>
#define NO_OTA_PORT  // no mDNS: saves a W5100 socket, uploads go to the ip
#include <ArduinoOTA.h>

#if __has_include("secret_ota.h")
#include "secret_ota.h"
#else
#error "include/secret_ota.h is missing: copy include/example_secret_ota.h and set the password"
#endif

// the upload blocks the loop: keep the watchdog quiet while bytes arrive,
// a stalled upload still ends in a watchdog reset
class WatchdogStorage : public OTAStorage {
  public:
    int open(int length) override { return InternalStorage.open(length); }
    size_t write(uint8_t b) override {
        wdt_reset();
        return InternalStorage.write(b);
    }
    void close() override { InternalStorage.close(); }
    void clear() override { InternalStorage.clear(); }
    void apply() override { InternalStorage.apply(); }
    long maxSize() override { return InternalStorage.maxSize(); }
};

WatchdogStorage ota_storage;
bool ota_running = false;

// apply() calls copy_flash_pages of Optiboot at the start of a 1 KB boot section,
// with the stock 8 KB stk500v2 bootloader that address holds something else
bool ota_supported() {
    cli();
    uint8_t high_fuse = boot_lock_fuse_bits_get(GET_HIGH_FUSE_BITS);
    sei();
    bool bootrst = !(high_fuse & _BV(FUSE_BOOTRST));
    bool boot_1k = (high_fuse & 0x06) == 0x06;  // BOOTSZ1:0 = 11
    return bootrst && boot_1k;
}

void ota_start() {
    Serial.println(F("INFO: OTA upload started."));
    wdt_reset();
}

void ota_before_apply() {
    Serial.println(F("INFO: OTA upload complete, copying to flash."));
    Serial.flush();
    mark_ota_restart();
    wdt_reset();  // copying takes a few seconds, Optiboot resets via watchdog afterwards
}

void ota_error(int code, const char *msg) {
    Serial.print(F("ERROR: OTA upload failed: "));
    Serial.print(code);
    Serial.print(' ');
    Serial.println(msg);
}

// after every DHCP, an ethernet reset also closes the listening socket
void start_ota() {
    if (!ota_supported()) {
        Serial.println(F("ERROR: OTA disabled, no Optiboot (high fuse must be 0xD6)."));
        return;
    }
    ArduinoOTA.onStart(ota_start);
    ArduinoOTA.beforeApply(ota_before_apply);
    ArduinoOTA.onError(ota_error);
    ArduinoOTA.begin(Ethernet.localIP(), node_info.unit_name.c_str(), OTA_PASSWORD, ota_storage);
    ota_running = true;
    Serial.print(F("INFO: OTA ready on port "));
    Serial.println(OTA_PORT);
}

bool ota_ready() { return ota_running; }

void handle_ota() {
    if (ota_running)
        ArduinoOTA.poll();
}
