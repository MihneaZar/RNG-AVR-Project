#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdio.h>

#include "usart.h"
#include "menus.h"
#include "timer.h"
#include "ssd1306.h"
#include "buttons.h"
// #include "pff.h"

/***
 * Disables all pull-ups for pins for power-saving.
 * 
 */
void set_pins() {
    DDRB = 0;
    DDRC = 0;
    DDRD = 0;
    PORTB = 0;
    PORTC = 0;
    PORTD = 0;

    DDRB |= (1 << PB5);
    PORTB &= ~(1 << PB5);
}

void init_all() {
    sei();
    set_pins();
    // USART0_init();
    // USART0_use_stdio();
    SSD1306_Init(SSD1306_ADDR);
    SSD1306_ClearScreen();
    SSD1306_UpdateScreen(SSD1306_ADDR);
    Timer2_init_systicks();
    Buttons_init();
}

int main() { 
    init_all();

    main_menu();
    return 0;
}