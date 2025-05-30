#include "print_to_lcd.h"
#include "timer.h"

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
    SSD1306_SetPosition(35, 0);
    SSD1306_DrawString(time_string);
    SSD1306_UpdateScreen(SSD1306_ADDR);
}

void print_options_to_lcd(char options[4][18], int option, int max_option) {
    char print_options[50];
    int position = 0;
    for (int i = 0; i <= max_option; i++) {
        if (i == option) {
            print_options[position++] = '>';
        } else {
            print_options[position++] = ' ';
        }
        int option_pos = 0;
        while(options[i][option_pos]) {
            print_options[position++] = options[i][option_pos++];
        }
        print_options[position++] = ' ';
    }
    print_options[position] = '\0';
    SSD1306_SetPosition(0, 2);
    SSD1306_DrawString(print_options);
    SSD1306_UpdateScreen(SSD1306_ADDR);
}

void value_to_string(char *string, int value, char append) {
    int power_of_ten = 1;
    while (value / power_of_ten >= 10) {
        power_of_ten *= 10;
    }
    int position = 0;
    while(power_of_ten > 0) {
        string[position++] = value % (power_of_ten * 10) / power_of_ten + '0';
        power_of_ten /= 10;
    }
    string[position++] = append;
    string[position] = '\0'; 
    // copy_string_values(append, string + position);
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
 * u = update
 * c = clear runtime
 * 
 */
void print_runtime_to_lcd(char command) {
    if (command == 'p') {
        clear_lcd_line(2);
        clear_lcd_line(3);
    }
    if (command == 'p' || command == 'u') {
        char print_runtime[18] = "Current runtime: ";
        SSD1306_SetPosition(0, 1);
        SSD1306_DrawString(print_runtime);
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

