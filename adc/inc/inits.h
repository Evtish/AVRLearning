#include <stdint.h>

void init_LED_portb(uint8_t);
void init_button_portd(uint8_t);
#if defined CHANGE_LIMIT_MODE
    void init_timer0_counter(void);
#else
    void init_external_interrupt(void);
#endif
void init_timer1_PWM(void);
void init_ADC(void);