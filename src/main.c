#include <MKL25Z4.h>

#include "../inc/light.h"
#include "../inc/timers.h"

int main(void) {
	/* Configure system clock to be 48 MHz */
	SystemCoreClockUpdate();

	light_init();
	// systick_init();

	light_test_low_level();
}
