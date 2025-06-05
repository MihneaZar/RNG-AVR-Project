#include "buttons.h"

volatile uint8_t left_blue_button = 0; 
volatile uint8_t red_button = 0;
volatile uint8_t right_blue_button = 0; 
volatile uint32_t last_left_blue = 0;
volatile uint32_t last_red = 0;
volatile uint32_t last_last_red = 0;
volatile uint32_t last_right_blue = 0;

// buttons for Arduino Mega (ATmega2560)
// PCINT21 / PK5 / D67 (A14) -> left blue button
// PCINT22 / PK6 / D68 (A15) -> blue button
// PCINT23 / PK7 / D69 (A16) -> right blue button
void Buttons_init() {
	DDRK  &= ~(1 << PK5) | ~(1 << PK6) | ~(1 << PK7);
    PORTK |=  (1 << PK5) |  (1 << PK6) |  (1 << PK7);

    // interrupt on port K
    PCICR |= (1 << PCIE2);

    // interrupt on pins PCINT21, PCINT22 and PCINT23
    PCMSK2 |= (1 << PCINT21) | (1 << PCINT22) | (1 << PCINT23);
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

// interrupt on port K
ISR(PCINT2_vect) {

    // left_blue_button
    // interrupt on PCINT21 (inverse logic)
    if (!(PINK & (1 << PK5)) && SYSTICKS_PASSED(last_left_blue, 200)) {
        last_left_blue = systicks;
        left_blue_button = 1;
    }

    // red_button
    // interrupt on PCINT22 (inverse logic)
    if (!(PINK & (1 << PK6)) && SYSTICKS_PASSED(last_red, 200)) {
        last_last_red = last_red;
        last_red = systicks;
        red_button = 1;
    }

    // right_blue_button
    // interrupt on PCINT23 (inverse logic)
    if (!(PINK & (1 << PK7)) && SYSTICKS_PASSED(last_right_blue, 200)) {
        last_right_blue = systicks;
        right_blue_button = 1;
    }
}