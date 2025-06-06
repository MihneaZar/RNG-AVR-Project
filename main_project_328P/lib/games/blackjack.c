#include "blackjack.h"
#include <stdio.h>

const char *bj_name(uint8_t card_no) {
    switch (card_no) {
        case 1:  return " A";
        case 2:  return " 2";
        case 3:  return " 3";
        case 4:  return " 4";
        case 5:  return " 5";
        case 6:  return " 6";
        case 7:  return " 7";
        case 8:  return " 8";
        case 9:  return " 9";
        case 10: return "10";
        case 11: return " J";
        case 12: return " Q";
        case 13: return " K";
    }

    return ""; // should never get here
}

/***
 * Returns the blackjack value of the card.
 *
*/
uint8_t bj_value(uint8_t card_no) {

    // ace big value
    if (card_no == 1) { 
        return 11;
    }

    // cards between 2 and 10 have their number as their actual value;
    if (2 <= card_no && card_no <= 10) {
        return card_no;
    }

    // rest have value ten
    return 10;
}

uint8_t bj_hit(uint8_t bj_freq[CARD_TYPES + 1]) {
    uint8_t card_no = random(1, CARD_TYPES);
    while (bj_freq[card_no] == 0) {
        card_no = random(1, CARD_TYPES);
    }
    bj_freq[card_no]--;
    return card_no;
}

uint8_t player_hit(uint8_t player_cards[], uint8_t *player_card_no, 
                uint8_t *player_aces, uint8_t *player_sum, uint8_t next_card) {

    uint8_t card_value = bj_value(next_card);
    if (next_card == ACE) {
        if (*player_sum > ACE_SPLIT) {
            card_value = ALT_ACE;
        } else {
            (*player_aces)++;
        }
    }

    *player_sum += card_value;
    while (*player_sum > BLACKJACK && *player_aces > 0) {
        (*player_aces)--;
        player_sum -= DIF_ACE;
    }

    player_cards[(*player_card_no)++] = next_card;

    return next_card;
}

/***
 * Cute animation at the beginning of game, but also serves as the reset function.
 * All frequencies are set to 4, and players get their first two cards.
 * 
 * @param mode pvp (p) or pvbot (b)
 * 
 */
void start_animation(char mode, uint8_t bj_freq[CARD_TYPES + 1], uint8_t first_four_hits[4]) {
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

    uint8_t player1_first_hit = bj_hit(bj_freq);
    wait_between_hits = systicks;
    while(!SYSTICKS_PASSED(wait_between_hits, 50));
    uint8_t player1_second_hit = bj_hit(bj_freq);
    char print_player1_line[LCD_LINE_LEN + 1] = "";
    if (mode == 'p') {
        append_chars(print_player1_line, "Player one: ");
    } 
    if (mode == 'b') {
        append_chars(print_player1_line, "Player: ");
    }

    append_chars(print_player1_line, bj_name(player1_first_hit));
    append_chars(print_player1_line, ", ");
    append_chars(print_player1_line, bj_name(player1_second_hit));

    print_line_to_lcd(1, print_player1_line);

    animation_ping = systicks;
    while (!SYSTICKS_PASSED(animation_ping, 750));

    uint8_t player2_first_hit = bj_hit(bj_freq);
    wait_between_hits = systicks;
    while(!SYSTICKS_PASSED(wait_between_hits, 50));
    uint8_t player2_second_hit = bj_hit(bj_freq);

    char print_player2_line[LCD_LINE_LEN] = "";
    if (mode == 'p') {
        append_chars(print_player2_line, "Player two: ");
    } 
    if (mode == 'b') {
        append_chars(print_player2_line, "   Bot: ");
    }

    append_chars(print_player2_line, bj_name(player2_first_hit));
    append_chars(print_player2_line, ", ");
    append_chars(print_player2_line, bj_name(player2_second_hit));

    print_line_to_lcd(2, print_player2_line);

    wait_for_input();
    clear_lcd_screen();

    first_four_hits[0] = player1_first_hit;
    first_four_hits[1] = player1_second_hit;
    first_four_hits[2] = player2_first_hit;
    first_four_hits[3] = player2_second_hit;
}

void print_blackjack_cards(char mode, uint8_t player1_cards[], uint8_t player1_card_no, uint8_t player2_cards[], uint8_t player2_card_no) {
    clear_lcd_screen();

    print_line_to_lcd(0, "Player's cards:");

    uint8_t player1_line = 2;
    uint8_t player2_line = 3;
    uint8_t player1_spaces = 1;
    uint8_t player2_spaces = 1;

    // move up a line because the card number is too high
    if (player1_card_no > 8) {
        player1_line = 1;
    }
    if (player2_card_no > 8) {
        player1_line = 1;
        player2_line = 2;
    }

    // cut spaces if card number is too high
    if (player1_card_no > 5) {
        player1_spaces = 0;
    }
    if (player2_card_no > 5) {
        player2_spaces = 0;
    }

    char player1_card_line[2 * LCD_LINE_LEN];
    uint8_t position = 0;

    player1_card_line[position++] = 'P';
    if (mode == 'p') {
        player1_card_line[position++] = '1';
        player1_card_line[position++] = ':';
    } else {
        player1_card_line[position++] = ':';
    }

    for (int i = 0; i < player1_card_no; i++) {
        if (player1_spaces || player1_cards[i] == 10) {
            player1_card_line[position++] = ' ';
        }
        const char *card_name = bj_name(player1_cards[i]);
        player1_card_line[position++] = card_name[0];
        player1_card_line[position++] = card_name[1];
    }
    player1_card_line[position] = '\0';

    print_line_to_lcd(player1_line, player1_card_line);

    char player2_card_line[2 * LCD_LINE_LEN];
    position = 0;

    if (mode == 'p') {
        player2_card_line[position++] = 'P';
        player2_card_line[position++] = '2';
        player2_card_line[position++] = ':';
    } else {
        player2_card_line[position++] = 'B';
        player2_card_line[position++] = ':';
    }

    for (int i = 0; i < player2_card_no; i++) {
        if (player2_spaces || player2_cards[i] == 10) {
            player2_card_line[position++] = ' ';
        }
        const char *card_name = bj_name(player2_cards[i]);
        player2_card_line[position++] = card_name[0];
        player2_card_line[position++] = card_name[1];
    }
    player2_card_line[position] = '\0';

    print_line_to_lcd(player2_line, player2_card_line);

    wait_for_input();
    clear_lcd_screen();
    
}

/***
 * Prints the current blackjack sums of the two players.
 * 
 * @param mode pvp ('p') or pvbot ('b')
 * 
 * Line should look like:
 * "P1: 13      P2:  2"     for pvp
 * "P: 13        B:  2"     for pvbot
 */
void print_blackjack_sums(char mode, uint8_t player1_sum, uint8_t player2_sum) {
    char blackjack_sums_line[LCD_LINE_LEN + 1];
    uint8_t position = 0;

    blackjack_sums_line[position++] = 'P';

    if (mode == 'p') {
        blackjack_sums_line[position++] = '1';
        blackjack_sums_line[position++] = ':';
        blackjack_sums_line[position++] = ' ';
    } else {
        blackjack_sums_line[position++] = ':';
        blackjack_sums_line[position++] = ' ';
    }

    if (player1_sum >= 10) {
        blackjack_sums_line[position++] = '0' + player1_sum / 10;
    } else {
        blackjack_sums_line[position++] = ' ';
    }

    blackjack_sums_line[position++] = '0' + player1_sum % 10;

    if (mode == 'b') {
        blackjack_sums_line[position++] = ' ';
    }

    // six spaces in between player 1 and player 2 
    // eight between player and bot
    for (int i = 0; i < (LCD_LINE_LEN - 2 * 6); i++) {
        blackjack_sums_line[position++] = ' ';
    }

    if (mode == 'p') {
        blackjack_sums_line[position++] = 'P';
        blackjack_sums_line[position++] = '2';
    } else {
        blackjack_sums_line[position++] = ' ';
        blackjack_sums_line[position++] = 'B';
    }

    blackjack_sums_line[position++] = ':';
    blackjack_sums_line[position++] = ' ';

    if (player2_sum >= 10) {
        blackjack_sums_line[position++] = '0' + player2_sum / 10;
    } else {
        blackjack_sums_line[position++] = ' ';
    }

    blackjack_sums_line[position++] = '0' + player2_sum % 10;

    blackjack_sums_line[position] = '\0';

    print_line_to_lcd(1, blackjack_sums_line);
}

/***
 * Prints the the action of a player, or the bot.
 * 
 * @param player       name of the player
 * @param hit_or_stand 0 if stand, card number otherwise (stand only for bot)
 * 
 */
void print_player_action(const char player[], uint8_t hit_or_stand) {
    char print_action_line[LCD_LINE_LEN + 1] = "";
    append_chars(print_action_line, player);
    if (hit_or_stand != 0) {
        append_chars(print_action_line, " hit: ");
        append_chars(print_action_line, bj_name(hit_or_stand));
    } else {
        append_chars(print_action_line, " stands.");
    }
    print_line_to_lcd(1, print_action_line);
}

/***
 * Bot decides whether it hits or it stands.
 * 
 * @param diff difficulty chosen for bot (r = random, s = 14-split, c = counter)
 *             player_stands = 1 if player is standing this turn
 *             sum_diff = bot_sum - player_sum (basically if bot has a lower, equal, or higher sum)
 * 
 * // identical to player
 * @return uint8_t 0 = hits  
 *                 1 = stands 
 * 
 */
uint8_t bot_turn(char diff, uint8_t bot_sum, uint8_t bj_freq[], uint8_t player_stands, uint8_t sum_diff) {
    if (diff == 'r') {
        return random(0, 1);
    }

    // if player is holding and the bot has less or equal value,
    // it will hit to avoid losing/draw
    if (player_stands && sum_diff <= 0) {
        return 0;
    }

    if (diff == 's') {
        if (bot_sum <= BASIC_SPLIT) {
            return 0;
        } else {
            return 1;
        }
    }

    // this algorithm counts all remaining cards
    // if there at least the same amount of cards 
    // that will keep his sum under or equal to blackjack,
    // it hits; otherwise, it stands
    if (diff == 'c') {
        // if sum was 21, game would already be ended
        // therefore, all aces are good
        uint8_t good_hit = bj_freq[ACE];
        uint8_t bad_hit = 0;

        for (int i = 2; i <= CARD_TYPES; i++) {
            if (bot_sum + bj_value(i) <= 21) {
                good_hit += bj_freq[i];
            } else {
                bad_hit += bj_freq[i];
            }
        }

        if (good_hit >= bad_hit) {
            return 0;
        } else {
            return 1;
        }
    }

    return 255; // should never get here
}

/***
 * This is the primary function for player vs. bot.
 * 
 * @param diff type of bot algorithm:
 *             'r' = random
 *             's' = 14-split 
 *             'c' = counter
 * 
 * @return uint8_t draw, winner, or return-to-game-menu value
 *         0 = draw
 *         1 = player won
 *         2 = bot won
 *         3 = return to game menu
 * 
 */
uint8_t blackjack_pvbot(char diff, uint8_t *player_sum, uint8_t *bot_sum) {
    uint8_t bj_freq[CARD_TYPES + 1];

    uint8_t player_choice = 0;
    uint8_t player_cards[MAX_CARDS];
    uint8_t player_card_no = 0;
    uint8_t player_aces = 0;

    uint8_t bot_cards[MAX_CARDS];
    uint8_t bot_card_no = 0;
    uint8_t bot_aces = 0;

    uint8_t first_four_hits[4];
    start_animation('b', bj_freq, first_four_hits);
    player_hit(player_cards, &player_card_no, &player_aces, player_sum, first_four_hits[0]);
    player_hit(player_cards, &player_card_no, &player_aces, player_sum, first_four_hits[1]);
    player_hit(bot_cards, &bot_card_no, &bot_aces, bot_sum, first_four_hits[2]);
    player_hit(bot_cards, &bot_card_no, &bot_aces, bot_sum, first_four_hits[3]);

    // draw by default
    if (*player_sum == BLACKJACK && *bot_sum == BLACKJACK) {
        return 0;
    }

    // player won by default
    if (*player_sum == BLACKJACK) {
        return 1;
    }

    // bot won by default
    if (*bot_sum == BLACKJACK) {
        return 2;
    }

    while (1) {
        char *play_options[] = {"Hit me!", "Stand.", "See cards", "return"};
        char player_text[] = "Player's turn";

        print_line_to_lcd(0, player_text);
        print_blackjack_sums('b', *player_sum, *bot_sum);
        player_choice = menu_interface(play_options, player_choice, FOUR_OPTIONS);
        
        // loop for see cards
        while (player_choice == 2) {
            print_blackjack_cards('b', player_cards, player_card_no, bot_cards, bot_card_no);

            print_line_to_lcd(0, player_text);
            print_blackjack_sums('b', *player_sum, *bot_sum);
            player_choice = menu_interface(play_options, player_choice, FOUR_OPTIONS);
        }

        if (player_choice == 0) {
            uint8_t player_card = player_hit(player_cards, &player_card_no, &player_aces, player_sum, bj_hit(bj_freq));
            clear_lcd_line(1);
            print_player_action("Player", player_card);
            wait_for_input();
            clear_lcd_line(1);
            if (*player_sum > BLACKJACK) {
                return 2;
            }
        }
        
        uint8_t player_stands = 0;

        if (player_choice == 1) {
            player_stands = 1;
        }

        if (player_choice == 3) {
            clear_lcd_screen();
            return player_choice;
        }

        uint8_t bot_choice = bot_turn(diff, *bot_sum, bj_freq, player_stands, *bot_sum - *player_sum);
        
        // uint32_t wait_between_inputs = systicks;
        // while (!SYSTICKS_PASSED(wait_between_inputs, 500));

        if (bot_choice == 1) {
            uint8_t bot_card = player_hit(bot_cards, &bot_card_no, &bot_aces, bot_sum, bj_hit(bj_freq));
            clear_lcd_line(1);
            print_player_action("Bot", bot_card);
            wait_for_input();
            clear_lcd_line(1);
            if (*bot_sum > BLACKJACK) {
                return 1;
            }
        } else {
            clear_lcd_line(1);
            print_player_action("Bot", 0);
            wait_for_input();
            clear_lcd_line(1);
        }


    }
}

void blackjack_pvp() {
    uint8_t player1_score = 0;
    uint8_t player2_score = 0;
    while (1) {
        // start_animation('p');

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
        char *diff_options[] = {"random", "14-split", "counter", "return"};
        char diff_text[] = "Choose a bot type:";
        uint8_t diff_option = 0;

        // r = random, s = 14-split, c = counter
        char diff_modes[] = {'r', 's', 'c'};
        
        print_line_to_lcd(1, diff_text);
        diff_option = menu_interface(diff_options, diff_option, FOUR_OPTIONS);
        clear_lcd_screen();
        if (diff_option == 3) {
            return;
        }
        
        uint8_t player_score = 0;
        uint8_t bot_score = 0;

        uint8_t result = 0;

        while (1) {
            print_line_to_lcd(0, "Score");
            print_score(1, "Player", player_score);
            print_score(2, "   Bot", bot_score);

            wait_for_input();
            clear_lcd_screen();

            uint8_t player_sum = 0;
            uint8_t bot_sum = 0;

            result = blackjack_pvbot(diff_modes[diff_option], &player_sum, &bot_sum);
            clear_lcd_screen();

            if (result == 3) {
                break;
            }

            print_line_to_lcd(0, "Blackjack");
            print_score(2, "Player sum", player_sum);
            print_score(3, "   Bot sum", bot_sum);
            
            if (result == 0) {
                print_line_to_lcd(1, "You tied with the bot!");
            }
            
            if (result == 1) {
                print_line_to_lcd(1, "You beat the bot!");
                player_score++;
            }

            if (result == 2) {
                print_line_to_lcd(1, "The bot beat you!");
                bot_score++;
            }

            wait_for_input();

            clear_lcd_screen();
        }

        clear_lcd_screen();

        print_line_to_lcd(0, "Score");
        print_score(1, "Player", player_score);
        print_score(2, "   Bot", bot_score);

        wait_for_input();
        clear_lcd_screen();
    }
    
    if (mode == 'p') {
        // blackjack_pvp();
    }
}