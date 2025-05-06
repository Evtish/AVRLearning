#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>

#define BUTTON_PIN PORTD7
#define LED_PIN PORTB1

#define BTN_WAIT_MS 20
#define BTN_DEBOUNCE_CHECK_AMOUNT 4

#define TIMER_PRESCALER_PWR_INDX 10
#define TIMER_TICK_AMOUNT ((F_CPU >> TIMER_PRESCALER_PWR_INDX) * BTN_WAIT_MS / BTN_DEBOUNCE_CHECK_AMOUNT / 1000)

#define PIND_IS_LOW(x) (PIND & (1 << (x)))

volatile bool btn_is_clicked = 0;

ISR(TIMER0_COMPA_vect) {
    bool btn_is_pressed = !PIND_IS_LOW(BUTTON_PIN);
    static bool btn_was_pressed = false;
    static uint8_t btn_press_amount = 0;

    if (btn_is_pressed && !btn_was_pressed)
        btn_press_amount = 1;
    else if (btn_is_pressed && btn_was_pressed && btn_press_amount >= 1)
        btn_press_amount++;
    else
        btn_press_amount = 0;

    if (btn_press_amount >= BTN_DEBOUNCE_CHECK_AMOUNT) {
        btn_is_clicked = true;
        btn_press_amount = 0;
    }

    btn_was_pressed = btn_is_pressed;
}

int main(void) {
    DDRB |= (1 << LED_PIN);
    PORTD |= (1 << BUTTON_PIN);

    TCCR0A = (1 << WGM01);
    TCCR0B = (1 << CS00) | (1 << CS02);

    sei();
    TIMSK0 = (1 << OCIE0A);
    OCR0A = TIMER_TICK_AMOUNT;

    while (true) {
        if (btn_is_clicked) {
            PORTB ^= (1 << LED_PIN);
            btn_is_clicked = false;
        }
    }
    
    return 0;
}