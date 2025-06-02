#include "menus.h"

/***
 * Returns random value between given min and max value.
 * 
 */
uint32_t random(uint32_t min_rand, uint32_t max_rand) {
    if (min_rand > max_rand) {
        return 0;
    }
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
            
            // check if option is blank string, 
            // skip to previous one if it is
            if (options[option][0] == '\0') { 
                option = PREV_OPTION(option, no_of_options);
            }
            print_options_to_lcd(options, option, no_of_options);
        }
        if (red_button) {
            red_button = 0;
            return option;
        }
        if (right_blue_button) {
            right_blue_button = 0;
            option = NEXT_OPTION(option, no_of_options);
            // check if option is blank string, 
            // skip to next one if it is
            if (options[option][0] == '\0') { 
                option = NEXT_OPTION(option, no_of_options);
            }
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
    clear_lcd_line(1);
    
    char *options[] = {"games", "runtime", "rand", "set rand"};
    uint8_t option = 0;

    uint32_t min_rand = 0;
    uint32_t max_rand = 255;

    while(1) {
        option = menu_interface(options, option, FOUR_OPTIONS);

        // games or no games
        if (option == 0) {
            clear_lcd_line(1);
            game_menu();
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
            print_options_to_lcd(options, option, FOUR_OPTIONS);
        }
    }
}

/***
 * The function for the game select menu.
 * 
 */
void game_menu() {
    char *game_options[] = {"rps", "xo", "blackjack", "return"};
    char game_text[] = "Choose a game:";
    uint8_t game_option = 0;

    char *versus_options[] = {"bot", "pvp", "", "return"};
    char versus_text[] = "Choose gamemode:";
    uint8_t versus_option = 0;

    char versus_modes[] = {'b', 'p'};

    while(1) {
        print_line_to_lcd(1, game_text);
        game_option = menu_interface(game_options, game_option, FOUR_OPTIONS);
        clear_lcd_line(1);
        if (game_option == 3) {
            return;
        }
        print_line_to_lcd(1, versus_text);
        versus_option = menu_interface(versus_options, versus_option, FOUR_OPTIONS);
        clear_lcd_line(1);
        if (versus_option == 0 || versus_option == 1) {
            switch (game_option) {
                case 0: rps(versus_modes[versus_option]);
                    break;
                case 1: xo(versus_modes[versus_option]);
                    break;
                case 2: blackjack(versus_modes[versus_option]);
                    break;
            }
        }
    }
}