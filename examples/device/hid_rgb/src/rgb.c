#include <avr/io.h>
#include <avr/interrupt.h>
#include "rgb.h"

ISR(TCA0_OVF_vect)
{
	VPORTA.OUT |= (PIN5_bm | PIN6_bm | PIN7_bm);
	TCA0.SINGLE.INTFLAGS = 0xFF;
}

ISR(TCA0_CMP0_vect)
{
	VPORTA.OUT &= ~(PIN5_bm); 
}

ISR(TCA0_CMP1_vect)
{
	VPORTA.OUT &= ~(PIN6_bm); 
}

ISR(TCA0_CMP2_vect)
{
	VPORTA.OUT &= ~(PIN7_bm); 
}

void pwm_init()
{
	/* set waveform output on PORT A */
	PORTA.DIRSET = PIN5_bm | PIN6_bm | PIN7_bm;
	PORTMUX.TCAROUTEA = PORTMUX_TCA0_PORTA_gc;
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

#define LIMIT 0x01

void pwm_set(uint8_t r, uint8_t g, uint8_t b)
{
	if (!r)
		PORTA.DIRCLR = PIN5_bm;
	else
		PORTA.DIRSET = PIN5_bm;
	TCA0.SINGLE.CMP0BUF = r;
	if (!g)
		PORTA.DIRCLR = PIN6_bm;
	else
		PORTA.DIRSET = PIN6_bm;
	TCA0.SINGLE.CMP1BUF = g;
	if (!b)
		PORTA.DIRCLR = PIN7_bm;
	else
		PORTA.DIRSET = PIN7_bm;
	TCA0.SINGLE.CMP2BUF = b;
}

void pwm_output()
{
	uint8_t flags = TCA0.SINGLE.INTFLAGS;

	if (flags & 1)
		PORTA.OUTSET = PIN5_bm | PIN6_bm | PIN7_bm;
	if (flags & (1 << 4))
		if (TCA0.SINGLE.CMP0BUF != 255)
			PORTA.OUTCLR = PIN5_bm;
	if (flags & (1 << 5))
		if (TCA0.SINGLE.CMP1BUF != 255)
			PORTA.OUTCLR = PIN6_bm;
	if (flags & (1 << 6))
		if (TCA0.SINGLE.CMP2BUF != 255)
			PORTA.OUTCLR = PIN7_bm;

	TCA0.SINGLE.INTFLAGS = flags;
}

void adc_trigger()
{
	ADC0.COMMAND |= 1;
}

int adc_ready()
{
	/* Wait for ADC to finish */
	if (ADC0.COMMAND & 1)
		return 0;
	if (!(ADC0.INTFLAGS & (1 << 0)))
		return 0;
	return 1;

}

uint8_t adc_read()
{
	return ADC0.RESULT;
}

void adc_init()
{
	/* Set TIMEBASE */
	CLKCTRL.MCLKTIMEBASE = F_CPU / 1000000;

	/* Enable ADC0 */
	ADC0.CTRLA = 1;

	/* Prescaler set t0 4 */
	ADC0.CTRLB = 1;

	/* Reference Voltage is VDD, no setting necessary */

	/* Sample duration */
	ADC0.CTRLE = 1;

	/* MUXPOS to AN0 - AN7 (PD0 - PD7), no setting necessary */

	/* Single 8-bit conversion, no setting necessary */

}
