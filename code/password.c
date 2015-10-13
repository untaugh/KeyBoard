#include "password.h"

//static uint8_t * password = 0;
static volatile uint8_t password[100];

static volatile uint8_t buffer_size;

static volatile uint8_t char_counter = 0;

/* Init password, with space for s characters. */
void password_init(uint8_t size)
{ 
  // check if password already initialized
  //if (!password)
  //{
      buffer_size = size + 1; // null terminated
      //password = (uint8_t*)malloc(buffer_size);
      password[0] = 0;
      //}
}

/* Free up memory. */
void password_clear(void)
{
  if (password)
    {
      //free(password);
      password[0] = 0;
      buffer_size = 0;
    }
}

/* Add a character to the password. */
void password_add(uint8_t c)
{
  uint8_t size = password_size();

  if (size + 2 < buffer_size)
    {  
      password[size] = c;
      password[size + 1] = 0;
    }
}

/* Number of chars stored (not buffer size). */
uint8_t password_size(void)
{
  uint8_t count = 0;

  if (buffer_size)
    {
      while (password[count] != 0)
	{
	  count++;
	}
      
      return count;
    }
  else
    {
      return 0;
    }
}

/* Get pointer to password string. */
const uint8_t * password_getpointer(void)
{
  return (const uint8_t*)password;
}

/* Get next character in the password. */ 
uint8_t password_getchar(void)
{
  uint8_t c = password[char_counter];

  if (c)
    {
      char_counter++;
      return c;
    }
  else
    {
      return 0;
    }
}

/* Start reading the password from the beginnign*/
void password_begin(void)
{
  char_counter = 0;
}
