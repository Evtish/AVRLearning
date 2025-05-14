#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>

#define LED_PIN PORTB1

int main(void) {
    /* -------------------- GPIO -------------------- */
    DDRB |= (1 << LED_PIN);  // set led pin as output

    /* -------------------- PWM -------------------- */
    TCCR1A |= (1 << COM1A1) |  // non-inverting PWM
    (1 << WGM10) | (1 << WGM11);  // set 10-bit fast PWM mode

    TCCR1B |= (1 << WGM12) |  // set 10-bit fast PWM mode
    (1 << CS10);  // set prescaler == 1

    /* -------------------- ADC -------------------- */
    ADMUX |= (1 << REFS0) |  // set VCC as voltage reference
    (1 << MUX0) | (1 << MUX1) |  // ADC on ADC3 (PORTC3) pin
    (1 << ADLAR);  // adjust ADC result to left

    ADCSRA |= (1 << ADEN) |  // enable ADC
    (1 << ADSC) |  // start ADC conversion
    (1 << ADIE) |  // enable conversion complete interrupt
    (1 << ADPS0) | (1 << ADPS1) | (1 << ADPS2);  // set prescaler == 1024

    DIDR0 |= (1 << ADC3D);  // disable digital input buffer to reduce power consumption (PINC3 will be always 0)

    sei();  // allow interrupts
    
    while (true) {
        /*
            set current PWM value
            use 8 high bits only due to the inaccuracy of 1-2 low bits
            change range from 0-255 to 0-1020
        */
        OCR1A = ADCH << 2;
    }
    
    return 0;
}