#define F_CPU 16000000U

#include <avr/io.h>
#include <avr/interrupt.h>

#define BUTTON_PIN PORTD7
#define LED_PIN PORTD3

#define PIN_IS_LOW(x) (PINB & (1 << (x)))
#define TIME_DELTA(counter, val) ((counter) >= (val) ? (counter) - (val) : 256 + (counter) - (val))

// uint8_t free_wait_ms(uint16_t wait_time) {}

int main(void) {
    uint8_t btn_is_pressed = 0, btn_was_pressed = 0, last_call_time = 0;

    DDRB |= (1 << LED_PIN);
    PORTB |= (1 << BUTTON_PIN);

    TCCR0B |= (1 << CS00) | (1 << CS02);

    while (1)
    {
        if (!PIN_IS_LOW(BUTTON_PIN)){
            if (TIME_DELTA(TCNT0, last_call_time) >= 20) {
                btn_is_pressed = !PIN_IS_LOW(BUTTON_PIN);
                last_call_time = TCNT0;
            }
        }

        if (btn_is_pressed && !btn_was_pressed)
            PORTB ^= (1 << LED_PIN);

        btn_was_pressed = btn_is_pressed;
    }
    
    return 0;
}