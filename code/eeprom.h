#ifndef EEPROM_H
#define EEPROM_H

#include <avr/eeprom.h>

void eeprom_write(uint8_t *src);
void eeprom_read(uint8_t *dst);

#endif // EEPROM_H
