#include <avr/io.h>
#include <avr/interrupt.h>
#include "rgb.h"

void button_init()
{
  PORTC.PIN3CTRL = (1 <<7) | (1 << 3); /* Enable Pull Up & invert */
  PORTC.DIRCLR = (1 << 3); /* PC3 is an input */
}

uint8_t read_button()
{
	return (PORTC.IN >> 3) & 1;
}

void pwm_init()
{
	/* set waveform output on PORT f */
	PORTD.DIRSET = PIN5_bm | PIN6_bm | PIN7_bm;
	TCA0.SINGLE.CTRLB =// TCA_SINGLE_CMP0EN_bm /* enable compare channel 0 */
//		| TCA_SINGLE_CMP1EN_bm 
//		| TCA_SINGLE_CMP2EN_bm 
	/*	|*/ TCA_SINGLE_WGMODE_SINGLESLOPE_gc; /* set single slope PWM mode */

	/* disable event counting */
	TCA0.SINGLE.EVCTRL &= ~(TCA_SINGLE_CNTBEI_bm);

	/* set PWM frequency and duty cycle (50%) */
	TCA0.SINGLE.PERBUF = 0xff;
	TCA0.SINGLE.CTRLA = TCA_SINGLE_CLKSEL_DIV1024_gc /* set clock source
													 (sys_clk/4) */
		| TCA_SINGLE_ENABLE_bm; /* start timer */

	pwm_set(0,0,0);
}

void pwm_set(uint8_t r, uint8_t g, uint8_t b)
{
	if (!r)
		PORTD.DIRCLR = PIN5_bm;
	else
		PORTD.DIRSET = PIN5_bm;
	TCA0.SINGLE.CMP0BUF = r;
	if (!g)
		PORTD.DIRCLR = PIN6_bm;
	else
		PORTD.DIRSET = PIN6_bm;
	TCA0.SINGLE.CMP1BUF = g;
	if (!b)
		PORTD.DIRCLR = PIN7_bm;
	else
		PORTD.DIRSET = PIN7_bm;
	TCA0.SINGLE.CMP2BUF = b;
}

void pwm_output()
{
	uint8_t flags = TCA0.SINGLE.INTFLAGS;

	if (flags & 1)
		PORTD.OUTSET = PIN5_bm | PIN6_bm | PIN7_bm;
	if (flags & (1 << 4))
		if (TCA0.SINGLE.CMP0BUF != 255)
			PORTD.OUTCLR = PIN5_bm;
	if (flags & (1 << 5))
		if (TCA0.SINGLE.CMP1BUF != 255)
			PORTD.OUTCLR = PIN6_bm;
	if (flags & (1 << 6))
		if (TCA0.SINGLE.CMP2BUF != 255)
			PORTD.OUTCLR = PIN7_bm;

	TCA0.SINGLE.INTFLAGS = flags;
}
