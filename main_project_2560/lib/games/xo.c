#include "xo.h"

/***
 * Print tic-tac-toe grid.
 * 
 */
void print_xo_borders() {
    uint8_t corner_x = XO_CORNER_X;
    uint8_t corner_y = XO_CORNER_Y;

    for (int i = 0; i < XO_SIZE + 1; i++) {
        SSD1306_DrawLine(corner_x, corner_x + XO_CELL_SIZE * XO_SIZE * CHAR_LENGTH, corner_y - (i > 0) + i * CHAR_HEIGHT, corner_y - (i > 0) + i * CHAR_HEIGHT);
        SSD1306_DrawLine(corner_x + i * 3 * CHAR_LENGTH, corner_x + i * 3 * CHAR_LENGTH, corner_y, corner_y + XO_SIZE * CHAR_HEIGHT - 1);
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
void xo_place_char(char c, uint8_t position, uint8_t is_select) {
    uint8_t corner_x = XO_CORNER_X_CHAR;
    uint8_t corner_y = XO_CORNER_Y_CHAR;

    uint8_t cell_corner_x = XO_CORNER_X + (position % XO_SIZE) * 3 * CHAR_LENGTH;
    uint8_t cell_corner_y = XO_CORNER_Y + (position / XO_SIZE) * CHAR_HEIGHT - 1; 

    // adds one to position X or O in middle of grid cell
    print_char_at(c, corner_x + (1 - is_select) + 3 * (position % XO_SIZE), corner_y + position / XO_SIZE);
    
    // rebuilding border
    SSD1306_DrawRectangle(cell_corner_x, cell_corner_x + 3 * CHAR_LENGTH, cell_corner_y + ((position / XO_SIZE) == 0), cell_corner_y + CHAR_HEIGHT);
    SSD1306_UpdateScreen(SSD1306_ADDR);
}

/***
 * Returns the status of the game.
 * 
 * @param board the xo board
 * @param last_placement position of last player's move
 * (function needs to check only if the last placement 
 * has changed the status of the game)
 * @param show_winner whether to print the line on the lcd
 *                    that shows how the game was won
 * 
 * @return uint8_t status:
 *                 - 0 = it's a draw (no player has won and all positions are covered)
 *                 - 1 = player1 won
 *                 - 2 = player2 won
 *                 - 3 = game hasn't ended
 * 
 */
uint8_t xo_status(uint8_t board[], uint8_t last_placement, uint8_t show_winner) {
    uint8_t last_placed = board[last_placement];

    // coordinates of last placed X/O in x and y
    uint8_t last_x = last_placement % XO_SIZE;
    uint8_t last_y = last_placement / XO_SIZE;

    // first diagonal
    if (last_x == last_y) {
        if (board[0] == last_placed && board[4] == last_placed && board[8] == last_placed) {
            if (show_winner) {
                SSD1306_DrawLine(XO_CORNER_X, XO_CORNER_X + 9 * CHAR_LENGTH, XO_CORNER_Y, XO_CORNER_Y + 3 * CHAR_HEIGHT - 2);
                SSD1306_UpdateScreen(SSD1306_ADDR);
            }
            return last_placed;
        }
    }

    // second diagonal
    if (last_x + last_y == XO_SIZE - 1) {
        if (board[2] == last_placed && board[4] == last_placed && board[6] == last_placed) {
            if (show_winner) {
                SSD1306_DrawLine(XO_CORNER_X, XO_CORNER_X + 3 * XO_CELL_SIZE * CHAR_LENGTH, XO_CORNER_Y + 3 * CHAR_HEIGHT - 2, XO_CORNER_Y);
                SSD1306_UpdateScreen(SSD1306_ADDR);
            }
            return last_placed;
        }
    }

    // vertical check
    if (board[last_x] == last_placed && board[XO_SIZE + last_x] == last_placed && board[2 * XO_SIZE + last_x] == last_placed) {
        if (show_winner) {
            SSD1306_DrawLine(XO_CORNER_X + XO_CELL_SIZE * CHAR_LENGTH * (last_x + 0.5), XO_CORNER_X + XO_CELL_SIZE * CHAR_LENGTH * (last_x + 0.5), XO_CORNER_Y, XO_CORNER_Y + 3 * CHAR_HEIGHT - 1);
            SSD1306_UpdateScreen(SSD1306_ADDR);
        }
        return last_placed;
    }   

    // horizontal check
    if (board[XO_SIZE * last_y] == last_placed && board[XO_SIZE * last_y + 1] == last_placed && board[XO_SIZE * last_y + 2] == last_placed) {
        if (show_winner) {
            SSD1306_DrawLine(XO_CORNER_X, XO_CORNER_X + 3 * XO_CELL_SIZE * CHAR_LENGTH, XO_CORNER_Y + CHAR_HEIGHT * (last_y + 0.5) - 1, XO_CORNER_Y + CHAR_HEIGHT * (last_y + 0.5) - 1);
            SSD1306_UpdateScreen(SSD1306_ADDR);
        }
        return last_placed;
    }

    // if no one has won, check that there are still empty positions
    for (int i = 0; i < XO_SIZE * XO_SIZE; i++) {

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
uint8_t get_player_move_xo(uint8_t board[]) {
    uint8_t position = 0;

    // searches for first empty position
    while (board[position] != EMPTY) {
        position++;
        
        // if no position is available, return 255 (error)
        if (position == XO_SIZE * XO_SIZE) {
            return 255;
        }
    }

    board[position] = SELECT;
    xo_place_char('>', position, IS_SELECT);

    while (1) {
        // previous position
        if (left_blue_button) {
            left_blue_button = 0;
            board[position] = EMPTY;
            xo_place_char(' ', position, IS_SELECT);
            position = PREV_OPTION(position, XO_SIZE * XO_SIZE); 
            while (board[position] != EMPTY) {
                position = PREV_OPTION(position, XO_SIZE * XO_SIZE); 
            }
            board[position] = SELECT;
            xo_place_char('>', position, IS_SELECT);
        }

        // selects position
        if (red_button) {
            red_button = 0;
            break;
        }

        if (right_blue_button) {
            right_blue_button = 0;
            board[position] = EMPTY;
            xo_place_char(' ', position, IS_SELECT);
            position = NEXT_OPTION(position, XO_SIZE * XO_SIZE); 
            while (board[position] != EMPTY) {
                position = NEXT_OPTION(position, XO_SIZE * XO_SIZE); 
            }
            board[position] = SELECT;
            xo_place_char('>', position, IS_SELECT);
        }
    }
    xo_place_char(' ', position, IS_SELECT);

    return position;
}

/***
 * Bot decides where to place
 * 
 * @param diff difficulty chosen for bot (r = random, b = blocker, p = perfect)
 * @param last_player_move if it is equal to size, bot is first 
 *                         otherwise, it is player's last move (to update bot_board)
 * 
 * @return uint8_t position of where it places
 * 
 */
uint8_t xo_bot_turn(char diff, uint8_t board[]) {
    if (diff == 'r') {
        uint8_t random_choice = random(0, XO_SIZE * XO_SIZE - 1);
        while (board[random_choice] != EMPTY) {
            random_choice = random(0, XO_SIZE * XO_SIZE - 1);
        }
        return random_choice;
    }

    // check if there is a spot where bot would win
    for (int position = 0; position < XO_SIZE * XO_SIZE; position++) {
        if (board[position] == EMPTY) {

            // simulate player move
            board[position] = O;
            uint8_t get_status = xo_status(board, position, DONT_SHOW_WINNER);
            board[position] = EMPTY;

            // player wins
            if (get_status == O) {
                return position;
            }
        }
    }

    // check if there is a spot where player would win
    // if there are multiple, the bot loses anyway
    for (int position = 0; position < XO_SIZE * XO_SIZE; position++) {
        if (board[position] == EMPTY) {

            // simulate player move
            board[position] = X;
            uint8_t get_status = xo_status(board, position, DONT_SHOW_WINNER);
            board[position] = EMPTY;

            // player wins
            if (get_status == X) {
                return position;
            }
        }
    }

    // if the player will not win next move and bot can't win,
    // choose a random spot
    if (diff == 'b') {
        uint8_t random_choice = random(0, XO_SIZE * XO_SIZE - 1);
        while (board[random_choice] != EMPTY) {
            random_choice = random(0, XO_SIZE * XO_SIZE - 1);
        }
        return random_choice;
    }

    // for perfect, it calculates the position with highest possible wins 
    // (similar to checking if someone has won)
    if (diff == 'p') {
        uint8_t best_position = 255;
        uint8_t highest_wins = 0;
        for (int position = 0; position < XO_SIZE * XO_SIZE; position++) {

            // skips non-empty positions
            if (board[position] != EMPTY) {
                continue;
            }

            uint8_t possible_wins = 0;

            // coordinates of position in x and y
            uint8_t position_x = position % XO_SIZE;
            uint8_t position_y = position / XO_SIZE;

            // if a line has at least an X, then it's blocked and can't be used to win

            // first diagonal
            if (position_x == position_y) {
                if (board[0] != X && board[4] != X && board[8] != X) {
                    possible_wins++;
                }
            }

            // second diagonal
            if (position_x + position_y == XO_SIZE - 1) {
                if (board[2] != X && board[4] != X && board[6] != X) {
                    possible_wins++;
                }
            }

            // vertical check
            if (board[position_x] != X && board[XO_SIZE + position_x] != X && board[2 * XO_SIZE + position_x] != X) {
                possible_wins++;
            }   

            // horizontal check
            if (board[XO_SIZE * position_y] != X && board[XO_SIZE * position_y + 1] != X && board[XO_SIZE * position_y + 2] != X) {
                possible_wins++;
            }

            // if the bot found a better position, it chooses it
            if (best_position == 255 || possible_wins > highest_wins) {
                highest_wins = possible_wins;
                best_position = position;
            }
        }
        return best_position;
    } 

    // in case it somehow gets here (it shouldn't)
    return xo_bot_turn('r', board);
}

/***
 * Plays a round of 3x3 tic-tac-toe, pvp or pvbot.
 * 
 * @param infinite if 1, game is infinite 
 *                  (after a player has placed three X/O's,
 *                  their earliest placement is removed)
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
uint8_t xo_round(uint8_t infinite, char mode, char diff, uint8_t round_parity) {
    print_xo_borders(XO_SIZE);
    uint8_t xo_board[XO_SIZE * XO_SIZE] = { EMPTY, EMPTY, EMPTY,   
                                            EMPTY, EMPTY, EMPTY,   
                                            EMPTY, EMPTY, EMPTY};

    uint8_t game_status = GAME_STATUS_CONTINUE;

    uint8_t player1_move = 0;
    uint8_t player2_move = 0;

    // movement queue for infinite version
    uint8_t infinite_queue[XO_INFINITE_MOVES];
    uint8_t queue_position = 0;
    uint8_t infinite_remove = 0;

    while (1) {
        if (round_parity == 0) {
            if (infinite_remove) {
                xo_board[infinite_queue[queue_position]] = EMPTY;
                xo_place_char(' ', infinite_queue[queue_position], IS_NOT_SELECT);
                uint32_t wait_a_moment = systicks;
                while(!SYSTICKS_PASSED(wait_a_moment, 750));
            }

            player1_move = get_player_move_xo(xo_board);
            xo_board[player1_move] = X;
            xo_place_char('X', player1_move, IS_NOT_SELECT);
            
            game_status = xo_status(xo_board, player1_move, SHOW_WINNER);
            if (game_status != GAME_STATUS_CONTINUE) {
                break;
            }

            if (infinite) {
                infinite_queue[queue_position++] = player1_move;
            }


            if (infinite_remove) {
                xo_board[infinite_queue[queue_position]] = EMPTY;
                xo_place_char(' ', infinite_queue[queue_position], IS_NOT_SELECT);
                uint32_t wait_a_moment = systicks;
                while(!SYSTICKS_PASSED(wait_a_moment, 750));
            }

            if (mode == 'p') {
                player2_move = get_player_move_xo(xo_board);
            } else {
                player2_move = xo_bot_turn(diff, xo_board);
                if (!infinite_remove) {
                    uint32_t wait_a_moment = systicks;
                    while(!SYSTICKS_PASSED(wait_a_moment, 750));
                }
            }
            xo_board[player2_move] = O;
            xo_place_char('O', player2_move, IS_NOT_SELECT);

            if (mode == 'b') {
                uint32_t wait_a_moment = systicks;
                while(!SYSTICKS_PASSED(wait_a_moment, 750));
            }

            game_status = xo_status(xo_board, player2_move, SHOW_WINNER);
            if (game_status != GAME_STATUS_CONTINUE) {
                break;
            }

            if (infinite) {
                infinite_queue[queue_position++] = player2_move;
                if (queue_position == XO_INFINITE_MOVES) {
                    queue_position = 0;
                    if (!infinite_remove) {
                        infinite_remove = 1;
                    }
                }
            }

        } else {
            if (infinite_remove) {
                xo_board[infinite_queue[queue_position]] = EMPTY;
                xo_place_char(' ', infinite_queue[queue_position], IS_NOT_SELECT);
                uint32_t wait_a_moment = systicks;
                while(!SYSTICKS_PASSED(wait_a_moment, 750));
            }

            if (mode == 'p') {
                player2_move = get_player_move_xo(xo_board);
            } else {
                player2_move = xo_bot_turn(diff, xo_board);
                if (!infinite_remove) {
                    uint32_t wait_a_moment = systicks;
                    while(!SYSTICKS_PASSED(wait_a_moment, 750));
                }
            }
            xo_board[player2_move] = O;
            xo_place_char('O', player2_move, IS_NOT_SELECT);

            if (mode == 'b') {
                uint32_t wait_a_moment = systicks;
                while(!SYSTICKS_PASSED(wait_a_moment, 750));
            }
            
            game_status = xo_status(xo_board, player2_move, SHOW_WINNER);
            if (game_status != GAME_STATUS_CONTINUE) {
                break;
            }
            
            if (infinite) {
                infinite_queue[queue_position++] = player2_move;
            }


            if (infinite_remove) {
                xo_board[infinite_queue[queue_position]] = EMPTY;
                xo_place_char(' ', infinite_queue[queue_position], IS_NOT_SELECT);
                uint32_t wait_a_moment = systicks;
                while(!SYSTICKS_PASSED(wait_a_moment, 750));
            }

            player1_move = get_player_move_xo(xo_board);
            xo_board[player1_move] = X;
            xo_place_char('X', player1_move, IS_NOT_SELECT);
            
            game_status = xo_status(xo_board, player1_move, SHOW_WINNER);
            if (game_status != GAME_STATUS_CONTINUE) {
                break;
            }
            
            if (infinite) {
                infinite_queue[queue_position++] = player1_move;
                if (queue_position == XO_INFINITE_MOVES) {
                    queue_position = 0;
                    if (!infinite_remove) {
                        infinite_remove = 1;
                    }
                }
            }
        }
    }
    
    uint32_t wait_a_moment = systicks;
    while(!SYSTICKS_PASSED(wait_a_moment, 1000));
    clear_lcd_screen();

    return game_status;
}

/***
 * Function for a game of tic-tac-toe, containing multiple rounds.
 * 
 * @param game_type type of xo game:
 *                    0 - normal
 *                    1 - infinite
 * @param mode b for pvbot, p for pvp
 * @param diff type of bot algorithm
 * 
 */
void play_xo(uint8_t game_type, char mode, char diff) {

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

        print_line_to_lcd(0, "Tic-tac-toe");

        if (mode == 'p') {
            if (!round_parity) {
                print_line_to_lcd(1, player1_name);
                print_line_to_lcd(2, " goes first!");
            } else {
                print_line_to_lcd(1, player2_name);
                print_line_to_lcd(2, " goes first!");
            }
        } else {
            if (!round_parity) {
                print_line_to_lcd(1, "The player");
                print_line_to_lcd(2, " goes first!");
            } else {
                print_line_to_lcd(1, "The bot");
                print_line_to_lcd(2, "goes first!");
            }
        }

        wait_for_input();
        clear_lcd_line(1);
        clear_lcd_line(2);

        uint8_t result = xo_round(game_type, mode, diff, round_parity);

        print_line_to_lcd(0, "Tic-tac-toe");
            
        if (result == GAME_DRAW) {
            print_line_to_lcd(1, "It's a tie!");
        }
            
        // player1 is X
        if (result == X) {
            if (mode == 'p') {
                print_line_to_lcd(1, "Player one wins!");
            } else {
                print_line_to_lcd(1, "You win!");
            }
            player1_score++;
        }

        // player2 is O
        if (result == O) {
            if (mode == 'p') {
                print_line_to_lcd(1, "Player two wins!");
            } else {
                print_line_to_lcd(1, "You lose!");
            }
            player2_score++;
        }

        wait_for_input();
        clear_lcd_line(0);
        clear_lcd_line(1);

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

        // r = random, b = blocker, p = perfect
        char diff_modes[] = {'r', 'b', 'p'};
        
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
    char *game_types[] = {"normal", "infinte", "", "return"};
    char game_text[] = "Choose game type:";
    uint8_t game_option = 0;
        
    print_line_to_lcd(1, game_text);
    game_option = menu_interface(game_types, game_option, FOUR_OPTIONS);

    if (game_option == 3) {
        clear_lcd_screen();
        return;
    }

    clear_lcd_screen();

    play_xo(game_option, mode, diff);

    clear_lcd_screen();
}