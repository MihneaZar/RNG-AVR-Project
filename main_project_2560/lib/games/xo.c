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
 * Returns the status of the game.
 * 
 * @param board the xo board
 * @param size size of board
 * @param win_condition no. of X/O's in a line to win
 * @param last_placement position of last player's move
 * (function needs to check only if the last placement 
 * has changed the status of the game)
 * 
 * @return uint8_t status:
 *                 - 0 = it's a draw (no player has won and all positions are covered)
 *                 - 1 = player1 won
 *                 - 2 = player2 won
 *                 - 3 = game hasn't ended
 * 
 */
uint8_t xo_status(uint8_t board[], uint8_t size, uint8_t win_condition, uint8_t last_placement) {
    uint8_t last_placed = board[last_placement];

    for (int i = 0; i < size * size; i++) {

        // game continues if there's at least one empty position 
        if (board[i] == EMPTY) {
            return GAME_STATUS_CONTINUE;
        }
    }

    return GAME_DRAW;
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
 * Plays a round of 3x3 tic-tac-toe, pvp or pvbot.
 * 
 * @param round_parity determines who goes first:
 *                     0 - player one / player goes first
 *                     1 - player two / bot goes first   
 * 
 * @return winner or draw:
 *         0 - draw
 *         1 - player one / player
 *         2 - player two / bot 
 * 
 */
uint8_t xo_3x3_round(char mode, char diff, uint8_t round_parity) {
    print_xo_borders(SIZE_3X3);
    uint8_t xo_board[SIZE_3X3 * SIZE_3X3] = {EMPTY, EMPTY, EMPTY,   
                                             EMPTY, EMPTY, EMPTY,   
                                             EMPTY, EMPTY, EMPTY};

    uint8_t game_status = GAME_STATUS_CONTINUE;

    uint8_t player1_move = 0;
    uint8_t player2_move = 0;

    while (1) {
        if (round_parity == 0) {
            player1_move = get_player_move_xo(xo_board, SIZE_3X3);
            xo_board[player1_move] = X;
            xo_place_char('X', player1_move, IS_NOT_SELECT, SIZE_3X3);
            
            game_status = xo_status(xo_board, SIZE_3X3, WIN_CONDITION_3, player1_move);
            if (game_status != GAME_STATUS_CONTINUE) {
                break;
            }

            if (mode == 'p') {
                player2_move = get_player_move_xo(xo_board, SIZE_3X3);
            } else {
                // player2_move = xo_bot_turn();
            }
            xo_board[player2_move] = O;
            xo_place_char('O', player2_move, IS_NOT_SELECT, SIZE_3X3);

            game_status = xo_status(xo_board, SIZE_3X3, WIN_CONDITION_3, player2_move);
            if (game_status != GAME_STATUS_CONTINUE) {
                break;
            }
        } else {
            if (mode == 'p') {
                player2_move = get_player_move_xo(xo_board, SIZE_3X3);
            } else {
                // player2_move = xo_bot_turn();
            }
            xo_place_char('X', player2_move, IS_NOT_SELECT, SIZE_3X3);
            
            game_status = xo_status(xo_board, SIZE_3X3, WIN_CONDITION_3, player2_move);
            if (game_status != GAME_STATUS_CONTINUE) {
                break;
            }

            player1_move = get_player_move_xo(xo_board, SIZE_3X3);
            xo_board[player1_move] = O;
            xo_place_char('O', player1_move, IS_NOT_SELECT, SIZE_3X3);
            
            game_status = xo_status(xo_board, SIZE_3X3, WIN_CONDITION_3, player1_move);
            if (game_status != GAME_STATUS_CONTINUE) {
                break;
            }

        }
    }
    
    wait_for_input();
    clear_lcd_screen();

    return game_status;
}

/***
 * Plays a round of 4x4 tic-tac-toe, pvp or pvbot.
 * 
 * @param win_condition number of X/O's to get in a row for win (3 or 4).
 * 
 * @return winner or draw:
 *         0 - draw
 *         1 - player one / player
 *         2 - player two / bot 
 * 
 */
uint8_t xo_4x4_round(char mode, char diff, uint8_t win_condition, uint8_t round_parity) {
    print_xo_borders(SIZE_4X4);
    uint8_t xo_board[SIZE_4X4 * SIZE_4X4] = {EMPTY, EMPTY, EMPTY, EMPTY,
                                             EMPTY, EMPTY, EMPTY, EMPTY,
                                             EMPTY, EMPTY, EMPTY, EMPTY,
                                             EMPTY, EMPTY, EMPTY, EMPTY};

    uint8_t game_status = GAME_STATUS_CONTINUE;

    uint8_t player1_move = 0;
    uint8_t player2_move = 0;

    while (1) {
        if (round_parity == 0) {
            player1_move = get_player_move_xo(xo_board, SIZE_4X4);
            xo_board[player1_move] = X;
            xo_place_char('X', player1_move, IS_NOT_SELECT, SIZE_4X4);
            
            game_status = xo_status(xo_board, SIZE_4X4, win_condition, player1_move);
            if (game_status != GAME_STATUS_CONTINUE) {
                break;
            }

            if (mode == 'p') {
                player2_move = get_player_move_xo(xo_board, SIZE_4X4);
            } else {
                // player2_move = xo_bot_turn();
            }
            xo_board[player2_move] = O;
            xo_place_char('O', player2_move, IS_NOT_SELECT, SIZE_4X4);

            game_status = xo_status(xo_board, SIZE_4X4, win_condition, player2_move);
            if (game_status != GAME_STATUS_CONTINUE) {
                break;
            }
        } else {
            if (mode == 'p') {
                player2_move = get_player_move_xo(xo_board, SIZE_4X4);
            } else {
                // player2_move = xo_bot_turn();
            }
            xo_board[player2_move] = O;
            xo_place_char('O', player2_move, IS_NOT_SELECT, SIZE_4X4);
            
            game_status = xo_status(xo_board, SIZE_4X4, win_condition, player2_move);
            if (game_status != GAME_STATUS_CONTINUE) {
                break;
            }

            player1_move = get_player_move_xo(xo_board, SIZE_4X4);
            xo_board[player1_move] = X;
            xo_place_char('X', player1_move, IS_NOT_SELECT, SIZE_4X4);
            
            game_status = xo_status(xo_board, SIZE_4X4, win_condition, player1_move);
            if (game_status != GAME_STATUS_CONTINUE) {
                break;
            }

        }
    }

    wait_for_input();
    clear_lcd_screen();

    return game_status; 
}

/***
 * Function for a game of tic-tac-toe, containing multiple rounds.
 * 
 * @param size_option type of board and rules:
 *                    0 - 3x3 (normal)
 *                    1 - 4x4 easy (dimensions of 4x4, with win condition on line of 4)
 *                    2 - 4x4 hard (dimensions of 4x4, with win condition on line of 3)
 * @param mode b for pvbot, p for pvp
 * @param diff type of bot algorithm
 * 
 */
void play_xo(uint8_t size_option, char mode, char diff) {

    // determines who goes first
    // even -> player1 goes first
    // odd  -> player2 goes first
    uint8_t round_parity = 0;

    uint8_t player1_score = 0;
    uint8_t player2_score = 0;

    char *player1_name = "";
    char *player2_name = "";

    if (mode == 'p') {
        player1_name = "Player one";
        player2_name = "Player two";
    } else {
        player1_name = "Player";
        player2_name = "   Bot";
    }

    while (1) {
        print_line_to_lcd(0, "Score");
        print_score(1, player1_name, player1_score);
        print_score(2, player2_name, player2_score);

        wait_for_input();
        clear_lcd_screen();

        // print_line_to_lcd(1, "")
        
        wait_for_input();
        clear_lcd_screen();

        uint8_t result = 4;

        switch (size_option) {
            case 0: 
                result = xo_3x3_round(mode, diff, round_parity);
                break;
            case 1:
                result = xo_4x4_round(mode, diff, WIN_CONDITION_4, round_parity);
                break;
            case 2:
                result = xo_4x4_round(mode, diff, WIN_CONDITION_3, round_parity);
                break;
        }
            
        if (result == GAME_DRAW) {
            print_line_to_lcd(1, "It's a tie!");
        }
            
        // round_parity (is true) => player1 and player2 are reversed
        if ((result == 1 && !round_parity) || (result == 2 && round_parity)) {
            if (mode == 'p') {
                print_line_to_lcd(1, "Player one wins!");
            } else {
                print_line_to_lcd(1, "You win!");
            }
            player1_score++;
        }

        // round_parity (is true) => player1 and player2 are reversed
        if ((result == 2 && !round_parity) || (result == 1 && round_parity)) {
            if (mode == 'p') {
                print_line_to_lcd(1, "Player two wins!");
            } else {
                print_line_to_lcd(1, "You lose!");
            }
            player2_score++;
        }

        wait_for_input();
        clear_lcd_screen();

        char *quit_play_options[] = {"continue", "return"};
        print_line_to_lcd(0, "Keep playing?");
        uint8_t quit_playing = menu_interface(quit_play_options, 0, TWO_OPTIONS);

        if (quit_playing) {
            break;
        } else {
            round_parity = !round_parity;
            clear_lcd_screen();
        }
    }

    clear_lcd_screen();

    print_line_to_lcd(0, "Final Score");
    print_score(1, player1_name, player1_score);
    print_score(2, player2_name, player2_score);

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

    clear_lcd_screen();

    play_xo(size_option, mode, diff);

    clear_lcd_screen();
}