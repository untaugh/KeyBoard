#include "eeprom.h"

static uint8_t data[100] EEMEM = {0x2d,0x20,0x65,0x42,0x21,0x29,0x4d,0x48,0x62,0x6e,0x25,0x29,0x35,0x34,0x1e,0x25,0x27,0x33,0x26,0x54};

void eeprom_write(uint8_t *src)
{
  eeprom_write_block(src, data, 100);
}

void eeprom_read(uint8_t *dst)
{
  eeprom_read_block(dst, data, 100);
}
