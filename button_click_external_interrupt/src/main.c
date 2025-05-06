#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>

#define BUTTON_PIN PORTD7
#define LED_PIN PORTB1

#define BTN_WAIT_MS 20
#define BTN_DEBOUNCE_CHECK_AMOUNT 1

#define TIMER1_PRESCALER_BITS ((1 << CS10) | (1 << CS12))
#define TIMER_PRESCALER_PWR_INDX 10
#define TIMER_TICK_AMOUNT ((F_CPU >> TIMER_PRESCALER_PWR_INDX) * BTN_WAIT_MS / BTN_DEBOUNCE_CHECK_AMOUNT / 1000)

#define PIND_IS_LOW(x) (PIND & (1 << (x)))

volatile bool debounce_passed = false;

ISR(PCINT2_vect) {
    TCCR1B |= TIMER1_PRESCALER_BITS;
}

ISR(TIMER1_COMPA_vect) {
    TCCR1B &= ~TIMER1_PRESCALER_BITS;
    debounce_passed = true;
}

int main(void) {
    DDRB |= (1 << LED_PIN);
    PORTD |= (1 << BUTTON_PIN);

    PCICR = (1 << PCIE2);
    PCMSK2 = (1 << PCINT23);

    TCCR1B = (1 << WGM12);
    OCR1A = TIMER_TICK_AMOUNT;
    TIMSK1 = (1 << OCIE1A);

    sei();

    while (true) {
        if (debounce_passed) {
            if (!PIND_IS_LOW(BUTTON_PIN))
                PORTB ^= (1 << LED_PIN);
            debounce_passed = false;
        }
    }
    
    return 0;
}