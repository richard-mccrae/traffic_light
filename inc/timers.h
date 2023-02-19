#ifndef TIMERS_H
#define TIMERS_H

/**
 * @brief Initialize 1 second interrupt
*/
void systick_init(void);

/**
 * @brief De-initialize SysTick
*/
void systick_deinit(void);

/**
 * @brief Initiail a watchdog timer of just over 1 second
*/
void COP_WDT_init(void);

/**
 * @brief Feed the beast
*/
void COP_WDT_service(void);

#endif /* TIMERS_H */
