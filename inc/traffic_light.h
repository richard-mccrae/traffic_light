#include "../inc/light.h"

#define TRAFFIC_LIGHT_LIGHT_0_START_COLOR   RED
#define TRAFFIC_LIGHT_LIGHT_1_START_COLOR   GREEN

/**
 * @brief Initialize start color of light or increment to next color
 * 
 * Verify that the selected color's diode and resistor are functioning
 * as expected by taking a voltage measurement between them. If the measured
 * voltage is outside of the predefined voltage window, the system will
 * raise an alarm and then shutdown.
*/
void traffic_light0_increment_and_verify(led_color initial_color);

/**
 * These are seperated into two functions due to their use of statics
*/
void traffic_light1_increment_and_verify(led_color initial_color);

/**
 * @brief Initialize failure alert system (LEDs on board)
*/
void system_failure_alert_init(void);

/**
 * Flash system lights 3 times at start-up if reset was caused by watchdog
*/
void watchdog_reset_notify(void);

/* *************************************  TESTS  ************************************* */

void test_traffic_light0_increment_and_verify(void);
void test_traffic_light1_increment_and_verify(void);

void test_system_lights(void);
