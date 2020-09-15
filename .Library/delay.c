
/* Header includes -----------------------------------------------------------*/
#include "delay.h"

void SysTick_Handler(void)
{
  HAL_IncTick();
}

void delayus(uint32_t nus){
    HAL_Delay(nus);
}
void delayms(uint32_t nms){
  while(nms--){
     delayus(1000);
  }
}
void delays (uint32_t ns){
  while(ns--){
     delayus(1000000);
  }
}