#include "pinio.h"

void pinio_write(uint8_t pin, uint8_t val) { digitalWrite(pin, val); }
void pinio_mode(uint8_t pin, uint8_t mode) { pinMode(pin, mode); }
int pinio_read(uint8_t pin) { return digitalRead(pin); }

void setup_pinio() { return; }
