#ifndef PRINT_TO_LCD_H
#define PRINT_TO_LCD_H

#include "ssd1306.h"
#include "buttons.h"

#define LCD_LINE_LEN 18
#define LCD_LINES 4
#define CHAR_LENGTH 7
#define CHAR_HEIGHT 8
#define MAX_RANDOM 999999

void append_chars(char *main_char, const char *add_char);
void print_lcd_boxes();
void clear_lcd_screen();
void clear_lcd_line(int line);
void runtime_to_string(char *string);
void print_options_to_lcd(char *options[], uint8_t option, uint8_t no_of_options);
void print_runtime_to_lcd(char command);
void print_rand_to_lcd(uint32_t rand, uint32_t max_rand);
void print_char_at(char c, uint8_t char_x, uint8_t char_y);
void print_line_to_lcd(uint8_t line, char text_line[]);
uint32_t get_value(char show_text[]);

#endif // PRINT_TO_LCD_H

