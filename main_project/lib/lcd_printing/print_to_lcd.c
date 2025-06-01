#include "print_to_lcd.h"

void clear_lcd_line(int line) {
    char clear[LCD_LINE_LEN + 1];
    for (int i = 0; i < LCD_LINE_LEN; i++) {
        clear[i] = ' ';
    }
    clear[LCD_LINE_LEN] = '\0';
    SSD1306_SetPosition(0, line);
    SSD1306_DrawString(clear);
    SSD1306_UpdateScreen(SSD1306_ADDR);
}

void print_time_to_lcd() {
    char time_string[12];
    time_to_string(time_string);
    SSD1306_SetPosition(CHAR_PIXELS * 5, 0);
    SSD1306_DrawString(time_string);
    SSD1306_UpdateScreen(SSD1306_ADDR);
}

void print_options_to_lcd(char *options[], uint8_t option, uint8_t no_of_options) {
    char print_options[50];
    uint8_t position = 0;
    uint8_t options0_len = 0;
    uint8_t options1_len = 0;
    uint8_t options2_len = 0;
    uint8_t options3_len = 0;
    char *count_len = options[1];
    while (*count_len) {
        options1_len++;
        count_len++;
    }
    if (no_of_options == 4) {
        count_len = options[3];
        while (*count_len) {
            options3_len++;
            count_len++;
        }
    }
    for (int i = 0; i < no_of_options; i++) {
        if (i == option) {
            print_options[position++] = '>';
        } else {
            print_options[position++] = ' ';
        }
        int option_pos = 0;
        while(options[i][option_pos]) {
            if (i == 0) {
                options0_len++;
            }
            if (i == 2) {
                options2_len++;
            }
            print_options[position++] = options[i][option_pos++];
        }
        if (i == 0) {
            for (int spaces = 0; spaces < LCD_LINE_LEN - options0_len - options1_len - 2; spaces++) {
                print_options[position++] = ' ';
            }
        } 
        if (i == 2) {
            for (int spaces = 0; spaces < LCD_LINE_LEN - options2_len - options3_len - 2; spaces++) {
                print_options[position++] = ' ';
            }
        }
    }
    print_options[position] = '\0';
    printf("%s\n\n", print_options);
    SSD1306_SetPosition(0, 2);
    SSD1306_DrawString(print_options);
    SSD1306_UpdateScreen(SSD1306_ADDR);
}

void runtime_to_string(char *string) {
    uint32_t runtime = systicks / 1000;
    uint8_t seconds = runtime % 60;
    runtime /= 60;
    uint32_t minutes = runtime % 60;
    runtime /= 60;
    uint8_t hours = runtime % 24;
    runtime /= 24;
    uint8_t days = runtime;
    uint8_t already_printed = 0;
    uint8_t position = 0;

    if (days > 0) {
        if (days > 9) {
            string[position++] = days / 10 + '0';
        }
        string[position++] = days % 10 + '0';
        string[position++] = 'd';
        string[position++] = ' ';
        already_printed = 1;
    }
    if (hours > 0 || already_printed) {
        if (hours > 9) {
            string[position++] = hours / 10 + '0';
        }
        string[position++] = hours % 10 + '0';
        string[position++] = 'h';
        string[position++] = ' ';
        already_printed = 1;
    }
    if (minutes > 0 || already_printed) {
        if (minutes > 9) {
            string[position++] = minutes / 10 + '0';
        }
        string[position++] = minutes % 10 + '0';
        string[position++] = 'm';
        string[position++] = ' ';
    }
    if (seconds > 9) {
        string[position++] = seconds / 10 + '0';
    }
    string[position++] = seconds % 10 + '0';
    string[position++] = 's';
    string[position++] = ' ';
    string[position++] = '\0';
}

/***
 * p = print runtime
 * c = clear runtime
 * u = update runtime (only value)
 * 
 */
void print_runtime_to_lcd(char command) {
    if (command == 'p') {
        clear_lcd_line(2);
        clear_lcd_line(3);
    }
    if (command == 'p') {
        char print_runtime[LCD_LINE_LEN] = "Current runtime: ";
        SSD1306_SetPosition(0, 1);
        SSD1306_DrawString(print_runtime);
    }
    if (command == 'p' || command == 'u') {
        char print_runtime[LCD_LINE_LEN];   
        runtime_to_string(print_runtime);
        SSD1306_SetPosition(0, 2);
        SSD1306_DrawString(print_runtime);
        SSD1306_UpdateScreen(SSD1306_ADDR);
    }
    if (command == 'c') {
        clear_lcd_line(1);
        clear_lcd_line(2);
        clear_lcd_line(3);
    }
}

void print_rand_to_lcd(uint32_t rand) {
    clear_lcd_line(1);
    SSD1306_UpdateScreen(SSD1306_ADDR);
    uint32_t wait_ping = systicks;
    while (!SYSTICKS_PASSED(wait_ping, 200));
    char rand_line[LCD_LINE_LEN + 1] = "   Rand: ";
    uint32_t pow10 = 100000;
    uint8_t non_zero = 0;
    uint8_t position = 9;
    if (rand == 0) {
        SSD1306_SetPosition(0, 1);
        SSD1306_DrawString("   Rand:      0");
        return;
    }
    while(pow10 > 0) {
        if (non_zero || rand / pow10 > 0) {
            non_zero = 1;
            rand_line[position++] = '0' + rand / pow10;
        } else {
            rand_line[position++] = ' ';
        }
        rand = rand % pow10;
        pow10 /= 10;
    }
    rand_line[position] = '\0';
    SSD1306_SetPosition(0, 1);
    SSD1306_DrawString(rand_line);
    SSD1306_UpdateScreen(SSD1306_ADDR);
}

void Set_time() {
    SSD1306_ClearScreen();
    uint8_t digit_position = 0;

    char time_string[12];
    char position_string[9] = "^       ";
    systicks = 0;
    time_to_string(time_string);
    SSD1306_SetPosition(CHAR_PIXELS * 5, 1);
    SSD1306_DrawString(time_string);
    SSD1306_SetPosition(CHAR_PIXELS * 5, 2);
    SSD1306_DrawString(position_string);
    SSD1306_UpdateScreen(SSD1306_ADDR);

    while (digit_position < 8) {
        if (left_blue_button) {
            left_blue_button = 0;
            if (digit_position != 0) {
                position_string[digit_position] = ' ';
                digit_position--;
                if (time_string[digit_position] == ':') {
                    digit_position--;
                }
                position_string[digit_position] = '^';
                SSD1306_SetPosition(CHAR_PIXELS * 5, 2);
                SSD1306_DrawString(position_string);
                SSD1306_UpdateScreen(SSD1306_ADDR);
            }
        }
        if (red_button) {
            red_button = 0;
            next_time_digit(time_string, digit_position);
            SSD1306_SetPosition(CHAR_PIXELS * 5, 1);
            SSD1306_DrawString(time_string);
            SSD1306_UpdateScreen(SSD1306_ADDR);
        }
        if (right_blue_button) {
            right_blue_button = 0;
            position_string[digit_position] = ' ';
            digit_position++;
            if (time_string[digit_position] == ':') {
                digit_position++;
            }
            position_string[digit_position] = '^';
            SSD1306_SetPosition(CHAR_PIXELS * 5, 2);
            SSD1306_DrawString(position_string);
            SSD1306_UpdateScreen(SSD1306_ADDR);
        }
    }
    SSD1306_ClearScreen();
    SSD1306_UpdateScreen(SSD1306_ADDR);
    set_start_time(time_string);
}

uint32_t get_value(char show_text[]) {
    SSD1306_ClearScreen();
    SSD1306_SetPosition(0, 0);
    SSD1306_DrawString(show_text);

    uint8_t digit_position = 0;

    char value_string[7]    = "000000";
    char position_string[7] = "^     ";
    SSD1306_SetPosition(CHAR_PIXELS * 6, 1);
    SSD1306_DrawString(value_string);
    SSD1306_SetPosition(CHAR_PIXELS * 6, 2);
    SSD1306_DrawString(position_string);
    SSD1306_UpdateScreen(SSD1306_ADDR);

    while (digit_position < 6) {
        if (left_blue_button) {
            left_blue_button = 0;
            if (digit_position != 0) {
                position_string[digit_position] = ' ';
                digit_position--;
                position_string[digit_position] = '^';
                SSD1306_SetPosition(CHAR_PIXELS * 6, 2);
                SSD1306_DrawString(position_string);
                SSD1306_UpdateScreen(SSD1306_ADDR);
            }
        }
        if (red_button) {
            red_button = 0;
            if (value_string[digit_position] - '0' == 9) {
                value_string[digit_position] = '0';
            } else {
                value_string[digit_position]++;
            }
            SSD1306_SetPosition(CHAR_PIXELS * 6, 1);
            SSD1306_DrawString(value_string);
            SSD1306_UpdateScreen(SSD1306_ADDR);
        }
        if (right_blue_button) {
            right_blue_button = 0;
            position_string[digit_position] = ' ';
            digit_position++;
            position_string[digit_position] = '^';
            SSD1306_SetPosition(CHAR_PIXELS * 6, 2);
            SSD1306_DrawString(position_string);
            SSD1306_UpdateScreen(SSD1306_ADDR);
        }
    }
    uint32_t value = 0;
    digit_position = 0;
    while (value_string[digit_position] != '\0') {
        value = value * 10 + value_string[digit_position] - '0';
        digit_position++;
    }
    SSD1306_ClearScreen();
    SSD1306_UpdateScreen(SSD1306_ADDR);
    return value;
}