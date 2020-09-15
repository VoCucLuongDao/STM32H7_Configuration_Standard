#include "tim_config.h"
void TIM6_Config()
{
  __HAL_RCC_TIM6_CLK_ENABLE();
  TIM_Base_InitTypeDef TIM_Base_InitStructure;
  TIM_Base_InitStructure.CounterMode = TIM_COUNTERMODE_UP;
  TIM_Base_InitStructure.ClockDivision = 5;
  TIM_Base_InitStructure.Prescaler = 5;
  TIM_Base_InitStructure.Period = 6;
  TIM_Base_InitStructure.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
 // TIM_Base_InitStructure.RepetitionCounter = TIM_
  
}

void TIM7_Config()
{
     __HAL_RCC_TIM7_CLK_ENABLE();

}