#include "menus.h"

/***
 * Returns random value between given min and max value.
 * 
 */
uint32_t random(uint32_t min_rand, uint32_t max_rand) {
    uint32_t interval = max_rand - min_rand + 1;
    uint32_t sys_in_interv = systicks % interval;
    // printf("%ld\n%ld\n%ld\n%ld\n\n", sys_in_interv, left_blue_count() % interval, red_count() % interval, right_blue_count() % interval);
    return min_rand + (red_count() % interval * sys_in_interv + (left_blue_count() % interval) * (right_blue_count() % interval)) % interval;
}

/***
 * General interface for menus.
 * 
 * @param options the options that user can choose
 * @param option the current selected option
 * @param no_of_options the number of choosable options (min 2, max 4)
 * 
 * @return the number of the option chosen (between 0 and 3)
 * 
 */
uint8_t menu_interface(char *options[], uint8_t option, uint8_t no_of_options) {
    
    print_time_to_lcd();
    uint32_t time_ping = systicks;
    print_options_to_lcd(options, option, no_of_options);

    while(1) {
        if (left_blue_button) {
            left_blue_button = 0;
            option = PREV_OPTION(option, no_of_options);
            print_options_to_lcd(options, option, no_of_options);
        }
        if (red_button) {
            red_button = 0;

            return option;
        }
        if (right_blue_button) {
            right_blue_button = 0;
            option = NEXT_OPTION(option, no_of_options);
            print_options_to_lcd(options, option, no_of_options);
        }
        if (SYSTICKS_PASSED(time_ping, 1000)) {
            print_time_to_lcd();
            time_ping = systicks;
            if (PORTB & (1 << PB5)) {
                PORTB &= ~(1 << PB5);
            } else {
                PORTB |= (1 << PB5);
            }
        }
    }
}

/***
 * The function for the main screen of the application.
 * 
 */
void main_menu() {
    char *options[] = {"time", "runtime", "rand", "set rand"};
    uint8_t option = 0;
    uint8_t no_of_options = 4;

    uint32_t min_rand = 0;
    uint32_t max_rand = 255;

    while(1) {
        option = menu_interface(options, option, no_of_options);

        // games or no games
        if (option == 0) {

        }

        // show runtime
        if (option == 1) {
            print_time_to_lcd();
            uint32_t time_ping = systicks;
            print_runtime_to_lcd('p');
            uint32_t runtime_ping = systicks;
            while (!SYSTICKS_PASSED(runtime_ping, 2000)) {
                if (SYSTICKS_PASSED(time_ping, 1000)) {
                    print_time_to_lcd();
                    time_ping = systicks;
                    print_runtime_to_lcd('u');
                }
            }
            print_runtime_to_lcd('c');
            left_blue_button = 0;
            red_button = 0;
            right_blue_button = 0;
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
            print_options_to_lcd(options, option, no_of_options);
        }
    }
}

