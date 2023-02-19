#include <MKL25Z4.h>

#include "../inc/light.h"
#include "../inc/adc.h"
#include "../inc/std_funcs.h"

// #include "../inc/timers.h"

int main(void) 
{
	float adc_value = 0.0;

	/* Configure system clock to be 48 MHz */
	SystemCoreClockUpdate();

	light_init();
	adc_init();
	// systick_init();

	test_adc_read_light0_red();
	delay(DELAY_4_S);
	test_adc_read_light0_yellow();
	delay(DELAY_4_S);
	test_adc_read_light0_green();
}
