#include "eeprom.h"

static uint8_t data[100] EEMEM = "this is password";

void eeprom_write(uint8_t *src)
{
  eeprom_write_block(src, data, 100);
}

void eeprom_read(uint8_t *dst)
{
  eeprom_read_block(dst, data, 100);
}
