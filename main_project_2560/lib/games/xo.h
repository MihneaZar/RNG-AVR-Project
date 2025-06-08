#ifndef XO_H
#define XO_H

#include "games.h"

// player1 is X, player2 is O
#define EMPTY 0 
#define X 1
#define O 2
#define SELECT 3

#define XO_STR_SIZE 3
#define SIZE_3X3 3
#define SIZE_4X4 4

#define CORNER_X_3x3 4 * CHAR_LENGTH
#define CORNER_Y_3x3 CHAR_HEIGHT
#define CORNER_X_CHAR_3x3 4
#define CORNER_Y_CHAR_3x3 1

#define CORNER_X_4x4 3 * CHAR_LENGTH
#define CORNER_Y_4x4 0
#define CORNER_X_CHAR_4x4 3
#define CORNER_Y_CHAR_4x4 0
#define WIN_CONDITION_3 3
#define WIN_CONDITION_4 4

#define IS_SELECT 1
#define IS_NOT_SELECT 0

#define SHOW_WINNER 1
#define DONT_SHOW_WINNER 0

void xo(char mode);

#endif // XO_H