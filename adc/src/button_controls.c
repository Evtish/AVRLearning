#include "defines.h"
#include "button_controls.h"

#include <stdint.h>
#include <stdbool.h>

/* update_button button state */
void update_button(button* btn_p) {
    bool btn_is_pressed = !PIND_IS_LOW(btn_p->pin);

    if (btn_is_pressed) {
        if (!btn_p->was_pressed)
            btn_p->passed_debounce_amount = 1;
        else if (btn_p->passed_debounce_amount >= 1)
            btn_p->passed_debounce_amount++;
    }
    else
        btn_p->passed_debounce_amount = 0;

    btn_p->was_pressed = btn_is_pressed;
}

void poll_button(button* btn_p) {
    if (TICK_DELTA(TCNT0, btn_p->last_call_time) >= TIMER_TICK_AMOUNT) {
        update_button(btn_p);
        btn_p->last_call_time = TCNT0;
    }
}