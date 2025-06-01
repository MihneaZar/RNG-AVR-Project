#ifndef MENUS_H
#define MENUS_H

#include <avr/io.h>

#include "timer.h"
#include "buttons.h"
#include "print_to_lcd.h"

uint32_t random(uint32_t min_rand, uint32_t max_rand);
uint8_t menu_interface(char *options[], uint8_t option, uint8_t max_option);
void main_menu();
void game_menu();

#endif // PRINT_TO_LCD_H
