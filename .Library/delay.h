#ifndef __DELAY_H
#define __DELAY_H

#ifdef __cplusplus
extern "C" {
#endif

/* Header includes -----------------------------------------------------------*/
#include "stm32h7xx_hal.h"
#include "stm32h7xx_hal_rcc.h"

/* Function definitions ------------------------------------------------------*/

void delayus(uint32_t nus);
void delayms(uint32_t nms);
void delays (uint32_t ns);
#ifdef __cplusplus
}
#endif

#endif /* __DELAY_H */
