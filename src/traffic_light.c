#include <stdbool.h>

#include "../inc/light.h"
#include "../inc/adc.h"

/* Measured voltages when a light is on */
#define RED_ON_VOLTAGE = 1.3
#define YELLOW_ON_VOLTAGE = 1.2
#define GREEN_ON_VOLTAGE = 1.2
#define ON_VOLTAGE_WINDOW = 0.2

/* Measured voltages between resistor and diode when their is an open circuit from resistor to GND 
 * A margin of safety is subtracted
 */
#define RED_RES_OPEN_V = 1.89 - 0.2
#define YELLOQ_RES_OPEN_V = 1.79 - 0.2
#define GREEN_RES_OPEN_V = 1.76 - 0.2

bool _verify_diode_and_resistor(light_n light, led_color color)
{
    bool verify = false;
    float voltage;

    voltage = adc_read_light_voltage(light, color);
}

void traffic_light_increment_and_verify(light_n light, led_color initial_color)
{
    static led_color color = NONE;
	static int8_t change = 1, first_call = 1;

    if (first_call) {
        color = initial_color;
    }

    light_set_color(light, color);

	if ( color == RED ) {
		change = 1;
	} else if ( color == GREEN ) {
		change = -1;
	}

    _verify_diode_and_resistor(light, color);

    color += change;
}
