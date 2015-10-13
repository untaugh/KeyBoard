#include "scancode.h"

uint8_t scancode(uint8_t ascii)
{
  return scancodetable[ascii-OFFSET];
}
