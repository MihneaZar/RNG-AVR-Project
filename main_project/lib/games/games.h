#ifndef GAMES_H
#define GAMES_H

#include <stdint.h>

#include "rps.h"
#include "xo.h"
#include "blackjack.h"

#include "buttons.h"
#include "print_to_lcd.h"
#include "menus.h"

uint8_t bot_chooses(uint8_t options[], uint8_t weights[], uint8_t no_of_options);
void print_score(uint8_t line, char player_name[], uint8_t score);
void wait_for_input();

#endif // GAMES_H