#include "rps.h"

/***
 * Function for playing rock-paper-scissors against a bot.
 * 
 * @param diff difficulty type:
 *              - r = random choice
 *              - d = defensive (response to its own choice)
 *              - a = aggresive (response to its own choice or player's choice)
 *  
 */
void rps_pvbot(char diff) {

}

void rps_pvp() {
    
}

/***
 * Main function for rock-paper-scissors game.
 * 
 * @param mode b for pvbot, p for pvp
 * 
 */
void rps(char mode) {
    if (mode == 'b') {
        char *diff_options[] = {"random", "defense", "offense", "return"};
        char diff_text[] = "Choose a bot type:";
        uint8_t diff_option = 0;

        char diff_modes[] = {'r', 'd', 'a'};
        
        print_line_to_lcd(1, diff_text);
        diff_option = menu_interface(diff_options, diff_option, FOUR_OPTIONS);
        clear_lcd_line(1);
        if (diff_option == 3) {
            return;
        } 

        rps_pvbot(diff_modes[diff_option]);
    }
    if (mode == 'p') {
        rps_pvp();
    }
}