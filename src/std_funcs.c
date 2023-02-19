#include <MKL25Z4.h>

#include "../inc/std_funcs.h"
#include "../inc/timers.h"

void delay(volatile unsigned int time_delay) {
	while ( time_delay--) {
	}
}
