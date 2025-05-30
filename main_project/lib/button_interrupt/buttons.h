#ifndef BUTTONS_H_
#define BUTTONS_H_

extern volatile uint8_t blue_button; 
extern volatile uint8_t red_button; 
uint32_t red_count();
uint32_t blue_count();

void Buttons_init();

#endif // BUTTONS_H_
