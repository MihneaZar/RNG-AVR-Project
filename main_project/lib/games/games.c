#include "games.h"

/***
 * Choosing move of bot based on weighted probabilities
 * 
 * @param options       the order of the options (0 to no_of_options - 1)
 * @param weights       the relative chance of choosing this option
 * @param no_of_options how many options the bot has
 * 
 * @return the choice of the bot
 * 
 */
uint8_t bot_chooses(uint8_t options[], uint8_t weights[], uint8_t no_of_options) {
    uint8_t total_weight = 0;
    for (uint8_t i = 0; i < no_of_options; i++) {
        total_weight += weights[i];
    }

    uint8_t rand_choice = random(1, total_weight);
    uint8_t last_percent = 0;
    for (uint8_t i = 0; i < no_of_options; i++) {
        if (last_percent < rand_choice && rand_choice <= last_percent + weights[options[i]]) {
            return i;
        }
        last_percent += weights[options[i]];
    }

    return 255; // should never get here
}

void print_score(uint8_t line, char player_name[], uint8_t score) {
    char score_line[LCD_LINE_LEN + 1];
    uint8_t position = 0;
    while(player_name[position] != '\0') {
        score_line[position] = player_name[position];
        position++;
    }
    score_line[position++] = ':';
    score_line[position++] = ' ';
    uint8_t pow10 = 100;
    uint8_t non_zero = 0;
    while (pow10 > 0) {
        if (non_zero || score / pow10 > 0) {
            non_zero = 1;
            score_line[position++] = '0' + score / pow10;
        } else {
            score_line[position++] = ' ';
        }
        score = score % pow10;
        pow10 /= 10;
    }
    if (!non_zero) {
        score_line[--position] = '0';
        position++;
    }
    score_line[position] = '\0';
    print_line_to_lcd(line, score_line);
}

void wait_for_input() {
    while (!left_blue_button && !red_button && !right_blue_button);
    left_blue_button = 0;
    red_button = 0;
    right_blue_button = 0;
}