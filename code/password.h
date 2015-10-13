#ifndef PASSWORD_H
#define PASSWORD_H

#include <stdint.h>

void password_init(uint8_t s);
void password_clear(void);
void password_add(uint8_t c);
uint8_t password_size(void);
const uint8_t * password_getpointer(void);
uint8_t password_getchar(void);
void password_begin(void);
#endif // PASSWORD_H
