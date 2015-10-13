#ifndef PORT_H
#define PORT_H

#define LED_RED PD0
#define LED_GREEN PD1
#define LED_YELLOW PC7
#define LED_BLUE PC6

#define BUTTON PD4

#define toggle_led_red PIND |= _BV(LED_RED)
#define toggle_led_green PIND |= _BV(LED_GREEN)
#define toggle_led_blue PINC |= _BV(LED_BLUE)
#define toggle_led_yellow PINC |= _BV(LED_YELLOW)

#define set_led_red PORTD |= _BV(LED_RED)
#define set_led_green PORTD |= _BV(LED_GREEN)
#define set_led_blue PORTC |= _BV(LED_BLUE)
#define set_led_yellow PORTC |= _BV(LED_YELLOW)

#define clr_led_red PORTD &= ~_BV(LED_RED)
#define clr_led_green PORTD &= ~_BV(LED_GREEN)
#define clr_led_blue PORTC &= ~_BV(LED_BLUE)
#define clr_led_yellow PORTC &= ~_BV(LED_YELLOW)

#endif // PORT_H
