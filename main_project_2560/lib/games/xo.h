#ifndef XO_H
#define XO_H

#include "games.h"

// player1 is X, player2 is O
#define EMPTY 0 
#define X 1
#define O 2
#define SELECT 3

#define XO_CELL_SIZE 3
#define XO_SIZE 3

#define XO_CORNER_X 4 * CHAR_LENGTH
#define XO_CORNER_Y CHAR_HEIGHT
#define XO_CORNER_X_CHAR 4
#define XO_CORNER_Y_CHAR 1

#define WIN_CONDITION 3

#define IS_SELECT 1
#define IS_NOT_SELECT 0

#define SHOW_WINNER 1
#define DONT_SHOW_WINNER 0

#define XO_INFINITE_MOVES 6

void xo(char mode);

#endif // XO_H