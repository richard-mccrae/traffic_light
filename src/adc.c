#include <MKL25Z4.h>

#include "../inc/adc.h"
#include "../inc/light.h"
#include "../inc/common.h"

#define ADC_IN_LIGHT0_RED (0) /* PORTB */
#define ADC_IN_LIGHT0_YELLOW (1) /* PORTB */
#define ADC_IN_LIGHT0_GREEN (2) /* PORTB */

#define ADC_IN_LIGHT1_RED (3) /* PORTB */
#define ADC_IN_LIGHT1_YELLOW (2) /* PORTC */
#define ADC_IN_LIGHT1_GREEN (1) /* PORTC */

#define ADC_REFSEL_LIGHT0_RED (8)
#define ADC_REFSEL_LIGHT0_YELLOW (9)
#define ADC_REFSEL_LIGHT0_GREEN (12)

#define ADC_REFSEL_LIGHT1_RED (13)
#define ADC_REFSEL_LIGHT1_YELLOW (11)
#define ADC_REFSEL_LIGHT1_GREEN (15)

/**
 * Static function declarations
*/

static unsigned short _get_conversion(unsigned char ch);
static float _convert_output_to_voltage(unsigned short adc_output);

/**
 * Static function definitions
*/

/*unsigned short    adc_read(unsigned char ch)
 *
 *
 *     Reads the specified adc channel and returns the 16 bits read value
 *     
 *     ch -> Number of the channel in which the reading will be performed
 *     Returns the -> Result of the conversion performed by the adc
 *
 * (!) This function stolen with good conscious from Manuel Rodríguez @ NXP Community
 * https://community.nxp.com/t5/Kinetis-Microcontrollers/PIT-ADC-DMA-Example-for-FRDM-KL25z-FRDM-K64F-TWR-K60D100-and-TWR/ta-p/1107325 
 */
static unsigned short _get_conversion(unsigned char ch)
{
    ADC0_SC1A = (ch & ADC_SC1_ADCH_MASK) | 
                (ADC0_SC1A & (ADC_SC1_AIEN_MASK | ADC_SC1_DIFF_MASK));     // Write to SC1A to start conversion
    while(ADC0_SC2 & ADC_SC2_ADACT_MASK);      // Conversion in progress
    while(!(ADC0_SC1A & ADC_SC1_COCO_MASK)); // Run until the conversion is complete

    return ((unsigned short)ADC0_RA);
}

/**
 * @brief Convert ADC output value to voltage based on 3v3 reference
 * @params[in] ADC output value
*/
static float _convert_output_to_voltage(unsigned short adc_output)
{
    unsigned short resolution = 65535;
    float reference_voltage = (float)3.3;


    return (((float)adc_output/resolution) * reference_voltage);
}

/**
 * Interface
*/

void adc_init(void)
{
    /* Enable clock to ADC, PORTB, and PORTC */
    SIM->SCGC6 |= SIM_SCGC6_ADC0_MASK;
    SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTC_MASK;

    /* Clear PCR MUX for ADC input pins */
    PORTB->PCR[ADC_IN_LIGHT0_RED] &= ~PORT_PCR_MUX_MASK;
    PORTB->PCR[ADC_IN_LIGHT0_YELLOW] &= ~PORT_PCR_MUX_MASK;
    PORTB->PCR[ADC_IN_LIGHT0_GREEN] &= ~PORT_PCR_MUX_MASK;

    PORTB->PCR[ADC_IN_LIGHT1_RED] &= ~PORT_PCR_MUX_MASK;
    PORTE->PCR[ADC_IN_LIGHT1_YELLOW] &= ~PORT_PCR_MUX_MASK;
    PORTE->PCR[ADC_IN_LIGHT1_GREEN] &= ~PORT_PCR_MUX_MASK;

    /* Set pins to analog input */
    PORTB->PCR[ADC_IN_LIGHT0_RED] |= PORT_PCR_MUX(0);
    PORTB->PCR[ADC_IN_LIGHT0_YELLOW] |= PORT_PCR_MUX(0);
    PORTB->PCR[ADC_IN_LIGHT0_GREEN] |= PORT_PCR_MUX(0);

    PORTB->PCR[ADC_IN_LIGHT0_RED] |= PORT_PCR_MUX(0);
    PORTE->PCR[ADC_IN_LIGHT0_YELLOW] |= PORT_PCR_MUX(0);
    PORTE->PCR[ADC_IN_LIGHT0_GREEN] |= PORT_PCR_MUX(0);

    /* ADC configurations */
    /** TODO: Read up on ADC configurations */
    ADC0->CFG1 = 0;
    ADC0->CFG1 |= 
        (ADC_CFG1_ADLSMP_MASK |     /* long sample time */
        ADC_CFG1_MODE(3) |         /* 16 bit single-ended conversion */
        ADC_CFG1_ADICLK(0));     /* Input Bus Clock (20-25 MHz out of reset (FEI mode)) */

    /* Default voltage reference pin pair, that is, external pins VREFH and VREFL 
     * Software trigger select
     */
    ADC0->SC2 = ADC_SC2_REFSEL(0);
}

float adc_read(unsigned char channel)
{
    unsigned short adc_output;

    adc_output = _get_conversion(channel);

    return (_convert_output_to_voltage(adc_output));
}

/**
 * Wrapper functions for triggering conversions from each input channel
*/

float adc_read_light0_red(void)
{
    return (adc_read(ADC_REFSEL_LIGHT0_RED));
}

float adc_read_light0_yellow(void)
{
    return (adc_read(ADC_REFSEL_LIGHT0_YELLOW));
}

float adc_read_light0_green(void)
{
    return (adc_read(ADC_REFSEL_LIGHT0_GREEN));
}

float adc_read_light1_red(void)
{
    return (adc_read(ADC_REFSEL_LIGHT1_RED));
}

float adc_read_light1_yellow(void)
{
    return (adc_read(ADC_REFSEL_LIGHT1_YELLOW));
}

float adc_read_light1_green(void)
{
    return (adc_read(ADC_REFSEL_LIGHT1_GREEN));
}

float adc_read_light_voltage(light_n light, led_color color)
{
    float voltage = (float)-1;

    switch (light)
    {
    case LIGHT_0:
        switch (color)
        {
        case RED:
            voltage = adc_read_light0_red();
            break;
        case YELLOW:
            voltage = adc_read_light0_yellow();
            break;
        case GREEN:
            voltage = adc_read_light0_green();
            break;
        case NONE:
            voltage = (float)-1;
            break;
        }
        break;
    case LIGHT_1:
        switch (color)
        {
        case RED:
            voltage = adc_read_light1_red();
            break;
        case YELLOW:
            voltage = adc_read_light1_yellow();
            break;
        case GREEN:
            voltage = adc_read_light1_green();
            break;
        case NONE:
            voltage = (float)-1;
            break;
        }
        break;
    }

    return voltage;
}

/* *************************************  TESTS  ************************************* */

void test_adc_read_light0_red(void)
{
    float voltage;

    /* Measure voltage with pin off, should be 0 */
    voltage = adc_read_light0_red();

    if (voltage < (float)1) {
        light_set_color(LIGHT_1, GREEN);
    } else if ( voltage >= (float)1.0 && voltage < (float)1.55) {
        light_set_color(LIGHT_1, RED);
    } else {
        light_set_color(LIGHT_1, YELLOW);
    }

    delay(DELAY_LONG);
    light_set_color(LIGHT_1, NONE);
    delay(DELAY_LONG);

    /* Turn on LIGHT_0 RED, test ADC voltage again, should be aroudn 1.3 V */
    light_set_color(LIGHT_0, RED);
    voltage = adc_read_light0_red();
    
    if (voltage < (float)1) {
        light_set_color(LIGHT_1, RED);
    } else if ( voltage >= (float)1 && voltage < (float)1.55) {
        light_set_color(LIGHT_1, GREEN);
    } else {
        light_set_color(LIGHT_1, YELLOW);
    }

    delay(DELAY_LONG);
    light_set_color(LIGHT_0, NONE);
    light_set_color(LIGHT_1, NONE);
}

void test_adc_read_light0_yellow(void)
{
    float voltage;

    /* Measure voltage with pin off, should be 0 */
    voltage = adc_read_light0_yellow();

    if (voltage < (float)1) {
        light_set_color(LIGHT_1, GREEN);
    } else if ( voltage >= (float)1 && voltage < (float)1.55) {
        light_set_color(LIGHT_1, RED);
    } else {
        light_set_color(LIGHT_1, YELLOW);
    }

    delay(DELAY_LONG);
    light_set_color(LIGHT_1, NONE);
    delay(DELAY_LONG);

    /* Turn on LIGHT_0 YELLOW, test ADC voltage again, should be aroudn 1.3 V */
    light_set_color(LIGHT_0, YELLOW);
    voltage = adc_read_light0_yellow();

    if (voltage < (float)1) {
        light_set_color(LIGHT_1, RED);
    } else if ( voltage >= (float)1 && voltage < (float)1.55) {
        light_set_color(LIGHT_1, GREEN);
    } else {
        light_set_color(LIGHT_1, YELLOW);
    }

    delay(DELAY_LONG);
    light_set_color(LIGHT_0, NONE);
    light_set_color(LIGHT_1, NONE);
}

void test_adc_read_light0_green(void)
{
    float voltage;

    /* Measure voltage with pin off, should be 0 */
    voltage = adc_read_light0_green();

    if (voltage < (float)1) {
        light_set_color(LIGHT_1, GREEN);
    } else if ( voltage >= (float)1.0 && voltage < (float)1.55) {
        light_set_color(LIGHT_1, RED);
    } else {
        light_set_color(LIGHT_1, YELLOW);
    }

    delay(DELAY_LONG);
    light_set_color(LIGHT_1, NONE);
    delay(DELAY_LONG);

    /* Turn on LIGHT_0 GREEN, test ADC voltage again, should be aroudn 1.3 V */
    light_set_color(LIGHT_0, GREEN);
    voltage = adc_read_light0_green();

    if (voltage < (float)1) {
        light_set_color(LIGHT_1, RED);
    } else if ( voltage >= (float)1 && voltage < (float)1.55) {
        /* Oi! There is a low-level protection to prevent two green lights */
        light_set_color(LIGHT_1, GREEN);
    } else {
        light_set_color(LIGHT_1, YELLOW);
    }

    delay(DELAY_LONG);
    light_set_color(LIGHT_0, NONE);
    light_set_color(LIGHT_1, NONE);
}

void test_adc_read_light1_red(void)
{
    float voltage;

    /* Measure voltage with pin off, should be 1 */
    voltage = adc_read_light1_red();

    if (voltage < (float)1) {
        light_set_color(LIGHT_0, GREEN);
    } else if ( voltage >= (float)1.0 && voltage < (float)1.55) {
        light_set_color(LIGHT_0, RED);
    } else {
        light_set_color(LIGHT_0, YELLOW);
    }

    delay(DELAY_LONG);
    light_set_color(LIGHT_0, NONE);
    delay(DELAY_LONG);

    /* Turn on LIGHT_0 RED, test ADC voltage again, should be aroudn 1.3 V */
    light_set_color(LIGHT_1, RED);
    voltage = adc_read_light1_red();

    if (voltage < (float)1) {
        light_set_color(LIGHT_0, RED);
    } else if ( voltage >= (float)1 && voltage < (float)1.55) {
        light_set_color(LIGHT_0, GREEN);
    } else {
        light_set_color(LIGHT_0, YELLOW);
    }

    delay(DELAY_LONG);
    light_set_color(LIGHT_0, NONE);
    light_set_color(LIGHT_1, NONE);
}

void test_adc_read_light1_yellow(void)
{
    float voltage;

    /* Measure voltage with pin off, should be 0 */
    voltage = adc_read_light1_yellow();

    if (voltage < (float)1) {
        light_set_color(LIGHT_0, GREEN);
    } else if ( voltage >= (float)1 && voltage < (float)1.55) {
        light_set_color(LIGHT_0, RED);
    } else {
        light_set_color(LIGHT_0, YELLOW);
    }

    delay(DELAY_LONG);
    light_set_color(LIGHT_0, NONE);
    delay(DELAY_LONG);

    /* Turn on LIGHT_0 YELLOW, test ADC voltage again, should be aroudn 1.3 V */
    light_set_color(LIGHT_1, YELLOW);
    voltage = adc_read_light1_yellow();

    if (voltage < (float)1) {
        light_set_color(LIGHT_0, RED);
    } else if ( voltage >= (float)1 && voltage < (float)1.55) {
        light_set_color(LIGHT_0, GREEN);
    } else {
        light_set_color(LIGHT_0, YELLOW);
    }

    delay(DELAY_LONG);
    light_set_color(LIGHT_0, NONE);
    light_set_color(LIGHT_1, NONE);
}

void test_adc_read_light1_green(void)
{
    float voltage;

    /* Measure voltage with pin off, should be 0 */
    voltage = adc_read_light1_green();

    if (voltage < (float)1) {
        light_set_color(LIGHT_0, GREEN);
    } else if ( voltage >= (float)1 && voltage < (float)1.55) {
        light_set_color(LIGHT_0, RED);
    } else {
        light_set_color(LIGHT_0, YELLOW);
    }

    delay(DELAY_LONG);
    light_set_color(LIGHT_0, NONE);
    delay(DELAY_LONG);

    /* Turn on LIGHT_0 GREEN, test ADC voltage again, should be aroudn 1.3 V */
    light_set_color(LIGHT_1, GREEN);
    voltage = adc_read_light1_green();

    if (voltage < (float)1) {
        light_set_color(LIGHT_0, RED);
    } else if ( voltage >= (float)1 && voltage < (float)1.55) {
        /* Oi! There is a low-level protection to prevent two green lights */
        light_set_color(LIGHT_0, GREEN);
    } else {
        light_set_color(LIGHT_0, YELLOW);
    }

    delay(DELAY_LONG);
    light_set_color(LIGHT_0, NONE);
    light_set_color(LIGHT_1, NONE);
}
