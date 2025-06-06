#include "blackjack.h"

const char *blackjack_name(uint8_t card_no) {
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
uint8_t blackjack_value(uint8_t card_no) {

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

uint8_t blackjack_hit(uint8_t bj_freq[CARD_TYPES + 1]) {
    uint8_t card_no = random(1, CARD_TYPES);
    while (bj_freq[card_no] == 0) {
        card_no = random(1, CARD_TYPES);
    }
    bj_freq[card_no]--;
    return card_no;
}

uint8_t player_hit(uint8_t player_cards[], uint8_t *player_card_no, 
                uint8_t *player_aces, uint8_t *player_sum, uint8_t next_card) {

    uint8_t card_value = blackjack_value(next_card);
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
        *player_sum -= DIF_ACE;
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
void blackjack_start_animation(char mode, uint8_t bj_freq[CARD_TYPES + 1], uint8_t first_four_hits[4]) {
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

    uint8_t player1_first_hit = blackjack_hit(bj_freq);
    wait_between_hits = systicks;
    while(!SYSTICKS_PASSED(wait_between_hits, left_blue_count() % 100 + 1));
    uint8_t player1_second_hit = blackjack_hit(bj_freq);
    char print_player1_line[LCD_LINE_LEN + 1] = "";
    if (mode == 'p') {
        append_chars(print_player1_line, "Player one: ");
    } 
    if (mode == 'b') {
        append_chars(print_player1_line, "Player: ");
    }

    append_chars(print_player1_line, blackjack_name(player1_first_hit));
    append_chars(print_player1_line, ", ");
    append_chars(print_player1_line, blackjack_name(player1_second_hit));

    print_line_to_lcd(1, print_player1_line);

    animation_ping = systicks;
    while (!SYSTICKS_PASSED(animation_ping, 750));

    wait_between_hits = systicks;
    while(!SYSTICKS_PASSED(wait_between_hits, red_count() % 100 + 1));
    uint8_t player2_first_hit = blackjack_hit(bj_freq);
    wait_between_hits = systicks;
    while(!SYSTICKS_PASSED(wait_between_hits, right_blue_count() % 100 + 1));
    uint8_t player2_second_hit = blackjack_hit(bj_freq);

    char print_player2_line[LCD_LINE_LEN] = "";
    if (mode == 'p') {
        append_chars(print_player2_line, "Player two: ");
    } 
    if (mode == 'b') {
        append_chars(print_player2_line, "   Bot: ");
    }

    append_chars(print_player2_line, blackjack_name(player2_first_hit));
    append_chars(print_player2_line, ", ");
    append_chars(print_player2_line, blackjack_name(player2_second_hit));

    print_line_to_lcd(2, print_player2_line);

    wait_for_input();
    clear_lcd_screen();

    first_four_hits[0] = player1_first_hit;
    first_four_hits[1] = player1_second_hit;
    first_four_hits[2] = player2_first_hit;
    first_four_hits[3] = player2_second_hit;
}

/***
 * Prints on the screen both players' current hands.
 * 
 */
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
        const char *card_name = blackjack_name(player1_cards[i]);
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
        const char *card_name = blackjack_name(player2_cards[i]);
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
void print_blackjack_player_action(const char player[], uint8_t hit_or_stand) {
    char print_action_line[LCD_LINE_LEN + 1] = "";
    append_chars(print_action_line, player);
    if (hit_or_stand != 0) {
        append_chars(print_action_line, " hit: ");
        append_chars(print_action_line, blackjack_name(hit_or_stand));
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
uint8_t blackjack_bot_turn(char diff, uint8_t bot_sum, uint8_t bot_aces, uint8_t bj_freq[], int8_t sum_diff) {
    if (diff == 'r') {
        return random(0, 1);
    }

    // 14-split algorithm is simple: if it's 14 or lower, hit
    // if it's higher, stand (easy to guess what it will do) 
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

        // if player has higher sum, there is no reason to stand
        // (the player can just stand to win)
        // therefore, the bot might as well just draw, it either
        // goes over blackjack and loses (which it would have anyway)
        // or potentially gets a draw or even a win
        if (sum_diff < 0) {
            return 0;
        }

        // if player has lower sum, there is no reason to risk it
        if (sum_diff > 0) {
            return 1;
        }

        // if sum was 21, game would already be ended
        // therefore, all aces are good
        uint8_t good_hit = bj_freq[ACE];
        uint8_t bad_hit = 0;

        // if it has aces, it considers them as their alternate 1 value
        uint8_t alt_sum = bot_sum;
        for (int i = 0; i < bot_aces; i++) {
            alt_sum -= ALT_ACE;
        }

        for (int i = 2; i <= CARD_TYPES; i++) {
            if (alt_sum + blackjack_value(i) <= 21) {
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
 * Returns the status of the game (based on sums and wether both players stand).
 * 
 * @return uint8_t status:
 *                 - 0 = it's a draw (both players stood on less than 21)
 *                 - 1 = player1 won
 *                 - 2 = player2 won
 *                 - 3 = game hasn't ended
 * 
 */
uint8_t blackjack_status(uint8_t player1_sum, uint8_t player2_sum, uint8_t both_stand) {
    
    // this is for the INCREDIBLY rare case where both players 
    // are dealt a blackjack at the start of the round
    if (player1_sum == BLACKJACK && player2_sum == BLACKJACK) {
        return 0;
    }

    // because each player has a separate action, only one player
    // can hit blackjack or higher at a time (therefore can't be equal in that case)
    if (player1_sum == BLACKJACK || player2_sum > BLACKJACK) {
        return 1;
    }

    if (player2_sum == BLACKJACK || player1_sum > BLACKJACK) {
        return 2;
    }

    if (both_stand) {
        if (player1_sum == player2_sum) {
            return 0;
        }

        if (player1_sum > player2_sum) {
            return 1;
        }

        if (player2_sum > player1_sum) {
            return 2;
        }
    }

    return 3;
}

/***
 * Function for a round of blackjack.
 * 
 * @param mode b for pvbot, p for pvp
 * @param diff type of bot algorithm:
 *             'r' = random
 *             's' = 14-split 
 *             'c' = counter
 * 
 */
uint8_t blackjack_round(char mode, char diff, uint8_t *player1_sum, uint8_t *player2_sum) {
    char *player1_name = "";
    char *player2_name = "";

    if (mode == 'p') {
        player1_name = "Player one";
        player2_name = "Player two";
    } else {
        player1_name = "Player";
        player2_name = "The bot";
    }

    uint8_t bj_freq[CARD_TYPES + 1];

    uint8_t player1_choice = 0;
    uint8_t player1_cards[MAX_CARDS];
    uint8_t player1_card_no = 0;
    uint8_t player1_aces = 0;

    uint8_t player2_choice = 0;
    uint8_t player2_cards[MAX_CARDS];
    uint8_t player2_card_no = 0;
    uint8_t player2_aces = 0;

    uint8_t first_four_hits[4];
    blackjack_start_animation(mode, bj_freq, first_four_hits);
    player_hit(player1_cards, &player1_card_no, &player1_aces, player1_sum, first_four_hits[0]);
    player_hit(player1_cards, &player1_card_no, &player1_aces, player1_sum, first_four_hits[1]);
    player_hit(player2_cards, &player2_card_no, &player2_aces, player2_sum, first_four_hits[2]);
    player_hit(player2_cards, &player2_card_no, &player2_aces, player2_sum, first_four_hits[3]);

    // changes when a game end condition has been met
    uint8_t game_status = blackjack_status(*player1_sum, *player2_sum, 0);

    if (game_status != 3) {
        return game_status;
    }

    // beginning of actual round
    while (1) {
        char *play_options[] = {"Hit me!", "Stand.", "See cards", "return"};
        char player1_text[LCD_LINE_LEN + 1] = "";
        append_chars(player1_text, player1_name);
        append_chars(player1_text, " turn");

        print_line_to_lcd(0, player1_text);
        print_blackjack_sums(mode, *player1_sum, *player2_sum);
        player1_choice = menu_interface(play_options, player1_choice, FOUR_OPTIONS);
        
        // loop for see cards
        while (player1_choice == 2) {
            print_blackjack_cards(mode, player1_cards, player1_card_no, player2_cards, player2_card_no);

            print_line_to_lcd(0, player1_text);
            print_blackjack_sums(mode, *player1_sum, *player2_sum);
            player1_choice = menu_interface(play_options, player1_choice, FOUR_OPTIONS);
        }

        if (player1_choice == 3) {
            return player1_choice;
        }

        if (player1_choice == 0) {
            uint8_t player1_card = player_hit(player1_cards, &player1_card_no, &player1_aces, player1_sum, blackjack_hit(bj_freq));
            clear_lcd_line(1);
            print_blackjack_player_action(player1_name, player1_card);
            wait_for_input();
            clear_lcd_line(1);
        }

        game_status = blackjack_status(*player1_sum, *player2_sum, (player1_choice == 1 && player2_choice == 1));

        if (game_status != 3) {
            return game_status;
        }

        if (mode == 'p') {
            char player2_text[LCD_LINE_LEN + 1] = "";
            append_chars(player2_text, player2_name);
            append_chars(player2_text, " turn");

            print_line_to_lcd(0, player2_text);
            print_blackjack_sums(mode, *player1_sum, *player2_sum);
            player2_choice = menu_interface(play_options, player2_choice, FOUR_OPTIONS);
            
            // loop for see cards
            while (player2_choice == 2) {
                print_blackjack_cards(mode, player2_cards, player2_card_no, player2_cards, player2_card_no);

                print_line_to_lcd(0, player2_text);
                print_blackjack_sums(mode, *player1_sum, *player2_sum);
                player2_choice = menu_interface(play_options, player2_choice, FOUR_OPTIONS);
            }

            if (player2_choice == 3) {
                return player2_choice;
            }

        } else {
            player2_choice = blackjack_bot_turn(diff, *player2_sum, player2_aces, bj_freq, *player2_sum - *player1_sum);
        }

        if (player2_choice == 0) {
            uint8_t player2_card = player_hit(player2_cards, &player2_card_no, &player2_aces, player2_sum, blackjack_hit(bj_freq));
            clear_lcd_line(1);
            print_blackjack_player_action(player2_name, player2_card);
            wait_for_input();
            clear_lcd_line(1);
        }

        // print to screen that the bot stands
        if (mode == 'b' && player2_choice == 1) {
            clear_lcd_line(1);
            print_blackjack_player_action("Bot", 0);
            wait_for_input();
            clear_lcd_line(1);
        }

        game_status = blackjack_status(*player1_sum, *player2_sum, (player1_choice == 1 && player2_choice == 1));

        if (game_status != 3) {
            return game_status;
        }
    }
}

/***
 * Function for a game of blackjack, containiing multiple rounds.
 * 
 * @param mode b for pvbot, p for pvp
 * @param diff type of bot algorithm
 * 
 */
void play_blackjack(char mode, char diff) {
    uint8_t player1_score = 0;
    uint8_t player2_score = 0;

    char *player1_name = "";
    char *player2_name = "";

    if (mode == 'p') {
        player1_name = "Player one";
        player2_name = "Player two";
    } else {
        player1_name = "Player";
        player2_name = "   Bot";
    }

    while (1) {
        print_line_to_lcd(0, "Score");
        print_score(1, player1_name, player1_score);
        print_score(2, player2_name, player2_score);

        wait_for_input();
        clear_lcd_screen();

        uint8_t player1_sum = 0;
        uint8_t player2_sum = 0;

        uint8_t result = blackjack_round(mode, diff, &player1_sum, &player2_sum);
        clear_lcd_screen();

        if (result == 3) {
            break;
        }

        char player1_sum_line[] = "";
        char player2_sum_line[] = "";
        append_chars(player1_sum_line, player1_name);
        append_chars(player2_sum_line, player2_name);
        append_chars(player1_sum_line, " sum");
        append_chars(player2_sum_line, " sum");

        print_line_to_lcd(0, "Blackjack");
        print_score(1, player1_sum_line, player1_sum);
        print_score(2, player2_sum_line, player2_sum);

        wait_for_input();
        clear_lcd_screen();
            
        if (result == 0) {
            if (mode == 'p') {
                print_line_to_lcd(1, "Players are tied!");
            } else {
                print_line_to_lcd(1, "You are tied");
                print_line_to_lcd(2, " with the bot!");
            }
        }
            
        if (result == 1) {
            if (mode == 'p') {
                print_line_to_lcd(1, "Player one wins!");
            } else {
                print_line_to_lcd(1, "You beat the bot!");
            }
            player1_score++;
        }

        if (result == 2) {
            if (mode == 'p') {
                print_line_to_lcd(1, "Player two wins!");
            } else {
                print_line_to_lcd(1, "The bot beat you!");
            }
            player2_score++;
        }

        wait_for_input();

        clear_lcd_screen();

        char *quit_play_options[] = {"continue", "return"};
        print_line_to_lcd(0, "Keep playing?");
        uint8_t quit_playing = menu_interface(quit_play_options, 0, TWO_OPTIONS);

        if (quit_playing) {
            break;
        } else {
            clear_lcd_screen();
        }
    }

    clear_lcd_screen();

    print_line_to_lcd(0, "Final Score");
    print_score(1, player1_name, player1_score);
    print_score(2, player2_name, player2_score);

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
    char diff = ' ';

    if (mode == 'b') {
        char *diff_options[] = {"random", "14-split", "counter", "return"};
        char diff_text[] = "Choose a bot type:";
        uint8_t diff_option = 0;

        // r = random, s = 14-split, c = counter
        char diff_modes[] = {'r', 's', 'c'};
        
        print_line_to_lcd(1, diff_text);
        diff_option = menu_interface(diff_options, diff_option, FOUR_OPTIONS);
        if (diff_option == 3) {
            clear_lcd_screen();
            return;
        }

        diff = diff_modes[diff_option];
    }

    clear_lcd_screen();
    play_blackjack(mode, diff);
}