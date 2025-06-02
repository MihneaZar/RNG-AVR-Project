#include "blackjack.h"

const uint8_t bj_values[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 10, 10, 10};
const char *bj_names[] = {"", " A", " 2", " 3", " 4", " 5", " 6", " 7", " 8", " 9", "10", " A", " J", " Q", " K"};

uint8_t bj_freq[NO_OF_VALUES + 1];

uint8_t bj_hit() {
    uint8_t card_no = random(1, NO_OF_VALUES);
    while (bj_freq[card_no] == 0) {
        card_no = random(1, NO_OF_VALUES);
    }
    bj_freq[card_no]--;
    return card_no;
}

/***
 * Cute animation at the beginning of game, but also resets frequencies of cards back to four.
 * 
 */
void shuffle_animation() {
    for (int i = 1; i <= NO_OF_VALUES; i++) {
        bj_freq[i] = NO_OF_CARDS;
    }

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

        // uint8_t winner = rps_winner[player1_choice][player2_choice];
        // clear_lcd_line(2);
        // clear_lcd_line(3);
        // if (winner == 0) {
        //     print_line_to_lcd(1, "It's a draw!");
        // }
        // if (winner == 1) {
        //     player1_score++;
        //     print_line_to_lcd(1, "Player one wins!");
        // } 
        // if (winner == 2) {
        //     player2_score++;
        //     print_line_to_lcd(1, "Player two wins!");
        // }

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