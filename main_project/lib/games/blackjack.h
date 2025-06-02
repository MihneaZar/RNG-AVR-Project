#ifndef BLACKJACK_H
#define BLACKJACK_H

#include "games.h"

#define NO_OF_CARDS 4
#define NO_OF_VALUES 15
#define BASIC_SPLIT 14

void shuffle_animation();
void blackjack_pvp();
void blackjack(char mode);

#endif // BLACKJACK_H