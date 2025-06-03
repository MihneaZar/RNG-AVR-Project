#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdio.h>

#include "usart.h"
#include "menus.h"
#include "timer.h"
#include "ssd1306.h"
#include "buttons.h"
// #include "pff.h"

// FATFS fs;  // sistemul de fisiere

/* No. of seconds before going into standby mode */
#define STANDBY 60

/***
 * Disables all pull-ups for pins for power-saving.
 * 
 */
void unused_pins() {
    DDRB = 0;
    DDRC = 0;
    DDRD = 0;
    PORTB = 0;
    PORTC = 0;
    PORTD = 0;
}

void init_all() {
    sei();
    unused_pins();
    USART0_init();
    USART0_use_stdio();
    SSD1306_Init(SSD1306_ADDR);
    SSD1306_ClearScreen();
    SSD1306_UpdateScreen(SSD1306_ADDR);
    Timer2_init_systicks();
    Buttons_init();
}

int main() { 
    init_all();

    DDRB |= (1 << PB5);
    PORTB &= ~(1 << PB5);

    main_menu();

    return 0;
}