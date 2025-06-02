#include "blackjack.h"

void shuffle_animation() {
    uint32_t animation_ping;
    clear_lcd_screen();
    print_line_to_lcd(0, "Shuffling cards.  ");
    animation_ping = systicks;
    while (!SYSTICKS_PASSED(animation_ping, 750));
    print_line_to_lcd(0, "Shuffling cards.. ");
    animation_ping = systicks;
    while (!SYSTICKS_PASSED(animation_ping, 750));
    print_line_to_lcd(0, "Shuffling cards...");
    animation_ping = systicks;
    while (!SYSTICKS_PASSED(animation_ping, 750));
    clear_lcd_screen();
}

void blackjack_pvp() {
    uint8_t player1_score = 0;
    uint8_t player2_score = 0;
    while (1) {
        shuffle_animation();

        print_line_to_lcd(0, "Score");
        print_score(1, "Player one", player1_score);
        print_score(2, "Player two", player2_score);

        wait_for_input();

        print_line_to_lcd(0, "Blackjack");

        char *play_options[] = {"Hit me!", "Stand.", "", "return"};
        char player1_text[] = "Player one:";
        char player2_text[] = "Player two:";

        print_line_to_lcd(1, player1_text);
        uint8_t player1_choice = menu_interface(play_options, 0, FOUR_OPTIONS);
        clear_lcd_line(1);

        if (player1_choice == 3) {
            break;
        }

        print_line_to_lcd(1, player2_text);
        uint8_t player2_choice = menu_interface(play_options, 0, FOUR_OPTIONS);
        clear_lcd_line(1);

        if (player2_choice == 3) {
            break;
        }

        uint8_t winner = rps_winner[player1_choice][player2_choice];
        clear_lcd_line(2);
        clear_lcd_line(3);
        if (winner == 0) {
            print_line_to_lcd(1, "It's a draw!");
        }
        if (winner == 1) {
            player1_score++;
            print_line_to_lcd(1, "Player one wins!");
        } 
        if (winner == 2) {
            player2_score++;
            print_line_to_lcd(1, "Player two wins!");
        }

        wait_for_input();
    }
    clear_lcd_screen();

    print_line_to_lcd(0, "Score");
    print_score(1, "Player one", player1_score);
    print_score(2, "Player two", player2_score);

    wait_for_input();

    clear_lcd_screen();
}

/***
 * Main function for blackjack game.
 * 
 * @param mode b for pvbot, p for pvp
 * 
 */
void blackjack(char mode) {
    if (mode == 'b') {
    
    }
    
    if (mode == 'p') {
        blackjack_pvp();
    }
}