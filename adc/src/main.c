#include "defines.h"
#include "inits.h"
#include "button_controls.h"
#include "utils.h"

#include <stdbool.h>
#include <avr/io.h>
#include <avr/interrupt.h>

volatile bool adc_complete = false;

// #if !defined CHANGE_LIMIT_MODE
//     ISR(INT0_vect) { }
// #endif

ISR(ADC_vect) {
    adc_complete = true;
}

int main(void) {
    uint16_t adc_high_limit = ADC_HIGH_LIMIT_STEP * 2;

    #if defined CHANGE_LIMIT_MODE
        button left_button = {L_BUTTON_PIN, false, 0, 0};
        button right_button = {R_BUTTON_PIN, false, 0, 0};
    #endif

    /* -------------------- GPIO -------------------- */
    init_LED_portb(LED_PIN);

    init_button_portd(R_BUTTON_PIN);    
    #if defined CHANGE_LIMIT_MODE
        init_button_portd(L_BUTTON_PIN);

    /* -------------------- button timer -------------------- */
        init_timer0_counter();

    #else
    /* -------------------- external interrupt -------------------- */
        init_external_interrupt();
    #endif

    /* -------------------- PWM -------------------- */
    init_timer1_PWM();

    /* -------------------- ADC -------------------- */
    init_ADC();
    sei();

    while (true) {
        #if defined CHANGE_LIMIT_MODE
            poll_button(&left_button);
            poll_button(&right_button);

            if (left_button.passed_debounce_amount >= BTN_DEBOUNCE_AMOUNT_TO_PASS) {
                adc_high_limit = limit(adc_high_limit - ADC_HIGH_LIMIT_STEP, 0, 1023);
                left_button.passed_debounce_amount = 0;
            }

            if (right_button.passed_debounce_amount >= BTN_DEBOUNCE_AMOUNT_TO_PASS) {
                    adc_high_limit = limit(adc_high_limit + ADC_HIGH_LIMIT_STEP, 0, 1023);
                right_button.passed_debounce_amount = 0;
            }
        #endif

        /*
            set current PWM value
            use 8 high ADC bits only due to the inaccuracy of 1-2 low bits
            change range
        */
        if (adc_complete) {
            OCR1A = map(ADCH, 0, 255, 0, adc_high_limit);
            adc_complete = false;
        }
    }
    
    return 0;
}