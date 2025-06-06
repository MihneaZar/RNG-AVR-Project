#include "rps.h"
#include <stdio.h>

const char *rps_choice_name(uint8_t choice) {
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
uint8_t rps_is_beaten_by(uint8_t choice) {
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
    if (rps_is_beaten_by(choice2) == choice1) {
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
uint8_t rps_bot_turn(char diff, uint8_t last_bot_choice, uint8_t last_player_choice) {
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
        uint8_t counter_choice = rps_is_beaten_by(last_bot_choice);
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
            return rps_bot_turn('d', last_bot_choice, last_player_choice);
        }
        
        uint8_t counter_bot_choice = rps_is_beaten_by(last_bot_choice);
        uint8_t counter_player_choice = rps_is_beaten_by(last_player_choice);
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
 * Play a game of rock-paper-scissors against a bot or another human.
 * 
 * @param mode b for pvbot, p for pvp
 * @param diff difficulty type:
 *              - r = random choice
 *              - d = defensive (response to its own choice)
 *              - a = aggresive (response to its own choice or player's choice)
 * 
 */
void play_rps(char mode, char diff) {
    uint8_t player1_score = 0;
    uint8_t player2_score = 0;
    
    char *player1_name = ""; 
    char *player2_name = ""; 
    char *player2_win_line = "";

    if (mode == 'p') {
        player1_name =     "Player one";
        player2_name =     "Player two";
        player2_win_line = "Player two wins!";
    } else {
        player1_name =     "Player";
        player2_name =     "   Bot";
        player2_win_line = "Bot wins!";
    }
    
    // for the bot's more advanced choice algorithms
    uint8_t last_player_choice = random(0, 2);
    uint8_t last_bot_choice = random(0, 2);

    while (1) {
        clear_lcd_screen();

        print_line_to_lcd(0, "Score");
        print_score(1, player1_name, player1_score);
        print_score(2, player2_name, player2_score);

        wait_for_input();

        print_line_to_lcd(0, "RPS");

        char *play_options[] = {"rock", "paper", "scissors", "return"};

        char player1_turn_line[] = "";
        append_chars(player1_turn_line, player1_name);
        append_chars(player1_turn_line, ":");
        print_line_to_lcd(1, player1_turn_line);
        uint8_t player1_choice = menu_interface(play_options, 0, FOUR_OPTIONS);
        clear_lcd_line(1);

        if (player1_choice == 3) {
            break;
        }
        
        uint8_t player2_choice = 0;

        if (mode == 'p') {
            print_line_to_lcd(1, "Player two:");
            player2_choice = menu_interface(play_options, 0, FOUR_OPTIONS);
            clear_lcd_line(1);

            if (player2_choice == 3) {
                break;
            }

        } else {
            player2_choice = rps_bot_turn(diff, last_bot_choice, last_player_choice);
            last_player_choice = player1_choice;
            last_bot_choice = player2_choice;

            char bot_turn_line[] = "Bot: ";
            append_chars(bot_turn_line, rps_choice_name(player2_choice));

            clear_lcd_line(2);
            clear_lcd_line(3);
            print_line_to_lcd(1, bot_turn_line);
            wait_for_input();
            clear_lcd_line(1);
        }

        uint8_t winner = rps_winner(player1_choice, player2_choice);
        clear_lcd_line(2);
        clear_lcd_line(3);
        if (winner == 0) {
            print_line_to_lcd(1, "It's a draw!");
        }
        
        if (winner == 1) {
            player1_score++;
            char player1_win_text[LCD_LINE_LEN + 1] = "";
            append_chars(player1_win_text, player1_name);
            append_chars(player1_win_text, " wins!");
            print_line_to_lcd(1, player1_win_text);
        } 

        if (winner == 2) {
            player2_score++;
            print_line_to_lcd(1, player2_win_line);
        }

        wait_for_input();
    }
    clear_lcd_screen();

    print_line_to_lcd(0, "Final Score");
    print_score(1, player1_name, player1_score);
    print_score(2, player2_name, player2_score);

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
    
    // difficulty chosen for rps bot
    char diff = ' ';

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

        diff = diff_modes[diff_option];
    }

    play_rps(mode, diff);
}