#ifndef BUTTONS_H_
#define BUTTONS_H_

#include <avr/io.h>
#include <avr/interrupt.h>

#include "timer.h"

extern volatile uint32_t sleep_ping;
extern volatile uint8_t left_blue_button; 
extern volatile uint8_t red_button; 
extern volatile uint8_t right_blue_button; 
uint32_t left_blue_count();
uint32_t red_count();
uint32_t right_blue_count();

void Buttons_init();

#endif // BUTTONS_H_
