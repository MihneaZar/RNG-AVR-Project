#ifndef BLACKJACK_H
#define BLACKJACK_H

#include "games.h"

// this helps figure out overflow for card showing:
// char test[] = "P1:  1  2  3  4  5";
// char test[] = "P1: 1 1 2 2 3 3 4 4 5";

// the theoretical maximum amount of cards you can draw
// (1 + 1 + 2 + 2 + 3 + 3 + 4 + 4 + 5) = 9
// if you would get any more, the other player would get blackjack or bust
#define MAX_CARDS 9
#define NO_OF_CARDS 4
#define CARD_TYPES 13
#define BLACKJACK 21

#define ACE 1
// max value where ace can still be 11
#define ACE_SPLIT 10
// the lower value for A 
#define ALT_ACE 1
// the difference between treating ace as an 11 and a 1 
#define DIF_ACE 10

// max value where the basic split algorithm still draws
#define BASIC_SPLIT 14

void blackjack(char mode);

#endif // BLACKJACK_H