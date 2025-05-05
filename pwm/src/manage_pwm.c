#include <stdint.h>
#include "../inc/manage_pwm.h"

// adjust PWM output by one
void pwm_step(volatile uint16_t* cur_val_p, uint16_t max_val) {
    static int8_t step = -1;

    // change adjusting direction if PWM has reached its limit
    if (*cur_val_p >= max_val || *cur_val_p <= 0)
        step *= -1;

    // apply the adjustment
    *cur_val_p += step;
}