#ifndef _pinio_h__
#define _pinio_h__

#include <Arduino.h>

void setup_pinio();
void pinio_write(uint8_t pin, uint8_t val);
void pinio_mode(uint8_t pin, uint8_t mode);
int pinio_read(uint8_t pin);

#endif