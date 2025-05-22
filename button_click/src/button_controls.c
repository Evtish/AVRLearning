#include "defines.h"
#include "button_controls.h"
#include <stdint.h>
#include <stdbool.h>

/* update_button button state */
void update_button(uint8_t btn_pin, volatile uint16_t* passed_debounce_amount_p) {
    static bool btn_was_pressed = false;
    bool btn_is_pressed = !PIND_IS_LOW(btn_pin);

    if (btn_is_pressed) {
        if (!btn_was_pressed)
            *passed_debounce_amount_p = 1;
        else if (*passed_debounce_amount_p >= 1)
            (*passed_debounce_amount_p)++;
    }
    else
        *passed_debounce_amount_p = 0;

    btn_was_pressed = btn_is_pressed;
}