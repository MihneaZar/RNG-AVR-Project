#ifndef PRINT_TO_LCD_H
#define PRINT_TO_LCD_H

#include <stdio.h>

#include "ssd1306.h"
#include "timer.h"
#include "buttons.h"

#define LCD_LINE_LEN 18
#define LCD_LINES 4
#define CHAR_PIXELS 7

void clear_lcd_line(int line);
void runtime_to_string(char *string);
void print_time_to_lcd();
void print_options_to_lcd(char *options[], uint8_t option, uint8_t no_of_options);
void print_runtime_to_lcd(char command);
void print_rand_to_lcd(uint32_t rand);
void Set_time();
uint32_t get_value(char show_text[]);

#endif // PRINT_TO_LCD_H

