#ifndef SCANCODE_H
#define SCANCODE_H

#include <stdint.h>

uint8_t scancode(uint8_t ascii);

#define SHIFT 0x80
#define OFFSET 32  // don't care about non-print characters

extern const uint8_t chars_allowed[];

extern const uint8_t chars_allowed_size;

extern const uint8_t scancodetable[];


#endif // SCANCODE_H
