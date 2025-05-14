#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>

#define BUTTON_PIN PORTD7
#define LED_PIN PORTB1

#define BTN_WAIT_MS 20
#define BTN_DEBOUNCE_CHECK_PERIOD_MS 5
#define BTN_DEBOUNCE_AMOUNT_TO_PASS (BTN_WAIT_MS / BTN_DEBOUNCE_CHECK_PERIOD_MS)

#define TIMER_PRESCALER_PWR_INDX 10
#define TIMER_TICK_AMOUNT ((F_CPU >> TIMER_PRESCALER_PWR_INDX) * BTN_DEBOUNCE_CHECK_PERIOD_MS / 1000)

#define PIND_IS_LOW(x) (PIND & (1 << (x)))

volatile uint16_t btn_debounce_passed_amount = false;

ISR(PCINT2_vect) {
    if (!PIND_IS_LOW(BUTTON_PIN))
        TCCR0B = (1 << CS00) | (1 << CS02);
}

ISR(TIMER0_COMPA_vect) {
    if (!PIND_IS_LOW(BUTTON_PIN))
        btn_debounce_passed_amount++;
    else {
        TCCR0B = 0;
        btn_debounce_passed_amount = 0;
    }

    if (btn_debounce_passed_amount >= BTN_DEBOUNCE_AMOUNT_TO_PASS)
        TCCR0B = 0;
}

int main(void) {
    DDRB |= (1 << LED_PIN);
    PORTD |= (1 << BUTTON_PIN);

    PCICR = (1 << PCIE2);
    PCMSK2 = (1 << PCINT23);

    TCCR0A = (1 << WGM01);
    OCR0A = TIMER_TICK_AMOUNT;
    TIMSK0 = (1 << OCIE0A);

    sei();

    while (true) {
        if (btn_debounce_passed_amount >= BTN_DEBOUNCE_AMOUNT_TO_PASS) {
            PORTB ^= (1 << LED_PIN);
            btn_debounce_passed_amount = 0;
        }
    }
    
    return 0;
}