#include <MKL25Z4.h>

#include "../inc/light.h"
#include "../inc/std_funcs.h"

#define MASK(x) (1UL << (x))

#define LIGHT0_RED_SHIFT (11) /* PORTB */
#define LIGHT0_YELLOW_SHIFT (4) /* PORTE */
#define LIGHT0_GREEN_SHIFT (3) /* PORTE */

#define LIGHT1_RED_SHIFT (8) /* PORTB */
#define LIGHT1_YELLOW_SHIFT (9) /* PORTB */
#define LIGHT1_GREEN_SHIFT (10) /* PORTB */

/**
 * Local function declarations
*/

static void _light0_red_on(void);
static void _light0_yellow_on(void);
static void _light0_green_on(void);
static void _light0_red_off(void);
static void _light0_yellow_off(void);
static void _light0_green_off(void);
static uint8_t _light0_red_get(void);
static uint8_t _light0_yellow_get(void);
static uint8_t _light0_green_get(void);

static void _light1_red_on(void);
static void _light1_yellow_on(void);
static void _light1_green_on(void);
static void _light1_red_off(void);
static void _light1_yellow_off(void);
static void _light1_green_off(void);
static uint8_t _light1_red_get(void);
static uint8_t _light1_yellow_get(void);
static uint8_t _light1_green_get(void);

static void _light0_set_color(led_color color);
static void _light1_set_color(led_color color);

/**
 * Local function definitions
*/

/*
 * Light 0 low-level functions
*/
static void _light0_red_on(void)
{
	PTB->PSOR = MASK(LIGHT0_RED_SHIFT);
}

static void _light0_yellow_on(void)
{
	PTE->PSOR = MASK(LIGHT0_YELLOW_SHIFT);
}

static void _light0_green_on(void)
{
	uint8_t light1_green_pin_state;

	/* Add a safety check so that light 1 can never be set to GREEN while light 0 is also GREEN */
	light1_green_pin_state = _light1_green_get();

	if ( !light1_green_pin_state ) {
		PTE->PSOR = MASK(LIGHT0_GREEN_SHIFT);
	}

	/** TODO: Add error handling logic, function should return failure if light 1 is GREEN */
}

static void _light0_red_off(void)
{
	PTB->PCOR = MASK(LIGHT0_RED_SHIFT);
}

static void _light0_yellow_off(void)
{
	PTE->PCOR = MASK(LIGHT0_YELLOW_SHIFT);
}

static void _light0_green_off(void)
{
	PTE->PCOR = MASK(LIGHT0_GREEN_SHIFT);
}

static uint8_t _light0_red_get(void) {
	
	return ((uint8_t)((PTB->PDOR & MASK(LIGHT0_RED_SHIFT)) >> LIGHT0_RED_SHIFT));
}

static uint8_t _light0_yellow_get(void)
{
	return ((uint8_t)((PTE->PDOR & MASK(LIGHT0_YELLOW_SHIFT)) >> LIGHT0_YELLOW_SHIFT));
}

static uint8_t _light0_green_get(void)
{
	return ((uint8_t)((PTE->PDOR & MASK(LIGHT0_GREEN_SHIFT)) >> LIGHT0_GREEN_SHIFT));
}

/*
 * Light 1 low-level functions
*/
static void _light1_red_on(void)
{
	PTB->PSOR = MASK(LIGHT1_RED_SHIFT);
}

static void _light1_yellow_on(void)
{
	PTB->PSOR = MASK(LIGHT1_YELLOW_SHIFT);
}

static void _light1_green_on(void)
{
	uint8_t light0_green_pin_state;

	/* Add a safety check so that light 1 can never be set to GREEN while light 0 is also GREEN */
	light0_green_pin_state = _light0_green_get();

	if (!light0_green_pin_state) {
		PTB->PSOR = MASK(LIGHT1_GREEN_SHIFT);
	}

	/** TODO: Add error handling logic, function should return failure if light 0 is GREEN */
}

static void _light1_red_off(void)
{
	PTB->PCOR = MASK(LIGHT1_RED_SHIFT);
}

static void _light1_yellow_off(void)
{
	PTB->PCOR = MASK(LIGHT1_YELLOW_SHIFT);
}

static void _light1_green_off(void)
{
	PTB->PCOR = MASK(LIGHT1_GREEN_SHIFT);
}

static uint8_t _light1_red_get(void) {
	
	return ((uint8_t)((PTB->PDOR & MASK(LIGHT1_RED_SHIFT)) >> LIGHT1_RED_SHIFT));
}

static uint8_t _light1_yellow_get(void)
{
	return ((uint8_t)((PTB->PDOR & MASK(LIGHT1_YELLOW_SHIFT)) >> LIGHT1_YELLOW_SHIFT));
}

static uint8_t _light1_green_get(void)
{
	return ((uint8_t)((PTB->PDOR & MASK(LIGHT1_GREEN_SHIFT)) >> LIGHT1_GREEN_SHIFT));
}

/**
 * Wrapper functions
*/

/**
 * @brief Set light 0 color
 * 
 * Only one color may be set at a time
*/
static void _light0_set_color(led_color color)
{
	switch (color) {
		case NONE:
			_light0_red_off();
			_light0_green_off();
			_light0_yellow_off();
			break;
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
	}	
}

/**
 * @brief Set light 1 color
 * 
 * Only one color may be set at a time
*/
static void _light1_set_color(led_color color)
{
	switch (color) {
		case NONE:
			_light1_red_off();
			_light1_green_off();
			_light1_yellow_off();
			break;
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
			_light1_green_on();
			_light1_red_off();
			_light1_yellow_off();
			break;
	}	
}

/**
 * Interface
*/
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

}

void light_set_color(light_n light_number, led_color color)
{
	switch ( light_number ) {
		case LIGHT_0:
			_light0_set_color(color);
			break;
		case LIGHT_1:
			_light1_set_color(color);
			break;
	}
}

void light_increment_light0_color(void)
{
	static uint8_t initial_state = 1;
	static led_color color = NONE;
	static int8_t change = 1;

	color += change;
	if (!initial_state) {
		_light0_set_color(color);
	} else { /* Light 0 to begin program as RED */
		_light0_set_color(RED);
		initial_state = 0;
	}

	if ( color == RED ) {
		change = 1;
	} else if ( color == GREEN ) {
		change = -1;
	}
}

void light_increment_light1_color(void)
{
	static uint8_t initial_state = 1;
	static led_color color = NONE;
	static int8_t change = 1;

	color += change;
	if (!initial_state) {
		_light1_set_color(color);
	} else { /* Light 1 to begin program as GREEN */
		_light1_set_color(GREEN);
		color = GREEN;
		initial_state = 0;
	}

	if ( color <= RED ) {
		change = 1;
	} else if ( color >= GREEN ) {
		change = -1;
	}
}


/* *************************************  TESTS  ************************************* */
/** 
 * low level set and clear functions
 */
void test_light_low_level(void)
{
	_light0_red_off();
	_light0_yellow_off();
	_light0_green_off();
	
	_light1_red_off();
	_light1_yellow_off();
	_light1_green_off();

	delay(DELAY_2_S);

	while(1) {
		_light0_red_off();
		_light0_yellow_off();
		_light0_green_off();
		
		_light1_red_off();
		_light1_yellow_off();
		_light1_green_off();

		delay(DELAY_2_S);

		_light0_red_on();
		delay(DELAY_1_S);
		_light0_yellow_on();
		delay(DELAY_1_S);
		_light0_green_on();
		delay(DELAY_1_S);

		_light1_red_on();
		delay(DELAY_1_S);
		_light1_yellow_on();
		delay(DELAY_1_S);
		_light1_green_on();
		delay(DELAY_2_S);
	}
}

/**
 *  Test light 0 get functions
 */
void test_light0_red_get(void)
{
	volatile uint8_t red_pin_state = 8;

	/* clear light, expect PDOR to be 0 */
	_light0_red_off();
	red_pin_state = _light0_red_get();

	if (red_pin_state == 0) {
		_light1_green_on();
	} else if (red_pin_state == 1) {
		_light1_red_on();
	} else {
		_light1_yellow_on();
	}

	delay(DELAY_4_S);

	/* Reset lightd and var */
	_light1_red_off();
	_light1_yellow_off();
	_light1_green_off();
	red_pin_state = 8;

	delay(DELAY_2_S);
	
	/* Set light, expect PDOR to be 1 */
	_light0_red_on();
	delay(DELAY_1_S);
	red_pin_state = _light0_red_get();

	if (red_pin_state == 1) {
		_light1_green_on();
	} else if (red_pin_state == 0) {
		_light1_red_on();
	} else {
		_light1_yellow_on();
	}

	delay(DELAY_4_S);
	_light0_red_off();
	_light1_red_off();
	_light1_yellow_off();
	_light1_green_off();
	delay(DELAY_4_S);
}

void test_light0_yellow_get(void)
{
	volatile uint8_t yellow_pin_state = 8;

	/* clear light, expect PDOR to be 0 */
	_light0_yellow_off();
	yellow_pin_state = _light0_yellow_get();

	if (yellow_pin_state == 0) {
		_light1_green_on();
	} else if (yellow_pin_state == 1) {
		_light1_red_on();
	} else {
		_light1_yellow_on();
	}

	delay(DELAY_4_S);

	/* Reset lightd and var */
	_light1_red_off();
	_light1_yellow_off();
	_light1_green_off();
	yellow_pin_state = 8;

	delay(DELAY_2_S);
	
	/* Set light, expect PDOR to be 1 */
	_light0_yellow_on();
	delay(DELAY_1_S);
	yellow_pin_state = _light0_yellow_get();

	if (yellow_pin_state == 1) {
		_light1_green_on();
	} else if (yellow_pin_state == 0) {
		_light1_red_on();
	} else {
		_light1_yellow_on();
	}

	delay(DELAY_4_S);
	_light0_yellow_off();
	_light1_red_off();
	_light1_yellow_off();
	_light1_green_off();
	delay(DELAY_4_S);
}

void test_light0_green_get(void)
{
	volatile uint8_t green_pin_state = 8;

	/* clear light, expect PDOR to be 0 */
	_light0_green_off();
	green_pin_state = _light0_green_get();

	if (green_pin_state == 0) {
		_light1_green_on();
	} else if (green_pin_state == 1) {
		_light1_red_on();
	} else {
		_light1_yellow_on();
	}

	delay(DELAY_4_S);

	/* Reset lightd and var */
	_light1_red_off();
	_light1_yellow_off();
	_light1_green_off();
	green_pin_state = 8;

	delay(DELAY_2_S);
	
	/* Set light, expect PDOR to be 1 */
	_light0_green_on();
	delay(DELAY_1_S);
	green_pin_state = _light0_green_get();

	if (green_pin_state == 1) {
		_light1_green_on();
	} else if (green_pin_state == 0) {
		_light1_red_on();
	} else {
		_light1_yellow_on();
	}

	delay(DELAY_4_S);
	_light0_green_off();
	_light1_red_off();
	_light1_yellow_off();
	_light1_green_off();
	delay(DELAY_4_S);
}

/**
 *  Test light 1 get functions
 */
void test_light1_red_get(void)
{
	volatile uint8_t red_pin_state = 8;

	/* clear light, expect PDOR to be 0 */
	_light0_red_off();
	red_pin_state = _light1_red_get();

	if (red_pin_state == 0) {
		_light0_green_on();
	} else if (red_pin_state == 1) {
		_light0_red_on();
	} else {
		_light0_yellow_on();
	}

	delay(DELAY_4_S);

	/* Reset lightd and var */
	_light0_red_off();
	_light0_yellow_off();
	_light0_green_off();
	red_pin_state = 8;

	delay(DELAY_2_S);
	
	/* Set light, expect PDOR to be 1 */
	_light1_red_on();
	delay(DELAY_1_S);
	red_pin_state = _light1_red_get();

	if (red_pin_state == 1) {
		_light0_green_on();
	} else if (red_pin_state == 0) {
		_light0_red_on();
	} else {
		_light0_yellow_on();
	}

	delay(DELAY_4_S);
	_light1_red_off();
	_light0_red_off();
	_light0_yellow_off();
	_light0_green_off();
	delay(DELAY_4_S);
}

void test_light1_yellow_get(void)
{
	volatile uint8_t yellow_pin_state = 8;

	/* clear light, expect PDOR to be 0 */
	_light1_yellow_off();
	yellow_pin_state = _light1_yellow_get();

	if (yellow_pin_state == 0) {
		_light0_green_on();
	} else if (yellow_pin_state == 1) {
		_light0_red_on();
	} else {
		_light0_yellow_on();
	}

	delay(DELAY_4_S);

	/* Reset lightd and var */
	_light0_red_off();
	_light0_yellow_off();
	_light0_green_off();
	yellow_pin_state = 8;

	delay(DELAY_2_S);
	
	/* Set light, expect PDOR to be 1 */
	_light1_yellow_on();
	delay(DELAY_1_S);
	yellow_pin_state = _light1_yellow_get();

	if (yellow_pin_state == 1) {
		_light0_green_on();
	} else if (yellow_pin_state == 0) {
		_light0_red_on();
	} else {
		_light0_yellow_on();
	}

	delay(DELAY_4_S);
	_light1_yellow_off();
	_light0_red_off();
	_light0_yellow_off();
	_light0_green_off();
	delay(DELAY_4_S);
}

void test_light1_green_get(void)
{
	volatile uint8_t green_pin_state = 8;

	/* clear light, expect PDOR to be 0 */
	_light1_green_off();
	green_pin_state = _light1_green_get();

	if (green_pin_state == 0) {
		_light0_green_on();
	} else if (green_pin_state == 1) {
		_light0_red_on();
	} else {
		_light0_yellow_on();
	}

	delay(DELAY_4_S);

	/* Reset lightd and var */
	_light0_red_off();
	_light0_yellow_off();
	_light0_green_off();
	green_pin_state = 8;

	delay(DELAY_2_S);
	
	/* Set light, expect PDOR to be 1 */
	_light1_green_on();
	delay(DELAY_1_S);
	green_pin_state = _light1_green_get();

	if (green_pin_state == 1) {
		_light0_green_on();
	} else if (green_pin_state == 0) {
		_light0_red_on();
	} else {
		_light0_yellow_on();
	}

	delay(DELAY_4_S);
	_light1_green_off();
	_light0_red_off();
	_light0_yellow_off();
	_light0_green_off();
	delay(DELAY_4_S);
}

/**
 * Test wrapper and higher level functions
*/
void test_light_set_color(void)
{
	delay(DELAY_2_S);
	light_set_color(LIGHT_0, RED);
	delay(DELAY_2_S);
	light_set_color(LIGHT_0, YELLOW);
	delay(DELAY_2_S);
	light_set_color(LIGHT_0, GREEN);
	delay(DELAY_2_S);
	light_set_color(LIGHT_0, NONE);

	delay(DELAY_4_S);

	light_set_color(LIGHT_1, RED);
	delay(DELAY_2_S);
	light_set_color(LIGHT_1, YELLOW);
	delay(DELAY_2_S);
	light_set_color(LIGHT_1, GREEN);
	delay(DELAY_2_S);
	light_set_color(LIGHT_1, NONE);
}

void test_light_increment_light0_color(void)
{
	while(1) {
		delay(DELAY_4_S);
		light_increment_light0_color();
	}
}

void test_light_increment_light1_color(void)
{
	while(1) {
		delay(DELAY_4_S);
		light_increment_light1_color();
	}
}

void test_both_lights_increment_color(void)
{
	while(1) {
		delay(DELAY_4_S);
		light_increment_light0_color();
		light_increment_light1_color();
	}
}
