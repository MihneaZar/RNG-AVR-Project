#include <avr/interrupt.h>
#include <avr/io.h>
// #include <stdio.h>

// #include "usart.h"
#include "timer.h"
#include "ssd1306.h"
#include "buttons.h"
#include "pff.h"
#include "print_to_lcd.h"

FATFS fs;  // sistemul de fisiere

/* No. of seconds before going into standby mode */
#define STANDBY 60

void copy_string_values(volatile char *string, char *copy_string) {
    uint8_t i = 0;
    while (string[i]) {
        copy_string[i] = string[i];
        i++;
    }
    string[i] = '\0';
}

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
    // USART0_init();
    // USART0_use_stdio();
    SSD1306_Init(SSD1306_ADDR);
    SSD1306_ClearScreen();
    SSD1306_UpdateScreen(SSD1306_ADDR);
    Timer2_init_systicks();
    while (!SYSTICKS_PASSED(0, 2000));
    Buttons_init();
    Set_time();
}

uint32_t random(uint32_t rand_min, uint32_t max_rand) {
    return rand_min + (systicks * systicks + red_count() * blue_count()) % (max_rand - rand_min + 1);
}

int main() { 
    init_all();

    DDRB |= (1 << PB5);
    PORTB &= ~(1 << PB5);

    uint32_t runtime_ping = -2001;
    uint32_t time_ping = -1001;
    uint8_t show_runtime = 0;

    char options[LCD_LINES][LCD_LINE_LEN] = {"time   ", "runtime", "rand  ", "set rand"};
    uint8_t option = 0;
    uint8_t max_option = 3;
    print_options_to_lcd(options, option, max_option);

    uint32_t min_rand = 0;
    uint32_t max_rand = 255;

    while(1) {
        if (blue_button && !show_runtime) {
            blue_button = 0;
            option = NEXT_OPTION(option, max_option);
            print_options_to_lcd(options, option, max_option);
        }
        if (red_button && !show_runtime) {
            red_button = 0;

            // change time format
            if (option == 0) {
                if (time_format == 24) {
                    time_format = 12;
                } else {
                    time_format = 24;
                }  
                print_time_to_lcd();
            }

            // show runtime
            if (option == 1) {
                print_runtime_to_lcd('p');
                runtime_ping = systicks;
                show_runtime = 1;
            }

            // print on the screen a random value
            if (option == 2) {
                print_rand_to_lcd(random(min_rand, max_rand));
            }

            // set minimum and maximum random value
            if (option == 3) {
                min_rand = get_value(" Set min rand val:");
                max_rand = get_value(" Set max rand val:");
                print_time_to_lcd();
                print_options_to_lcd(options, option, max_option);
            }
        }
        if (SYSTICKS_PASSED(runtime_ping, 2000) && show_runtime) {
            print_runtime_to_lcd('c');
            show_runtime = 0;
            print_options_to_lcd(options, option, max_option);
        }
        if (SYSTICKS_PASSED(time_ping, 1000)) {
            if (!SYSTICKS_PASSED(runtime_ping, 2000)) {
                print_runtime_to_lcd('u');
            }
            print_time_to_lcd();
            time_ping = systicks;
            if (PORTB & (1 << PB5)) {
                PORTB &= ~(1 << PB5);
            } else {
                PORTB |= (1 << PB5);
            }
        }
    }
    return 0;
}