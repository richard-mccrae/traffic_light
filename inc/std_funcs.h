#ifndef STDFUNCS_H
#define STDFUNCS_H

#define MASK(x) (1UL << (x))

#define DELAY_250_US    250
#define DELAY_500_US    500
#define DELAY_1_MS      1 * 1000U
#define DELAY_250_MS	250 * 1000U
#define DELAY_500_MS	500 * 1000U
#define DELAY_1_S	1 * 1000U * 1000U
#define DELAY_2_S	2 * 1000U * 1000U
#define DELAY_4_S	4 * 1000U * 1000U

void delay(volatile unsigned int time_delay);

#endif /* STDFUNCS_H */
