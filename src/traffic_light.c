#include <stdbool.h>
#include <stdint.h>

#include "../inc/light.h"
#include "../inc/adc.h"
#include "../inc/traffic_light.h"

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

void traffic_light0_increment_and_verify(led_color initial_color)
{
    static led_color color = 0;
	static int8_t change = 1;
    static bool first_call = true;

    if (first_call) {
        color = initial_color;
        first_call = false;
    }

    light_set_color(LIGHT_0, color);

	if ( color == RED ) {
		change = 1;
	} else if ( color == GREEN ) {
		change = -1;
	}

    // _verify_diode_and_resistor(light, color);

    color += change;
}

void traffic_light1_increment_and_verify(led_color initial_color)
{
    static led_color color = 0;
	static int8_t change = 1;
    static bool first_call = true;

    if (first_call) {
        color = initial_color;
        first_call = false;
    }

    light_set_color(LIGHT_1, color);

	if ( color == RED ) {
		change = 1;
	} else if ( color == GREEN ) {
		change = -1;
	}

    // _verify_diode_and_resistor(light, color);

    color += change;
}
