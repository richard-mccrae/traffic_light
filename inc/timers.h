#ifndef TIMERS_H
#define TIMERS_H

void systick_init(void);

void COP_WDT_init(void);
void COP_WDT_service(void);

void TPM_init(void);

#endif /* TIMERS_H */
