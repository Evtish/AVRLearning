#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include "../inc/manage_pwm.h"

#define LED_PIN PORTB1

#define PWM_BITNESS 0b1010  // bits
#define PWM_MAX_VAL ((1 << PWM_BITNESS) - 1)

// adjust PWM output when a little time has passed
ISR(TIMER0_OVF_vect) {
    pwm_step(&OCR1A, PWM_MAX_VAL);
}

int main(void) {
    DDRB |= (1 << LED_PIN);  // set pin as output

    sei();  // allow interrupts

    // use TIMER0 for interrupts
    TCCR0B = (1 << CS00) | (1 << CS01);  // set prescaler
    TIMSK0 = (1 << TOIE0);  // turn overflow interrupts on

    // use TIMER1 for 10-bit fast PWM
    TCCR1A = (1 << COM1A1) | (1 << WGM10) | (1 << WGM11);  // set compare output mode as non-inverting, waveform generator as 10-bit fast PWM
    TCCR1B = (1 << WGM12) | (1 << CS10);  // set waveform generator as 10-bit fast PWM

    while (true) {}
    return 0;
}