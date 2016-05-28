#include "blinkmode.h"

// pattern clock
uint8_t blinkmode_clock;

// count pattern state
static uint8_t pattern_counter;

// current pattern
static struct Blinkmode * blinkmode_current_pattern;

// temporary pattern
static struct Blinkmode * blinkmode_temp_pattern;

// temp pattern counter
static uint8_t temp_counter;

static void (*blinkmode_set[8])(void); // array of set functions

static void (*blinkmode_clr[8])(void); // array of clr functions

// match current pattern against led index, and run function if exists
static void process_led(uint8_t i)
{
  if (blinkmode_current_pattern->pattern[pattern_counter] & (1<<i))
    {
      if (blinkmode_set[i] != 0)
	{
	  blinkmode_set[i]();
	}
    }
  else
    {
      if (blinkmode_clr[i] != 0)
	{
	  blinkmode_clr[i]();
	}
    }
}

// match temporary pattern against led index, and run function if exists
static void process_temp_led(uint8_t i)
{
  if (blinkmode_temp_pattern->pattern[pattern_counter] & (1<<i))
    {
      if (blinkmode_set[i] != 0)
	{
	  blinkmode_set[i]();
	}
    }
  else
    {
      if (blinkmode_clr[i] != 0)
	{
	  blinkmode_clr[i]();
	}
    }
}

// make clock move and blink leds if time
void blinkmode_tick(void)
{
  if (blinkmode_clock == 0 && blinkmode_current_pattern)
    {
      if (blinkmode_temp_pattern)
	{
	  blinkmode_clock = blinkmode_temp_pattern->delay;

	  uint8_t i;

	  for (i=0; i<8; i++)
	    {
	      process_temp_led(i);	  
	    }

	  pattern_counter++;
	  
	  if (pattern_counter == blinkmode_current_pattern->size)
	    {
	      pattern_counter = 0;
	      if (--temp_counter == 0)
		{
		  blinkmode_temp_pattern = 0;
		}
	    }
	}
      else
	{
	  blinkmode_clock = blinkmode_current_pattern->delay;
	  
	  uint8_t i;
	  
	  for (i=0; i<8; i++)
	    {
	      process_led(i);	  
	    }
	  
	  pattern_counter++;
	  
	  if (pattern_counter == blinkmode_current_pattern->size)
	    {
	      pattern_counter = 0;
	    }
	}
    }
  
  blinkmode_clock--;
}

// set the set led function
void blinkmode_setset(void setter(), uint8_t index)
{
  blinkmode_set[index] = setter;
}

// set the clear led function
void blinkmode_setclr(void clrer(), uint8_t index)
{
  blinkmode_clr[index] = clrer;
}

// reset everything
void blinkmode_init(void)
{
  blinkmode_clock = 0;

  pattern_counter = 0;

  blinkmode_current_pattern = 0;

  blinkmode_temp_pattern = 0;
  
  int i;

  for (i=0; i<8; i++)
    {
      blinkmode_set[i] = 0;
      blinkmode_clr[i] = 0;
    }
}

void blinkmode_set_pattern(struct Blinkmode * bm)
{
  blinkmode_clock = 0;
  pattern_counter = 0;

  blinkmode_current_pattern = bm;
}

void blinkmode_set_temp(struct Blinkmode * bm,uint8_t count)
{
  blinkmode_clock = 0;
  pattern_counter = 0;

  blinkmode_temp_pattern = bm;
  temp_counter = count;
 
}
