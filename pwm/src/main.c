#define F_CPU 16000000U

#include <avr/io.h>
#include <avr/interrupt.h>
#include "../inc/manage_pwm.h"

#define LED_PIN PORTB1

#define PWM_BIT_CAPACITY 0b1010  // bits
#define PWM_MAX_VAL (1 << (PWM_BIT_CAPACITY)) - 1

ISR(TIMER0_OVF_vect) {
    pwm_step(&OCR1A, PWM_MAX_VAL);
}

int main(void) {
    DDRB |= (1 << LED_PIN);

    sei();

    TCCR0B = (1 << CS00) | (1 << CS01);
    TIMSK0 = (1 << TOIE0);

    TCCR1A = (1 << COM1A1) | (1 << WGM10) | (1 << WGM11);
    TCCR1B = (1 << WGM12) | (1 << CS10);

    while (1) {}
    return 0;
}