#include <MKL25Z4.h>

#include "../inc/light.h"
#include "../inc/timers.h"

int main(void) {
	/* Configure system clock to be 48 MHz */
	SystemCoreClockUpdate();

	light_init();
	// systick_init();

	test_light1_increment_color();
}
