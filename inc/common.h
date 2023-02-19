#ifndef COMMON_H
#define COMMON_H

#define MASK(x) (1UL << (x))

#define DELAY_SHORT	    500 * 1000U
#define DELAY_MEDIUM	2 * 1000U * 1000U
#define DELAY_LONG	    4 * 1000U * 1000U

void delay(volatile unsigned int time_delay);

#endif /* COMMON_H */
