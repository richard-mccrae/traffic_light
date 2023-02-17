#include <MKL25Z4.h>
#include "../inc/light.h"

#define MASK(x) (1UL << (x))

#define LIGHT0_RED_SHIFT (18) /* PORTB */
#define LIGHT0_GREEN_SHIFT (19) /* PORTB */
#define LIGHT0_BLUE_SHIFT (1) /* PORTD */

#define LIGHT1_RED_SHIFT (5) /* PORTB */
#define LIGHT1_GREEN_SHIFT (6) /* PORTB */
#define LIGHT1_BLUE_SHIFT (7) /* PORTD */

static led_color light0_state;
static led_color light1_state;

/* Light 0 */
static void _light0_red_on(void) {
	PTB->PCOR = MASK(LIGHT0_RED_SHIFT);
}

static void _light0_green_on(void) {
	PTB->PCOR = MASK(LIGHT0_GREEN_SHIFT);
}

static void _light0_blue_on(void) {
	PTD->PCOR = MASK(LIGHT0_BLUE_SHIFT);
}

static void _light0_red_off(void) {
	PTB->PSOR = MASK(LIGHT0_RED_SHIFT);
}

static void _light0_green_off(void) {
	PTB->PSOR = MASK(LIGHT0_GREEN_SHIFT);
}

static void _light0_blue_off(void) {
	PTB->PSOR = MASK(LIGHT0_BLUE_SHIFT);
}

/* Light 1 */
static void _light1_red_on(void) {
	PTB->PCOR = MASK(LIGHT1_RED_SHIFT);
}

static void _light1_green_on(void) {
	PTB->PCOR = MASK(LIGHT1_GREEN_SHIFT);
}

static void _light1_blue_on(void) {
	PTD->PCOR = MASK(LIGHT1_BLUE_SHIFT);
}

static void _light1_red_off(void) {
	PTB->PSOR = MASK(LIGHT1_RED_SHIFT);
}

static void _light1_green_off(void) {
	PTB->PSOR = MASK(LIGHT1_GREEN_SHIFT);
}

static void _light1_blue_off(void) {
	PTB->PSOR = MASK(LIGHT1_BLUE_SHIFT);
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
			_light0_blue_off();
			break;
		case BLUE:
			_light0_blue_on();
			_light0_red_off();
			_light0_green_off();
			break;
		case GREEN:
			_light0_green_on();
			_light0_red_off();
			_light0_blue_off();
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
			_light1_blue_off();
			break;
		case BLUE:
			_light1_blue_on();
			_light1_red_off();
			_light1_green_off();
			break;
		case GREEN:
			/* Add protection so that light can never turn green if 0 is red */
			_light1_green_on();
			_light1_red_off();
			_light1_blue_off();
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
	/* Enable clock on PORTB and PORTD */
	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTD_MASK;
	
	/* Clear port PCR */
	PORTB->PCR[LIGHT0_RED_SHIFT] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[LIGHT0_GREEN_SHIFT] &= ~PORT_PCR_MUX_MASK;
	PORTD->PCR[LIGHT0_BLUE_SHIFT] &= ~PORT_PCR_MUX_MASK;

	// PORTB->PCR[LIGHT1_RED_SHIFT] &= ~PORT_PCR_MUX_MASK;
	// PORTB->PCR[LIGHT1_GREEN_SHIFT] &= ~PORT_PCR_MUX_MASK;
	// PORTD->PCR[LIGHT1_BLUE_SHIFT] &= ~PORT_PCR_MUX_MASK;

	/* Configure pins to use GPIO peripheral module */
	PORTB->PCR[LIGHT0_RED_SHIFT] |= PORT_PCR_MUX(1);
	PORTB->PCR[LIGHT0_GREEN_SHIFT] |= PORT_PCR_MUX(1);
	PORTD->PCR[LIGHT0_BLUE_SHIFT] |= PORT_PCR_MUX(1);

	// PORTB->PCR[LIGHT1_RED_SHIFT] |= PORT_PCR_MUX(1);
	// PORTB->PCR[LIGHT1_GREEN_SHIFT] |= PORT_PCR_MUX(1);
	// PORTD->PCR[LIGHT1_BLUE_SHIFT] |= PORT_PCR_MUX(1);
	
	/* Set LED pins to be outputs */
	PTB->PDDR |= MASK(LIGHT0_RED_SHIFT) | MASK(LIGHT0_GREEN_SHIFT);
	PTD->PDDR |= MASK(LIGHT0_BLUE_SHIFT);

	// PTB->PDDR |= MASK(LIGHT1_RED_SHIFT) | MASK(LIGHT1_GREEN_SHIFT);
	// PTD->PDDR |= MASK(LIGHT1_BLUE_SHIFT);

	/* Set initial light colors */
	light_set_color(LIGHT_0, GREEN);
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
