#ifndef PRINT_TO_LCD_H
#define PRINT_TO_LCD_H

#include <stdio.h>
#include "ssd1306.h"

#define LCD_LINE_LEN 18
#define LCD_LINES 4
#define CHAR_PIXELS 7

void clear_lcd_line(int line);
void value_to_string(char *string, int value, char append);
void runtime_to_string(char *string);
void print_time_to_lcd();
void print_options_to_lcd(char options[4][18], int option, int max_option);
void print_runtime_to_lcd(char command);
void Set_time();
uint32_t get_value(char show_text[]);

#endif // PRINT_TO_LCD_H

