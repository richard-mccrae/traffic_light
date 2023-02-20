#include <MKL25Z4.h>

#include "../inc/light.h"
#include "../inc/adc.h"
#include "../inc/timers.h"
#include "../inc/traffic_light.h"

int main(void) 
{
	/* Configure system clock to be 48 MHz
	 * "#define CLOCK_SETUP	1" must also be placed in system_MKL25Z4.h
	 */
	SystemCoreClockUpdate();

	system_failure_alert_init();
	light_init();
	adc_init();
	systick_init();

	/* Configure wacthdog timer
	 * "#define DISABLE_WDOG 0" must also be placed in system_MKL25Z4.h
	 */
	COP_WDT_init();

	while(1);
}
