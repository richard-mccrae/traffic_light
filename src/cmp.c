#include <MKL25Z4.h>
#include <stdbool.h>

#include "../inc/cmp.h"
#include "../inc/light.h"
#include "../inc/std_funcs.h"

#define CMP_COUT (CMP0->SCR & CMP_SCR_COUT_MASK)

void CMP0_IRQHandler(void);

void cmp_init(bool irq)
{	
	/* Enable clock to comparator and enable continuous mode */
	SIM->SCGC4 |= SIM_SCGC4_CMP_MASK;
	CMP0->CR1 = CMP_CR1_EN_MASK;

	/* Enable clock on PORTE, configure pin 29 to analog */
	SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;
	PORTE->PCR[29] &= ~PORT_PCR_MUX_MASK;
	PORTE->PCR[29] |= PORT_PCR_MUX(0);

	/* Select input channels,
	 * Plus: channel 5 on Port E bit 29
	 * Minus: channel 7 is CMP DAC
	 */
	CMP0->MUXCR = CMP_MUXCR_PSEL(5) | CMP_MUXCR_MSEL(7);
	
	/* Enable CMP's DAC, set reference voltage to 1.85V
	 * CMP's internal DAC has a 6-bit resolution (64)
	 * 64 * 1.85/3.3V ~= 36
	 */
	CMP0->DACCR = CMP_DACCR_DACEN_MASK | CMP_DACCR_VOSEL(36);
	
	if (irq) {
		/* Enable interrupt on both rising and falling edges */
		CMP0->SCR = CMP_SCR_IEF_MASK | CMP_SCR_IER_MASK;

		NVIC_SetPriority(CMP0_IRQn, 2);
		NVIC_ClearPendingIRQ(CMP0_IRQn);
		NVIC_EnableIRQ(CMP0_IRQn);
	}
}

void CMP0_IRQHandler(void)
{
	if (CMP_COUT) { 
		/* Rising edge */
		CMP0->SCR |= CMP_SCR_CFR_MASK;
	} else if (!CMP_COUT) {
		/* Falling edge */
		CMP0->SCR |= CMP_SCR_CFF_MASK;
	}
}
