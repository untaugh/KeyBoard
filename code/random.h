#ifndef RANDOM_H
#define RANDOM_H

#include <stdint.h>

void random_init(volatile uint8_t * timer);
uint8_t random_get(uint8_t limit);


#endif // RANDOM_H
