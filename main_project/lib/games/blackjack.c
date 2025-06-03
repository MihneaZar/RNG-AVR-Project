#include "blackjack.h"

static const char bj_names[CARD_TYPES + 1][3] PROGMEM = {"", " A", " 2", " 3", " 4", " 5", " 6", " 7", " 8", 
                        " 9", "10", " J", " Q", " K"};

void bj_name(uint8_t card_no, char *card_name) {
    card_name[0] = (char) pgm_read_byte(&bj_names[card_no][0]);
    card_name[1] = (char) pgm_read_byte(&bj_names[card_no][1]);
    card_name[2] = '\0'; 
}

uint8_t bj_freq[CARD_TYPES + 1];

uint8_t bj_hit() {
    uint8_t card_no = random(1, CARD_TYPES);
    while (bj_freq[card_no] == 0) {
        card_no = random(1, CARD_TYPES);
    }
    bj_freq[card_no]--;
    return card_no;
}

// player 1 or player
uint8_t player1_cards[MAX_CARDS];
uint8_t player1_card_no = 0;
uint8_t player1_As = 0;
uint8_t player1_sum = 0;

uint8_t player1_hit() {
    uint8_t next_card = bj_hit();
    uint8_t card_value = bj_value[next_card];
    if (bj_value[next_card] == 11) {
        if (player1_sum > 10) {
            card_value = ALT_A;
        } else {
            player1_As++;
        }
    }

    player1_sum += card_value;
    while (player1_sum > 21 && player1_As > 0) {
        player1_As--;
        player1_sum -= DIF_A;
    }

    player1_cards[player1_card_no++] = next_card;

    return next_card;
}

// player 2 or bot
uint8_t player2_cards[MAX_CARDS];
uint8_t player2_card_no = 0;
uint8_t player2_As = 0;
uint8_t player2_sum = 0;

uint8_t player2_hit() {
    uint8_t next_card = bj_hit();
    uint8_t card_value = bj_value[next_card];
    if (bj_value[next_card] == 11) {
        if (player2_sum > 10) {
            card_value = ALT_A;
        } else {
            player2_As++;
        }
    }

    player2_sum += card_value;
    while (player2_sum > 21 && player2_As > 0) {
        player2_As--;
        player2_sum -= DIF_A;
    }

    player2_cards[player2_card_no++] = next_card;

    return next_card;
}

/***
 * Cute animation at the beginning of game, but also serves as the reset function.
 * All frequencies are set to 4, and players get their first two cards.
 * 
 * @param mode pvp (p) or pvbot (b)
 * 
 */
void start_animation(char mode) {
    for (int i = 1; i <= CARD_TYPES; i++) {
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

    uint32_t wait_between_hits;

    uint8_t player1_first_hit = player1_hit();
    wait_between_hits = systicks;
    while(!SYSTICKS_PASSED(wait_between_hits, 50));
    uint8_t player1_second_hit = player1_hit();
    char print_player1_line[LCD_LINE_LEN + 1] = "";
    if (mode == 'p') {
        append_chars(print_player1_line, "Player one: ");
    } 
    if (mode == 'b') {
        append_chars(print_player1_line, "Player: ");
    }

    char card_name[3] = "";
    bj_name(player1_first_hit, card_name);
    append_chars(print_player1_line, card_name);
    append_chars(print_player1_line, ", ");
    bj_name(player1_second_hit, card_name);
    append_chars(print_player1_line, card_name);

    print_line_to_lcd(1, print_player1_line);

    animation_ping = systicks;
    while (!SYSTICKS_PASSED(animation_ping, 750));

    uint8_t player2_first_hit = player2_hit();
    wait_between_hits = systicks;
    while(!SYSTICKS_PASSED(wait_between_hits, 50));
    uint8_t player2_second_hit = player2_hit();

    char print_player2_line[LCD_LINE_LEN] = "";
    if (mode == 'p') {
        append_chars(print_player2_line, "Player two: ");
    } 
    if (mode == 'b') {
        append_chars(print_player2_line, "   Bot: ");
    }

    bj_name(player2_first_hit, card_name);
    append_chars(print_player2_line, card_name);
    append_chars(print_player2_line, ", ");
    bj_name(player2_second_hit, card_name);
    append_chars(print_player2_line, card_name);

    print_line_to_lcd(2, print_player2_line);

    wait_for_input();
    clear_lcd_screen();
}

void blackjack_pvbot() {
    uint8_t player_score = 0;
    uint8_t bot_score = 0;
    while (1) {
        start_animation('b');

        print_line_to_lcd(0, "Score");
        print_score(1, "Player", player_score);
        print_score(2, "   Bot", bot_score);

    //     print_line_to_lcd(0, "Blackjack");

    //     char *play_options[] = {"Hit me!", "Stand.", "See cards", "return"};
    //     char player_text[] = "Player:";
    //     char bot_text[] = "   Bot:";

    //     print_line_to_lcd(1, player_text);
    //     uint8_t player1_choice = menu_interface(play_options, 0, FOUR_OPTIONS);
    //     clear_lcd_line(1);

    //     if (player1_choice == 3) {
    //         break;
    //     }

    //     // print_line_to_lcd(1, bot_text);
    //     // uint8_t player2_choice = menu_interface(play_options, 0, FOUR_OPTIONS);
    //     // clear_lcd_line(1);

    //     // if (player2_choice == 3) {
    //         // break;
    //     // }

    //     // uint8_t winner = rps_winner[player1_choice][player2_choice];
    //     // clear_lcd_line(2);
    //     // clear_lcd_line(3);
    //     // if (winner == 0) {
    //     //     print_line_to_lcd(1, "It's a draw!");
    //     // }
    //     // if (winner == 1) {
    //     //     player1_score++;
    //     //     print_line_to_lcd(1, "Player one wins!");
    //     // } 
    //     // if (winner == 2) {
    //     //     player2_score++;
    //     //     print_line_to_lcd(1, "Player two wins!");
    //     // }

        wait_for_input();
    }
    clear_lcd_screen();

    print_line_to_lcd(0, "Score");
    print_score(1, "Player", player_score);
    print_score(2, "   Bot", bot_score);

    wait_for_input();

    clear_lcd_screen();
}

void blackjack_pvp() {
    uint8_t player1_score = 0;
    uint8_t player2_score = 0;
    while (1) {
        start_animation('p');

        print_line_to_lcd(0, "Score");
        print_score(1, "Player one", player1_score);
        print_score(2, "Player two", player2_score);

        // print_line_to_lcd(0, "Blackjack");

        // char *play_options[] = {"Hit me!", "Stand.", "", "return"};
        // char player1_text[] = "Player one:";
        // char player2_text[] = "Player two:";

        // print_line_to_lcd(1, player1_text);
        // uint8_t player1_choice = menu_interface(play_options, 0, FOUR_OPTIONS);
        // clear_lcd_line(1);

        // if (player1_choice == 3) {
        //     break;
        // }

        // print_line_to_lcd(1, player2_text);
        // uint8_t player2_choice = menu_interface(play_options, 0, FOUR_OPTIONS);
        // clear_lcd_line(1);

        // if (player2_choice == 3) {
        //     break;
        // }

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
        start_animation('b');
        // blackjack_pvbot();
    }
    
    if (mode == 'p') {
        start_animation('p');
        // blackjack_pvp();
    }
}