#include "defines.h"
#include "inits.h"
#include <avr/io.h>

void init_TIMER0_CTC_interrupts(void) {
    TCCR0A = (1 << WGM01);  // set timer to CTC mode
    TCCR0B = TIMER_PRESCALER;  // set timer prescaler
    TIMSK0 = (1 << OCIE0A);  // enable timer CTC interrupt
    OCR0A = TIMER_TICK_AMOUNT;  // set value to match
}

void init_PCINT23_external_interrupts(void) {
    PCICR = (1 << PCIE2);  // enable pin change interrupt 2 (PCINT23..16)
    PCMSK2 = (1 << PCINT23);  // enable pin change interrupt on PCINT23 (button pin)

    TCCR0A = (1 << WGM01);
    TIMSK0 = (1 << OCIE0A);
    OCR0A = TIMER_TICK_AMOUNT;
}

void init_TIMER0_counter(void) {
    TCCR0B = TIMER_PRESCALER;
}

/* ------------ initialisation of pins ------------ */

void init_LED_PORTB(void) {
    DDRB |= (1 << LED_PIN);  // set pin as output
}

void init_button_PORTD(void) {
    PORTD |= (1 << BUTTON_PIN);  // enable pin pull-up resistor
}