#include "defines.h"
#include "controls.h"
#include <stdbool.h>
#include <avr/io.h>
#include <avr/interrupt.h>

volatile uint16_t passed_debounce_amount = 0;

ISR(TIMER0_COMPA_vect) {
    update(BUTTON_PIN, &passed_debounce_amount);
}

int main(void) {
    DDRB |= (1 << LED_PIN);
    PORTD |= (1 << BUTTON_PIN);

    TCCR0A = (1 << WGM01);
    TCCR0B = (1 << CS00) | (1 << CS02);

    TIMSK0 = (1 << OCIE0A);
    OCR0A = TIMER_TICK_AMOUNT;
    
    sei();

    while (true) {
        if (passed_debounce_amount >= BTN_DEBOUNCE_AMOUNT_TO_PASS) {
            PORTB ^= (1 << LED_PIN);
            passed_debounce_amount = 0;
        }
    }
    
    return 0;
}