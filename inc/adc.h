#ifndef ADC_H
#define ADC_H

/**
 * @ Initialise ADC module
 * 
 * Enable long sample time, 16-bit single-ended conversion, and use 24 MHz bus clock.
 * In addition, enable clock to input pins and configure them as analog inputs
*/
void adc_init(void);

/**
 * @brief Perform ADC conversion, further convert output value to voltage
 * @param[in] ADC channel to perform conversion
*/
float adc_read(unsigned char channel);

/**
 * Convinience functions for obtaining a specific voltage
*/
float adc_read_light0_red(void);
float adc_read_light0_yellow(void);
float adc_read_light0_green(void);
float adc_read_light1_red(void);
float adc_read_light1_yellow(void);
float adc_read_light1_green(void);

/**
 * @brief Performs ADC conversion of desired light number and color
 * @param[in] light light number
 * @param[in] color led color
*/
float adc_read_light_voltage(light_n light, led_color color);

/**
 * Tests
*/
void test_adc_read_light0_red(void);
void test_adc_read_light0_yellow(void);
void test_adc_read_light0_green(void);
void test_adc_read_light1_red(void);
void test_adc_read_light1_yellow(void);
void test_adc_read_light1_green(void);

#endif /* ADC_H */
