#include "main.h"
#include "stdio.h"

void Led_config(){
   RCC->AHB4ENR |= 1<<3;
   GPIOD->MODER &= ~0x03; 
   GPIOD->MODER |=0x01;
   GPIOD->OTYPER|= 0x00; 
   GPIOD->PUPDR |= 0x01;
   GPIOD->OSPEEDR |= 0x03;
  
   GPIOD->ODR ^= 0x01;
}


void main(void)
{
  
  while(1);
}
