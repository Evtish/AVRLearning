#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>

#define BUTTON_PIN PORTD7
#define LED_PIN PORTB1

#define BTN_WAIT_MS 2000
#define BTN_DEBOUNCE_CHECK_PERIOD_MS 5
#define BTN_DEBOUNCE_AMOUNT_TO_PASS (BTN_WAIT_MS / BTN_DEBOUNCE_CHECK_PERIOD_MS)

#define TIMER_BITNESS 8
#define TIMER_SIZE (1 << TIMER_BITNESS)
#define TIMER_PRESCALER_PWR_INDX 10
#define TIMER_TICK_AMOUNT ((F_CPU >> TIMER_PRESCALER_PWR_INDX) * BTN_DEBOUNCE_CHECK_PERIOD_MS / 1000)

#define PIND_IS_LOW(x) (PIND & (1 << (x)))
#define TICK_DELTA(counter, val) ((counter) >= (val) ? (counter) - (val) : TIMER_SIZE + (counter) - (val))

volatile uint16_t btn_debounce_passed_amount = 0;

int main(void) {
    uint8_t last_call_time = 0;//, btn_click_amount = 0;
    bool btn_is_pressed = false, btn_was_pressed = false;

    DDRB |= (1 << LED_PIN);
    PORTD |= (1 << BUTTON_PIN);

    TCCR0B = (1 << CS00) | (1 << CS02);

    while (true) {
        if (TICK_DELTA(TCNT0, last_call_time) >= TIMER_TICK_AMOUNT) {
            btn_is_pressed = !PIND_IS_LOW(BUTTON_PIN);

            if (btn_is_pressed) {
                if (!btn_was_pressed)
                    btn_debounce_passed_amount = 1;
                else if (btn_debounce_passed_amount >= 1)
                    btn_debounce_passed_amount++;
            }
            else
                btn_debounce_passed_amount = 0;

            btn_was_pressed = btn_is_pressed;
            last_call_time = TCNT0;
        }

        if (btn_debounce_passed_amount >= BTN_DEBOUNCE_AMOUNT_TO_PASS) {
            // btn_click_amount++;
            PORTB ^= (1 << LED_PIN);
            btn_debounce_passed_amount = 0;
        }

        // if (btn_click_amount >= 3) {  // && time_sinse_last_click_ms <= 250
        //     btn_click_amount = 0;
        //     PORTB ^= (1 << LED_PIN);
        // }
    }
    
    return 0;
}