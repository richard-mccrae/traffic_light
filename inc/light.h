#ifndef LIGHT_H
#define LIGHT_H

#include <stdint.h>

typedef enum { 
	RED = 0, 
	BLUE, 
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
 * @brief Set chosen traffic light to desired color
 * @param[in] light_number	The traffic light number to set
 * @param[in] color	Color to set chosen traffic light
 * @return 0 on success, -1 otherwise
*/
int light_set_color(light_n light_number, led_color color);

/**
 * @brief Change chosen traffic light to its next stage
 * @param[in] light_number The traffic light number to change
*/
void light_increment_color_state(light_n light_number);

#endif /* LIGHT_H */
