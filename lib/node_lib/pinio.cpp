#include "pinio.h"

#ifndef __io_exteneder__
void pinio_write(uint8_t pin, uint8_t val) { digitalWrite(pin, val); }
void pinio_mode(uint8_t pin, uint8_t mode) { pinMode(pin, mode); }
int pinio_read(uint8_t pin) { return digitalRead(pin); }

void setup_pinio() { return; }

#else

#include "PCF8574.h"

TwoWire Wire_1 = TwoWire(1);
PCF8574 pcfIn1(&Wire_1, 0x22);  // Channels 1-8
PCF8574 pcfIn2(&Wire_1, 0x21);  // Channels 9-16
// For MOSFET outputs
PCF8574 pcfOut1(&Wire_1, 0x24);  // Channels 1-8
PCF8574 pcfOut2(&Wire_1, 0x25);  // Channels 9-16

void setup_pinio() {
    Wire_1.begin(4, 5, 400000U);  // custom i2c port on ESP

    for (int i = 0; i < 8; i++) {
        pcfOut1.pinMode(i, OUTPUT);
        pcfOut2.pinMode(i, OUTPUT);
    }
    if (pcfOut1.begin()) {
        Serial.println("INFO: pcfOut1 good.");
    } else {
        Serial.println("WARNING: pcfOut1 not good.");
    }
    if (pcfOut2.begin()) {
        Serial.println("INFO: pcfOut2 good.");
    } else {
        Serial.println("WARNING: pcfOut2 not good.");
    }

    for (int i = 0; i < 8; i++) {
        pcfIn1.pinMode(i, INPUT);
        pcfIn2.pinMode(i, INPUT);
    }
    if (pcfIn1.begin()) {
        Serial.println("INFO: pcfIn1 good.");
    } else {
        Serial.println("WARNING: pcfIn1 not good.");
    }
    if (pcfIn2.begin()) {
        Serial.println("INFO: pcfIn2 good.");
    } else {
        Serial.println("WARNING: pcfIn2 not good.");
    }
}

void pinio_write(uint8_t pin, uint8_t val) {
    if (pin < 8) {
        pcfOut1.digitalWrite(pin, val);
        return;
    }
    if (pin < 16) {
        pcfOut2.digitalWrite(pin - 8, val);
        return;
    }
    Serial.println("WARNING: pinio_write pin to high");
    return;
}

void pinio_mode(uint8_t pin, uint8_t mode) { return; }
int pinio_read(uint8_t pin) {
    if (pin < 16) {
        Serial.println("WARNING: pinio_read pin to low");
        return 0;
    }
    if (pin < 24) {
        return pcfIn1.digitalRead(pin - 16);
    }
    if (pin < 32) {
        return pcfIn2.digitalRead(pin - 24);
    }
    Serial.println("WARNING: pinio_write pin to high");
    return 0;
}
#endif