#ifndef _RGB_H_
#define _RGB_H_

#include <stdint.h>

void pwm_init();
void adc_init();
void adc_trigger();
int adc_ready();
uint8_t adc_read();
void pwm_set(uint8_t r, uint8_t g, uint8_t b);
void pwm_output();

#endif
