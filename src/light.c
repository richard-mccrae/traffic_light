#include <MKL25Z4.h>

#include "../inc/light.h"

#define MASK(x) (1UL << (x))

#define LIGHT0_RED_SHIFT (11) /* PORTB */
#define LIGHT0_YELLOW_SHIFT (2) /* PORTE */
#define LIGHT0_GREEN_SHIFT (3) /* PORTE */

#define LIGHT1_RED_SHIFT (8) /* PORTB */
#define LIGHT1_YELLOW_SHIFT (9) /* PORTB */
#define LIGHT1_GREEN_SHIFT (10) /* PORTB */

#define DELAY_500_MS	500 * 1000U
#define DELAY_1_S	1 * 1000U * 1000U
#define DELAY_2_S	2 * 1000U * 1000U
#define DELAY_4_S	4 * 1000U * 1000U

static led_color light0_state;
static led_color light1_state;

static void _delay(volatile unsigned int time_delay) {
	while ( time_delay--) {
	}
}

/* Light 0 */
static void _light0_red_on(void) {
	PTB->PSOR = MASK(LIGHT0_RED_SHIFT);
}

static void _light0_yellow_on(void) {
	PTE->PSOR = MASK(LIGHT0_YELLOW_SHIFT);
}

static void _light0_green_on(void) {
	PTE->PSOR = MASK(LIGHT0_GREEN_SHIFT);
}

static void _light0_red_off(void) {
	PTB->PCOR = MASK(LIGHT0_RED_SHIFT);
}

static void _light0_yellow_off(void) {
	PTE->PCOR = MASK(LIGHT0_YELLOW_SHIFT);
}

static void _light0_green_off(void) {
	PTE->PCOR = MASK(LIGHT0_GREEN_SHIFT);
}

/* Light 1 */
static void _light1_red_on(void) {
	PTB->PSOR = MASK(LIGHT1_RED_SHIFT);
}

static void _light1_yellow_on(void) {
	PTB->PSOR = MASK(LIGHT1_YELLOW_SHIFT);
}

static void _light1_green_on(void) {
	PTB->PSOR = MASK(LIGHT1_GREEN_SHIFT);
}

static void _light1_red_off(void) {
	PTB->PCOR = MASK(LIGHT1_RED_SHIFT);
}

static void _light1_yellow_off(void) {
	PTB->PCOR = MASK(LIGHT1_YELLOW_SHIFT);
}

static void _light1_green_off(void) {
	PTB->PCOR = MASK(LIGHT1_GREEN_SHIFT);
}

static int _light_set_color_state(light_n light_number, led_color color)
{
	switch ( light_number ) {
	case LIGHT_0:
		light0_state = color;
		break;
	case LIGHT_1:
		light1_state = color;
		break;
	default:
		/* Invalid light number */
		return -1;
	}

	return 0;
}

static led_color _light_get_color_state(light_n light_number)
{
	led_color color;

	switch ( light_number ) {
	case LIGHT_0:
		color = light0_state;
		break;
	case LIGHT_1:
		color = light1_state;
		break;
	// What should I do here? Add ERROR color state?
	// default:
	// 	/* Invalid light number */
	}

	return (color);
}

int light_set_color(light_n light_number, led_color color) {
	switch ( light_number ) {
	case LIGHT_0:
		switch (color) {
		case RED:
			_light0_red_on();
			_light0_green_off();
			_light0_yellow_off();
			break;
		case YELLOW:
			_light0_yellow_on();
			_light0_red_off();
			_light0_green_off();
			break;
		case GREEN:
			_light0_green_on();
			_light0_red_off();
			_light0_yellow_off();
			break;
		default:
			/* Invalid color */
			return -1;
		}
		break;

	case LIGHT_1:
		switch (color) {
		case RED:
			_light1_red_on();
			_light1_green_off();
			_light1_yellow_off();
			break;
		case YELLOW:
			_light1_yellow_on();
			_light1_red_off();
			_light1_green_off();
			break;
		case GREEN:
			/* Add protection so that light can never turn green if 0 is red */
			_light1_green_on();
			_light1_red_off();
			_light1_yellow_off();
			break;
		default:
			/* Invalid color */
			return -1;
		}
		break;

	default:
		/* Invalid light number */
		return -1;
	}
	
	_light_set_color_state(light_number, color);

	return 0;
}

void light_init(void) {
	/* Enable clock on PORTB and PORTE */
	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTE_MASK;
	
	/* Clear PCR MUXes */
	PORTB->PCR[LIGHT0_RED_SHIFT] &= ~PORT_PCR_MUX_MASK;
	PORTE->PCR[LIGHT0_YELLOW_SHIFT] &= ~PORT_PCR_MUX_MASK;
	PORTE->PCR[LIGHT0_GREEN_SHIFT] &= ~PORT_PCR_MUX_MASK;

	PORTB->PCR[LIGHT1_RED_SHIFT] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[LIGHT1_YELLOW_SHIFT] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[LIGHT1_GREEN_SHIFT] &= ~PORT_PCR_MUX_MASK;

	/* Configure pins to use GPIO peripheral module */
	PORTB->PCR[LIGHT0_RED_SHIFT] |= PORT_PCR_MUX(1);
	PORTE->PCR[LIGHT0_YELLOW_SHIFT] |= PORT_PCR_MUX(1);
	PORTE->PCR[LIGHT0_GREEN_SHIFT] |= PORT_PCR_MUX(1);

	PORTB->PCR[LIGHT1_RED_SHIFT] |= PORT_PCR_MUX(1);
	PORTB->PCR[LIGHT1_YELLOW_SHIFT] |= PORT_PCR_MUX(1);
	PORTB->PCR[LIGHT1_GREEN_SHIFT] |= PORT_PCR_MUX(1);
	
	/* Set pins to be outputs */
	PTB->PDDR |= MASK(LIGHT0_RED_SHIFT);
	PTE->PDDR |= MASK(LIGHT0_YELLOW_SHIFT) | MASK(LIGHT0_GREEN_SHIFT);

	PTB->PDDR |= MASK(LIGHT1_RED_SHIFT) | MASK(LIGHT1_YELLOW_SHIFT) | MASK(LIGHT1_GREEN_SHIFT);

	/* Set initial light colors */
	// light_set_color(LIGHT_0, GREEN);
	// light_set_color(LIGHT_1, RED);
}

void light_increment_color_state(light_n light_number)
{
	led_color color;

	color = _light_get_color_state(light_number);

	/* How to deal with possible illegal enum values, how is this protected internally, during runtime, compilation? */
	if (color < 0 ) {
		/* Failure! */
	} else if (color >= 0 && color < 2) {
		light_set_color(light_number, color++);
	} else {
		light_set_color(light_number, RED);
	}
}

void light_test_low_level(void)
{
	_light0_red_off();
	_light0_yellow_off();
	_light0_green_off();
	
	_light1_red_off();
	_light1_yellow_off();
	_light1_green_off();

	_delay(DELAY_2_S);

	while(1) {
		_light0_red_off();
		_light0_yellow_off();
		_light0_green_off();
		
		_light1_red_off();
		_light1_yellow_off();
		_light1_green_off();

		_delay(DELAY_2_S);

		_light0_red_on();
		_delay(DELAY_1_S);
		_light0_yellow_on();
		_delay(DELAY_1_S);
		_light0_green_on();
		_delay(DELAY_1_S);

		_light1_red_on();
		_delay(DELAY_1_S);
		_light1_yellow_on();
		_delay(DELAY_1_S);
		_light1_green_on();
		_delay(DELAY_2_S);
	}
}
