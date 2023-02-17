#include <MKL25Z4.h>

#include "../inc/timers.h"
#include "../inc/light.h"

#define SYSTEM_CLK 48 * 1000UL * 1000UL

void SysTick_Handler(void);

/**
 *  SysTick 
 */
void systick_init(void)
{
    SysTick->LOAD = (SYSTEM_CLK/16) - 1; /* 1 second */

    SysTick->VAL = 0;
    SysTick->CTRL = SysTick_CTRL_TICKINT_Msk /* Enable interrupt when counter reaches 0 */
                  | SysTick_CTRL_ENABLE_Msk; /* Enable SysTick Timer */

    NVIC_SetPriority(SysTick_IRQn, 3);
}

void SysTick_Handler(void)
{
    /* Change lights to next color */
    // light_increment_color_state(LIGHT_0);
    // light_increment_color_state(LIGHT_1);
    static int n = 0;

    if (n>2) {
        n=0;
    }
    light_set_color(LIGHT_0, n);
    n++;
}

/**
 * Watchdog
 */

/**
 * @brief Intialize watchdog timer
 * Use 1 kHz clock with timeout after 1024 cycles = 1.024 seconds
*/
void COP_WDT_init(void)
{
		SIM->COPC = SIM_COPC_COPT(3) /* COP timeout after 2^10 LPO cycles or 2^18 bus clock cycles */
              | SIM_COPC_COPCLKS(0) /* Internal 1 kHz clock is source to COP, alternatively can use 24 MHz bus clock */
              | SIM_COPC_COPW(0); 
}

void COP_WDT_service(void)
{
    SIM_SRVCOP = 0x55;
    SIM_SRVCOP = 0xaa;
}
