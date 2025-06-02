#include "timer.h"

/* Allocates the system ticks counter (milliseconds since boot). */
volatile uint32_t systicks = 0;
uint32_t start_time = 0;

/* Clock correction */
#define CLOCK_SKEW 86400
uint32_t last_correction = 0;

void Timer2_init_systicks(void) {
    /* Clear previous settings */
    TCCR2A = 0;
    TCCR2B = 0;

    /* Set CTC mode */
    TCCR2A |= (1 << WGM01);

    /* Set prescaler to 8 */
    TCCR2B |= (1 << CS22);

    /* Activate Compare A interrupt */
    TIMSK2 |= (1 << OCIE2A);

    /* 16MHz / 8 => 2000 kHz */
    OCR2A = 249;
}

/* interrupt every 1ms */
ISR(TIMER2_COMPA_vect) {
    if (SYSTICKS_PASSED(last_correction, CLOCK_SKEW)) {
        last_correction = systicks;
    } else { 
        systicks++;
    }
}
