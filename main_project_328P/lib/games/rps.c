#include "rps.h"
#include <stdio.h>

const char *choice_name(uint8_t choice) {
    switch (choice) {
        case ROCK: return "rock";
        case PAPER: return "paper";
        case SCISSORS: return "scissors"; 
    }
    return "";
}

/*** 
 * Returns what is beaten by given choice.
 * 
*/
uint8_t is_beaten_by(uint8_t choice) {
    switch (choice) {
        case ROCK: return PAPER;
        case PAPER: return SCISSORS;
        case SCISSORS: return ROCK;
    }
    return 255; // should never get here
}

/*** 
 * Returns the winner of rock-paper-scissors.
 * (0 = draw, 1 = player1, 2 = player2)
 *
*/
uint8_t rps_winner(uint8_t choice1, uint8_t choice2) {
    if (choice1 == choice2) { 
        return 0;
    }
    if (is_beaten_by(choice2) == choice1) {
        return 1;
    } else {
        return 2;
    }
}

/***
 * Calculates the bot's rock-paper-scissors choice.
 * 
 * @param diff difficulty type:
 *              - r = random choice
 *              - d = defensive (response to its own choice)
 *              - a = aggresive (response to its own choice or player's choice)
 *  
 */
uint8_t bot_turn(char diff, uint8_t last_bot_choice, uint8_t last_player_choice) {
    // orders the three options randomly
    uint8_t options[THREE_OPTIONS] = {0, 0, 0};

    // weights for random bot strategy
    uint8_t weights[THREE_OPTIONS] = {33, 33, 33};

    while (options[0] == options[1]) {
        options[0] = random(0, 2);
        options[1] = random(0, 2);
    }
    options[2] = 3 - options[0] - options[1];

    // in defensive mode, the bot assumes the player will choose
    // the option that beats its last choice, therefore it 
    // chooses what beats that
    if (diff == 'd') {
        uint8_t counter_choice = is_beaten_by(last_bot_choice);
        for (int i = 0; i < THREE_OPTIONS; i++) {
            if (options[i] == counter_choice) {
                weights[i] = 60;
            } else {
                weights[i] = 20;
            }
        }
    }

    // the most complicated one: it prefers the choices that beat
    // both what beats the player's last choice, and what beats
    // the bot's last choice (which can beat one another)
    if (diff == 'a') {
        
        // if both the player and the bot have chosen 
        // the same thing last time, it treats it as 
        // defensive (60, 20, 20)
        if (last_bot_choice == last_player_choice) {
            return bot_turn('d', last_bot_choice, last_player_choice);
        }
        
        uint8_t counter_bot_choice = is_beaten_by(last_bot_choice);
        uint8_t counter_player_choice = is_beaten_by(last_player_choice);
        for (int i = 0; i < THREE_OPTIONS; i++) {
            if (options[i] == counter_bot_choice || options[i] == counter_player_choice) {
                weights[i] = 40;
            } else {
                weights[i] = 20;
            }
        }
    }

    return bot_chooses(options, weights, THREE_OPTIONS);
}

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
    uint8_t player_score = 0;
    uint8_t bot_score = 0;
    
    uint8_t last_player_choice = random(0, 2);
    uint8_t last_bot_choice = random(0, 2);

    uint8_t player_choice = 0;

    while (1) {
        clear_lcd_screen();

        print_line_to_lcd(0, "Score");
        print_score(1, "Player", player_score);
        print_score(2, "  Bot", bot_score);

        wait_for_input();

        print_line_to_lcd(0, "RPS");

        char *play_options[] = {"rock", "paper", "scissors", "return"};
        char player_text[] = "Player:";
        char bot_text[LCD_LINE_LEN + 1] = "Bot: ";

        print_line_to_lcd(1, player_text);
        player_choice = menu_interface(play_options, player_choice, FOUR_OPTIONS);
        clear_lcd_line(1);

        if (player_choice == 3) {
            break;
        }

        uint8_t bot_choice = bot_turn(diff, last_bot_choice, last_player_choice);
        last_player_choice = player_choice;
        last_bot_choice = bot_choice;

        append_chars(bot_text, choice_name(bot_choice));

        clear_lcd_line(2);
        clear_lcd_line(3);
        print_line_to_lcd(1, bot_text);
        wait_for_input();
        clear_lcd_line(1);

        if (bot_choice == 3) {
            break;
        }

        uint8_t winner = rps_winner(player_choice, bot_choice);
        // uint8_t winner = 0;
        clear_lcd_line(2);
        clear_lcd_line(3);
        if (winner == 0) {
            print_line_to_lcd(1, "It's a draw!");
        }
        if (winner == 1) {
            player_score++;
            print_line_to_lcd(1, "The player wins!");
        } 
        if (winner == 2) {
            bot_score++;
            print_line_to_lcd(1, "The bot wins!");
        }

        wait_for_input();
    }

    clear_lcd_screen();

    print_line_to_lcd(0, "Score");
    print_score(1, "Player", player_score);
    print_score(2, "  Bot", bot_score);

    wait_for_input();

    clear_lcd_screen();
}

void rps_pvp() {
    uint8_t player1_score = 0;
    uint8_t player2_score = 0;
    while (1) {
        clear_lcd_screen();

        print_line_to_lcd(0, "Score");
        print_score(1, "Player one", player1_score);
        print_score(2, "Player two", player2_score);

        wait_for_input();

        print_line_to_lcd(0, "RPS");

        char *play_options[] = {"rock", "paper", "scissors", "return"};
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

        uint8_t winner = rps_winner(player1_choice, player2_choice);
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