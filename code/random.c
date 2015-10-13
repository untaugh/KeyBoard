#include "random.h"

static volatile uint8_t * timer_p;

void random_init(volatile uint8_t * timer)
{
  timer_p = timer; 
}

uint8_t random_get(uint8_t limit)
{
  return (*timer_p % limit);
}
