#include "buttons.h"

volatile uint8_t left_blue_button = 0; 
volatile uint8_t red_button = 0;
volatile uint8_t right_blue_button = 0; 
volatile uint32_t last_left_blue = 0;
volatile uint32_t last_red = 0;
volatile uint32_t last_last_red = 0;
volatile uint32_t last_right_blue = 0;

// PCINT20 / PD4 / 4 (RX) on Arduino Uno -> left blue button
// PCINT19 / PD3 / 3 (TX) on Arduino UNO -> blue button
// PCINT18 / PD2 / 2 (RX) on Arduino Uno -> right blue button
void Buttons_init() {
	DDRD  &= ~(1 << PD4) | ~(1 << PD3) | ~(1 << PD2);
    PORTD |=  (1 << PD4) |  (1 << PD3) |  (1 << PD2);

    // interrupt on port D
    PCICR |= (1 << PCIE2);

    // interrupt on pins PCINT18 and PCINT19
    PCMSK2 |= (1 << PCINT20) | (1 << PCINT19) | (1 << PCINT18);
}

uint32_t left_blue_count() {
    return systicks - last_left_blue;
}

uint32_t red_count() {
    return systicks - last_last_red;
}

uint32_t right_blue_count() {
    return systicks - last_right_blue;
}

// intrerupere on D
ISR(PCINT2_vect) {
    // interrupt on PCINT20 (inverse logic)
    if (!(PIND & (1 << PD4)) && SYSTICKS_PASSED(last_left_blue, 200)) {
        last_left_blue = systicks;
        left_blue_button = 1;
    }

    // interrupt on PCINT19 (inverse logic)
    if (!(PIND & (1 << PD3)) && SYSTICKS_PASSED(last_red, 200)) {
        milis = systicks % 1000;
        last_last_red = last_red;
        last_red = systicks;
        red_button = 1;
    }
    // interrupt on PCINT18 (inverse logic)
    if (!(PIND & (1 << PD2)) && SYSTICKS_PASSED(last_right_blue, 200)) {
        last_right_blue = systicks;
        right_blue_button = 1;
    }
}