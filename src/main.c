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
	// adc_init();
	// systick_init();

	while(1) {
		light_set_color(LIGHT_0, YELLOW);
		delay(DELAY_4_S);
		light_set_color(LIGHT_0, GREEN);
		delay(DELAY_4_S);
	}
}
