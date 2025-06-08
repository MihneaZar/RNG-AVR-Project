#include "xo.h"

void print_xo_borders(uint8_t size) {
    uint8_t corner_x = CORNER_X_3x3;
    uint8_t corner_y = CORNER_Y_3x3;

    if (size == SIZE_4X4) {
        corner_x = CORNER_X_4x4;
        corner_y = CORNER_Y_4x4;
    }

    for (int i = 0; i < size + 1; i++) {
        SSD1306_DrawLine(corner_x, corner_x + 3 * size * CHAR_LENGTH, corner_y - (i > 0) + i * CHAR_HEIGHT, corner_y - (i > 0) + i * CHAR_HEIGHT);
        SSD1306_DrawLine(corner_x + i * 3 * CHAR_LENGTH, corner_x + i * 3 * CHAR_LENGTH, corner_y, corner_y + size * CHAR_HEIGHT - 1);
    }
    SSD1306_UpdateScreen(SSD1306_ADDR);
}

/***
 * Places character at given position, and repairs the grid cell.
 * 
 * @param position the position in the xo grid (0-8/0-15)
 * @param size size of grid (3/4)
 * @param is_select whether it's the > character for selection (or space to delete it)
 *                  this means it needs to be placed one position back
 * 
 */
void xo_place_char(char c, uint8_t position, uint8_t is_select, uint8_t size) {
    uint8_t corner_x = CORNER_X_CHAR_3x3;
    uint8_t corner_y = CORNER_Y_CHAR_3x3;

    uint8_t cell_corner_x = CORNER_X_3x3 + (position % size) * 3 * CHAR_LENGTH;
    uint8_t cell_corner_y = CORNER_Y_3x3 + (position / size) * CHAR_HEIGHT - 1; 

    if (size == SIZE_4X4) {
        corner_x = CORNER_X_CHAR_4x4;
        corner_y = CORNER_Y_CHAR_4x4;
        cell_corner_x = CORNER_X_4x4 + (position % size) * 3 * CHAR_LENGTH;
        cell_corner_y = CORNER_Y_4x4 + (position / size) * CHAR_HEIGHT - 1; 
    }

    // adds one to position X or O in middle of grid cell
    print_char_at(c, corner_x + (1 - is_select) + 3 * (position % size), corner_y + position / size);
    
    // rebuilding border
    SSD1306_DrawRectangle(cell_corner_x, cell_corner_x + 3 * CHAR_LENGTH, cell_corner_y + ((position / size) == 0), cell_corner_y + CHAR_HEIGHT);
    SSD1306_UpdateScreen(SSD1306_ADDR);
}

/***
 * Interface for player to choose where to place their X or O.
 * 
 */
uint8_t get_player_move_xo(uint8_t board[], uint8_t size) {
    uint8_t position = 0;

    // searches for first empty position
    while (board[position] != EMPTY) {
        position++;
        
        // if no position is available, return 255 (error)
        if (position == size * size) {
            return 255;
        }
    }

    board[position] = SELECT;
    xo_place_char('>', position, IS_SELECT, size);

    while (1) {
        // previous position
        if (left_blue_button) {
            left_blue_button = 0;
            board[position] = EMPTY;
            xo_place_char(' ', position, IS_SELECT, size);
            position = PREV_OPTION(position, size * size); 
            while (board[position] != EMPTY) {
                position = PREV_OPTION(position, size * size); 
            }
            board[position] = SELECT;
            xo_place_char('>', position, IS_SELECT, size);
        }

        // selects position
        if (red_button) {
            red_button = 0;
            break;
        }

        if (right_blue_button) {
            right_blue_button = 0;
            board[position] = EMPTY;
            xo_place_char(' ', position, IS_SELECT, size);
            position = NEXT_OPTION(position, size * size); 
            while (board[position] != EMPTY) {
                position = NEXT_OPTION(position, size * size); 
            }
            board[position] = SELECT;
            xo_place_char('>', position, IS_SELECT, size);
        }
    }
    xo_place_char(' ', position, IS_SELECT, size);

    return position;
}

/***
 * Function for a game of tic-tac-toe, containing multiple rounds.
 * 
 * @param mode b for pvbot, p for pvp
 * @param diff type of bot algorithm
 * 
 */
void xo_3x3_round(char mode, char diff) {
    print_xo_borders(SIZE_3X3);
    uint8_t xo_board[SIZE_3X3 * SIZE_3X3] = {EMPTY, EMPTY, EMPTY,   
                                             EMPTY, EMPTY, EMPTY,   
                                             EMPTY, EMPTY, EMPTY};

    while (1) {
        uint8_t player1_move = get_player_move_xo(xo_board, SIZE_3X3);
        if (player1_move == 255) {
            break;
        }
        xo_board[player1_move] = X;
        xo_place_char('X', player1_move, IS_NOT_SELECT, SIZE_3X3);
        uint8_t player2_move = get_player_move_xo(xo_board, SIZE_3X3);
        if (player2_move == 255) {
            break;
        }
        xo_board[player2_move] = O;
        xo_place_char('O', player2_move, IS_NOT_SELECT, SIZE_3X3);
    }
    wait_for_input();
    clear_lcd_screen();
}

void xo_4x4_round(char mode, char diff, uint8_t win_condition) {
    print_xo_borders(SIZE_4X4);
    uint8_t xo_board[SIZE_4X4 * SIZE_4X4] = {EMPTY, EMPTY, EMPTY, EMPTY,
                                             EMPTY, EMPTY, EMPTY, EMPTY,
                                             EMPTY, EMPTY, EMPTY, EMPTY,
                                             EMPTY, EMPTY, EMPTY, EMPTY};

    while (1) {
        uint8_t player1_move = get_player_move_xo(xo_board, SIZE_4X4);
        if (player1_move == 255) {
            break;
        }
        xo_board[player1_move] = X;
        xo_place_char('X', player1_move, IS_NOT_SELECT, SIZE_4X4);
        uint8_t player2_move = get_player_move_xo(xo_board, SIZE_4X4);
        if (player2_move == 255) {
            break;
        }
        xo_board[player2_move] = O;
        xo_place_char('O', player2_move, IS_NOT_SELECT, SIZE_4X4);
    }
    wait_for_input();
    clear_lcd_screen();
}

/***
 * Main function for tic-tac-toe game.
 * 
 * @param mode b for pvbot, p for pvp
 * 
 */
void xo(char mode) {

    // 3x3 = normal xo, 4x4 three = board is 4x4, with four in a row for win
    // 4x4 four = board is 4x4, but win condition is three in a row
    char *size_options[] = {"3x3", "4x4 easy", "4x4 hard", "return"};
    char size_text[] = "Choose game type:";
    uint8_t size_option = 0;
        
    print_line_to_lcd(1, size_text);
    size_option = menu_interface(size_options, size_option, FOUR_OPTIONS);

    if (size_option == 3) {
        clear_lcd_screen();
        return;
    }

    char diff = ' ';

    if (mode == 'b') {
        clear_lcd_line(1);
        char *diff_options[] = {"random", "blocker", "perfect", "return"};
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

    
    switch (size_option) {
        case 0: 
            xo_3x3_round(mode, diff);
            break;
        case 1:
            // xo_4x4_round(mode, diff, WIN_CONDITION_4);
            break;
        case 2:
            // xo_4x4_round(mode, diff, WIN_CONDITION_3);
            break;
    }
}