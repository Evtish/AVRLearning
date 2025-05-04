#define F_CPU 16000000U

#include <avr/io.h>
#include <util/delay.h>

#define LED_PIN _BV(PORTB5)
#define LED_DELAY_MS 1000

int main(void) {
    DDRB |= LED_PIN;
    while (1) {
        PORTB ^= LED_PIN;
        _delay_ms(LED_DELAY_MS);
    }
}