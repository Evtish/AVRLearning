#include <stdint.h>
#include <stdbool.h>

typedef struct {
    uint8_t pin;
    bool was_pressed;
    uint8_t last_call_time;
    uint16_t passed_debounce_amount;
} button;