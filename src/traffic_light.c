#include <MKL25Z4.h>
#include <stdbool.h>
#include <stdint.h>

#include "../inc/light.h"
#include "../inc/adc.h"
#include "../inc/traffic_light.h"
#include "../inc/common.h"
#include "../inc/timers.h"

/* Measured voltage between resistor and diode when pin is high is 1.2 - 1.3 V 
 * Voltage when resistor to GND is an open circuit is 1.76 - 1.9 V
 */
#define LOWEST_ACCEPTABLE_VOLTAGE (float)1
#define HIGHEST_ACCEPTABLE_VOLTAGE (float)1.5
#define DIODE_OPEN_VOLTAGE  (float)3.1

/* LEDs on board */
#define SYSTEM_ERROR_LIGHT_BLUE (1) /* PORTD */
#define SYSTEM_ERROR_LIGHT_RED (18) /* PORTB */
#define SYSTEM_ERROR_LIGHT_GREEN (19) /* PORTB */

/* Voltage error codes */
#define LOW_VOLTAGE_ERROR   -2
#define HIGH_VOLTAGE_ERROR  -3
#define MAX_VOLTAGE_ERROR   -4

static bool system_failure_initialized = false;

/**
 * Static function declarations
*/

static void _red_toggle(void);
static void _red_off(void);
static void _red_on(void);
static void _blue_toggle(void);
static void _blue_off(void);
static void _blue_on(void);
static void _green_toggle(void);
static void _green_off(void);
static void _green_on(void);
static void _system_failure(int error);
static int _verify_diode_and_resistor_voltage(light_n light, led_color color);

/**
 * Static function definitions
*/

static void _red_on(void) {
    PTB->PCOR = MASK(SYSTEM_ERROR_LIGHT_RED);
}
static void _red_off(void) {
    PTB->PSOR = MASK(SYSTEM_ERROR_LIGHT_RED);
}
static void _red_toggle(void) {
    PTB->PTOR = MASK(SYSTEM_ERROR_LIGHT_RED);
}

static void _blue_toggle(void) {
    PTD->PTOR = MASK(SYSTEM_ERROR_LIGHT_BLUE);
}
static void _blue_on(void) {
    PTD->PCOR = MASK(SYSTEM_ERROR_LIGHT_BLUE);
}
static void _blue_off(void) {
    PTD->PSOR = MASK(SYSTEM_ERROR_LIGHT_BLUE);
}

static void _green_toggle(void) {
    PTB->PTOR = MASK(SYSTEM_ERROR_LIGHT_GREEN); 
    // | MASK(SYSTEM_ERROR_LIGHT_RED);
}
static void _green_on(void) {
    PTB->PCOR = MASK(SYSTEM_ERROR_LIGHT_GREEN); 
    // | MASK(SYSTEM_ERROR_LIGHT_RED);
}
static void _green_off(void) {
    PTB->PSOR = MASK(SYSTEM_ERROR_LIGHT_GREEN); 
    // | MASK(SYSTEM_ERROR_LIGHT_RED);
}

/**
 * @brief System failure. Stop traffic lights and raise alarm
 * 
 * Flash board's light to indicate unexpected voltage between the resistor and diode
 * Servce watchdog to prevent system reset
*/
static void _system_failure(int error)
{
    systick_deinit();
    light_set_color(LIGHT_0, NONE);
    light_set_color(LIGHT_1, NONE);

    while (1) {
        COP_WDT_service();
        
        switch (error)
        {
        case LOW_VOLTAGE_ERROR:
            _red_toggle();
            break;
        case HIGH_VOLTAGE_ERROR:
            _blue_toggle();
            break;
        case MAX_VOLTAGE_ERROR:
            _green_toggle();
            _red_toggle();
            break;
        }

        delay(DELAY_MEDIUM);
    }
}

/**
 * @brief Read ADC voltage, verify that it is within the expected range
 * 
 * @param[in] light traffic light number
 * @param[in] color color to read from
*/
static int _verify_diode_and_resistor_voltage(light_n light, led_color color)
{
    float voltage;

    voltage = adc_read_light_voltage(light, color);

    if (voltage < LOWEST_ACCEPTABLE_VOLTAGE) {
        return LOW_VOLTAGE_ERROR;
    } else if (voltage > HIGHEST_ACCEPTABLE_VOLTAGE && voltage < DIODE_OPEN_VOLTAGE) {
        return HIGH_VOLTAGE_ERROR;
    } else if (voltage >= DIODE_OPEN_VOLTAGE) {
        return MAX_VOLTAGE_ERROR;
    }

    return 0;
}

/**
 * Interface
*/

void watchdog_reset_notify(void)
{
    if (RCM->SRS0 & RCM_SRS0_WDOG_MASK) {
        for (int i = 0; i < 6; i++) {
            _red_toggle();
            delay(DELAY_SHORT);
        }
    }
}

void system_failure_alert_init(void)
{
    watchdog_reset_notify();

    /* Enable clock on PORTB and PORTD */
    SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTD_MASK;

    /* Clear PCR MUXes */
    PORTB->PCR[SYSTEM_ERROR_LIGHT_GREEN] &= ~PORT_PCR_MUX_MASK;
    PORTB->PCR[SYSTEM_ERROR_LIGHT_RED] &= ~PORT_PCR_MUX_MASK;
    PORTD->PCR[SYSTEM_ERROR_LIGHT_BLUE] &= ~PORT_PCR_MUX_MASK;

    /* Configure pins to use GPIO peripheral module */
    PORTB->PCR[SYSTEM_ERROR_LIGHT_GREEN] |= PORT_PCR_MUX(1);
    PORTB->PCR[SYSTEM_ERROR_LIGHT_RED] |= PORT_PCR_MUX(1);
    PORTD->PCR[SYSTEM_ERROR_LIGHT_BLUE] |= PORT_PCR_MUX(1);

    /* Set pins to output */
    PTB->PDDR |= MASK(SYSTEM_ERROR_LIGHT_RED) | MASK(SYSTEM_ERROR_LIGHT_GREEN);
    PTD->PDDR |= MASK(SYSTEM_ERROR_LIGHT_BLUE);

    _blue_off();
    _red_off();
    _green_off();

    system_failure_initialized = true;
}

void traffic_light0_increment_and_verify(led_color initial_color)
{
    int err;
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

    err = _verify_diode_and_resistor_voltage(LIGHT_0, color);
    if (err && system_failure_initialized) {
        _system_failure(err);
    }

    color += change;
}

void traffic_light1_increment_and_verify(led_color initial_color)
{
    int err;
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

    err = _verify_diode_and_resistor_voltage(LIGHT_1, color);
    if (err && system_failure_initialized) {
        _system_failure(err);
    }

    color += change;
}

/* *************************************  TESTS  ************************************* */

void test_traffic_light0_increment_and_verify(void)
{
    while(1) {
        traffic_light0_increment_and_verify(RED);
        delay(DELAY_MEDIUM);
    }
}

void test_traffic_light1_increment_and_verify(void)
{
    while(1) {
        traffic_light1_increment_and_verify(GREEN);
        delay(DELAY_MEDIUM);
    }
}

void test_system_lights(void)
{
    while(1) {
        _blue_on();
        delay(DELAY_MEDIUM);
        _blue_off();

        delay(DELAY_MEDIUM);

        _red_on();
        delay(DELAY_MEDIUM);
        _red_off();

        _green_on();
        delay(DELAY_MEDIUM);
        _green_off();

        delay(DELAY_MEDIUM);
    }
}
