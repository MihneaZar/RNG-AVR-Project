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
    DDRA = 0;
    DDRB = 0;
    DDRC = 0;
    DDRD = 0;
    DDRE = 0;
    DDRF = 0;
    DDRG = 0;
    DDRH = 0;
    DDRJ = 0;
    DDRK = 0;
    DDRL = 0;

    PORTA = 0;
    PORTB = 0;
    PORTC = 0;
    PORTD = 0;
    PORTE = 0;
    PORTF = 0;
    PORTG = 0;
    PORTH = 0;
    PORTJ = 0;
    PORTK = 0;
    PORTL = 0;

    DDRB |= (1 << PB7);
    PORTB &= ~(1 << PB7);
}

void init_all() {
    sei();
    set_pins();
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

    main_menu();

    return 0;
}