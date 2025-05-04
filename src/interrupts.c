#define F_CPU 16000000U

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define LED_PIN PORTB5
#define OCR1A_PIN PORTD6
#define OCR1A_PORT PORTD

#define TIMER1_PRESCALER_PWR_INDX 8  // 2 ** TIMER1_PRESCALER_PWR_INDX is timer1 prescaler
#define OCR1A_VAL (F_CPU >> TIMER1_PRESCALER_PWR_INDX)  // actually this equals F_CPU / 2 ** TIMER1_PRESCALER_PWR_INDX

// toggle LED if TIMER1 equals OCR1A value
// ISR(TIMER1_COMPA_vect) {
//     PORTB ^= (1 << LED_PIN);
// }

int main(void) {
    // set pins as output
    DDRB |= (1 << LED_PIN);
    // DDRD |= (1 << OCR1A_PIN);

    // set OCR1A value
    OCR1AH = (OCR1A_VAL >> 8);  // high byte
    OCR1AL = (OCR1A_VAL << 8);  // low byte
    // OCR1A = OCR1A_VAL;

    // TCCR1A |= (1 << COM1A0);
    TCCR1B |= (1 << WGM12) | (1 << CS12);  // set timer to CTC mode and set timer prescaler

    sei();  // enable interrupts
    // SREG |= SREG_I;

    TIMSK1 |= (1 << OCIE1A);  // enable CTC interrupt

    // toggle LED if TIMER1 equals OCR1A value
    if (TIFR1 & ~(1 << OCF1A)) {
        PORTB ^= (1 <<LED_PIN);
    }

    while (1) {}  // infinite loop
}

// T = 2 ** timer_bit_capacity * timer_prescaler / freq
// timer_prescaler = freq * T / 2 ** timer_bit_capacity