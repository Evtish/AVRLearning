#include "defines.h"
#include "controls.h"
#include "inits.h"

#include <stdbool.h>
#include <stdint.h>
#include <avr/interrupt.h>

volatile uint16_t passed_debounce_amount = 0;

#if defined TIMER0_CTC
    /* CTC interrupt (when counter == match value) */
    ISR(TIMER0_COMPA_vect) {
        update(BUTTON_PIN, &passed_debounce_amount);
    }
#elif defined PCINT23_EXTERNAL_INTR
    /* pin change interrupt (when PCINT pin state has changed) */
    ISR(PCINT2_vect) {
        if (!PIND_IS_LOW(BUTTON_PIN))
            TCCR0B = TIMER_PRESCALER;
    }

    ISR(TIMER0_COMPA_vect) {
        if (!PIND_IS_LOW(BUTTON_PIN))
            passed_debounce_amount++;
        else {
            TCCR0B = 0;
            passed_debounce_amount = 0;
        }

        if (passed_debounce_amount >= BTN_DEBOUNCE_AMOUNT_TO_PASS)
            TCCR0B = 0;
        }
#endif

int main(void) {
    init_LED_PORTB();
    init_button_PORTD();    
    
    #if defined TIMER0_CTC
        init_TIMER0_CTC_interrupts();
        sei();  // allow interrupts
    #elif defined PCINT23_EXTERNAL_INTR
        init_PCINT23_external_interrupts();
        sei();
    #elif defined TIMER0_COUNTER
        uint8_t last_call_time = 0;
        init_TIMER0_counter();
    #endif
    

    while (true) {
        #if defined TIMER0_COUNTER
            /* if some time has passed since the last time */
            if (TICK_DELTA(TCNT0, last_call_time) >= TIMER_TICK_AMOUNT) {
                update(BUTTON_PIN, &passed_debounce_amount);
                last_call_time = TCNT0;
            }
        #endif

        if (passed_debounce_amount >= BTN_DEBOUNCE_AMOUNT_TO_PASS) {
            PORTB ^= (1 << LED_PIN);  // toggle pin
            passed_debounce_amount = 0;
        }
    }
    
    return 0;
}