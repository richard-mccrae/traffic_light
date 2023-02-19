#ifndef LIGHT_H
#define LIGHT_H

#include <stdint.h>

typedef enum { 
	NONE = 0,
	RED, 
	YELLOW, 
	GREEN
} led_color;

typedef enum {
	LIGHT_0 = 0,
	LIGHT_1
} light_n;

/**
 * @brief Initialize LEDs pins
 */
void light_init(void);

/**
 * @brief Set chosen traffic light to desired color. Only one color per light can be set at any given time
 * 
 * @param[in] light_number	The traffic light number to set
 * @param[in] color	Color to set chosen traffic light
*/
void light_set_color(light_n light_number, led_color color);

/**
 * @brief Incremenent light 0 color 
 * 
 * Ordering is RED -> YELLOW -> GREEN - > YELLOW -> RED ...
*/
void light0_increment_color(void);

/**
 * @brief Incremenent light 1 color 
 * 
 * Ordering is GREEB -> YELLOW -> RED - > YELLOW -> GREEN ...
 * 
 * Safety check that GREEN can never be set if light 0 is also GREEN
*/
void light1_increment_color(void);

/**
 * @brief Cycle through all lights to test that they work as expected
 * 
 * Test for lowest level functions
*/
void test_light_low_level(void);

void test_light0_red_get(void);
void test_light0_yellow_get(void);
void test_light0_green_get(void);

void test_light1_red_get(void);
void test_light1_yellow_get(void);
void test_light1_green_get(void);

void test_light_set_color(void);

void test_light0_increment_color(void);
void test_light1_increment_color(void);

#endif /* LIGHT_H */
