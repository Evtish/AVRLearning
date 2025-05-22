#include "defines.h"
#include "inits.h"
#include "button_controls.h"
#include "utils.h"

#include <avr/io.h>
#include <stdbool.h>

int main(void) {
    uint16_t adc_high_limit = ADC_HIGH_LIMIT_STEP * 2;

    button left_button = {L_BUTTON_PIN, false, 0, 0};
    button right_button = {R_BUTTON_PIN, false, 0, 0};

    // bool l_btn_was_pressed = false, r_btn_was_pressed = false;
    // uint8_t l_btn_last_call_time = 0, r_btn_last_call_time = 0;
    // uint16_t l_btn_debounce_amount = 0, r_btn_debounce_amount = 0;

    /* -------------------- GPIO -------------------- */
    init_LED_portb(LED_PIN);
    
    init_button_portd(L_BUTTON_PIN);
    init_button_portd(R_BUTTON_PIN);

    /* -------------------- button timer -------------------- */
    init_timer0_counter();

    /* -------------------- PWM -------------------- */
    init_timer1_PWM();

    /* -------------------- ADC -------------------- */
    init_ADC();

    while (true) {
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

        /*
            set current PWM value
            use 8 high ADC bits only due to the inaccuracy of 1-2 low bits
            change range
        */
        OCR1A = map(ADCH, 0, 255, 0, adc_high_limit);
    }
    
    return 0;
}