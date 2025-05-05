#include <stdint.h>
#include "../inc/manage_pwm.h"

void pwm_step(volatile uint16_t* cur_val_p, uint16_t max_val) {
    static int8_t step = -1;

    if (*cur_val_p >= max_val || *cur_val_p <= 0)
        step *= -1;

    *cur_val_p += step;
}