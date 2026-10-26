#ifndef _RGB_H_
#define _RGB_H_

#include <stdint.h>

void pwm_init();
void pwm_set(uint8_t r, uint8_t g, uint8_t b);
void pwm_output();

uint8_t read_button();
void button_init();

#endif
