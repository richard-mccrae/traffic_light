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

void test_adc_read_light0_red(void);
void test_adc_read_light0_yellow(void);
void test_adc_read_light0_green(void);

#endif /* ADC_H */
