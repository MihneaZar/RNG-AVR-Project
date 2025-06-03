#ifndef BLACKJACK_H
#define BLACKJACK_H

#include "games.h"

// the theoretical maximum amount of cards you can draw
// (1 + 1 + 2 + 2 + 3 + 3 + 4 + 4 + 5) = 9
// if you would get any more, the other player would get blackjack or bust
#define MAX_CARDS 9
#define NO_OF_CARDS 4
#define CARD_TYPES 13
#define BASIC_SPLIT 14

// the lower value for A 
#define ALT_A 1
// the difference between treating A as an 11 and a 1 
#define DIF_A 10

static const uint8_t bj_value[] PROGMEM = {0, 11, 2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10, 10};

void bj_name(uint8_t card_no, char *card_name);
uint8_t bj_hit();
uint8_t player1_hit();
uint8_t player2_hit();
void start_animation(char mode);
void blackjack_pvbot();
void blackjack_pvp();
void blackjack(char mode);

#endif // BLACKJACK_H