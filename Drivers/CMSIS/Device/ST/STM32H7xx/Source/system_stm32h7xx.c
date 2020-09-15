/**
  ******************************************************************************
  * @file    system_stm32h7xx.c
  * @author  MCD Application Team
  * @brief   CMSIS Cortex-Mx Device Peripheral Access Layer System Source File.
  *
  *   This file provides two functions and one global variable to be called from
  *   user application:
  *      - SystemInit(): This function is called at startup just after reset and
  *                      before branch to main program. This call is made inside
  *                      the "startup_stm32h7xx.s" file.
  *
  *      - SystemCoreClock variable: Contains the core clock, it can be used
  *                                  by the user application to setup the SysTick
  *                                  timer or configure other parameters.
  *
  *      - SystemCoreClockUpdate(): Updates the variable SystemCoreClock and must
  *                                 be called whenever the core clock is changed
  *                                 during program execution.
  *
  *
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/** @addtogroup CMSIS
  * @{
  */

/** @addtogroup stm32h7xx_system
  * @{
  */

/** @addtogroup STM32H7xx_System_Private_Includes
  * @{
  */

#include "stm32h7xx.h"
#include <math.h>
#if !defined  (HSE_VALUE)
#define HSE_VALUE    ((uint32_t)25000000) /*!< Value of the External oscillator in Hz */
#endif /* HSE_VALUE */

#if !defined  (CSI_VALUE)
  #define CSI_VALUE    ((uint32_t)4000000) /*!< Value of the Internal oscillator in Hz*/
#endif /* CSI_VALUE */

#if !defined  (HSI_VALUE)
  #define HSI_VALUE    ((uint32_t)64000000) /*!< Value of the Internal oscillator in Hz*/
#endif /* HSI_VALUE */


/**
  * @}
  */

/** @addtogroup STM32H7xx_System_Private_TypesDefinitions
  * @{
  */

/**
  * @}
  */

/** @addtogroup STM32H7xx_System_Private_Defines
  * @{
  */

/************************* Miscellaneous Configuration ************************/
/*!< Uncomment the following line if you need to use initialized data in D2 domain SRAM (AHB SRAM) */
/* #define DATA_IN_D2_SRAM */

/*!< Uncomment the following line if you need to relocate your vector Table in
     Internal SRAM. */
/* #define VECT_TAB_SRAM */
#define VECT_TAB_OFFSET  0x00000000UL /*!< Vector Table base offset field.
                                      This value must be a multiple of 0x200. */
/******************************************************************************/

/**
  * @}
  */

/** @addtogroup STM32H7xx_System_Private_Macros
  * @{
  */

/**
  * @}
  */

/** @addtogroup STM32H7xx_System_Private_Variables
  * @{
  */
  /* This variable is updated in three ways:
      1) by calling CMSIS function SystemCoreClockUpdate()
      2) by calling HAL API function HAL_RCC_GetHCLKFreq()
      3) each time HAL_RCC_ClockConfig() is called to configure the system clock frequency
         Note: If you use this function to configure the system clock; then there
               is no need to call the 2 first functions listed above, since SystemCoreClock
               variable is updated automatically.
  */
  uint32_t SystemCoreClock = 64000000;
  uint32_t SystemD2Clock =   64000000;
  const  uint8_t D1CorePrescTable[16] = {0, 0, 0, 0, 1, 2, 3, 4, 1, 2, 3, 4, 6, 7, 8, 9};

/**
  * @}
  */

/** @addtogroup STM32H7xx_System_Private_FunctionPrototypes
  * @{
  */

/**
  * @}
  */

/** @addtogroup STM32H7xx_System_Private_Functions
  * @{
  */
void SystemCoreClockUpdate(void);
void SetSystemClock(void);
/**
  * @brief  Setup the microcontroller system
  *         Initialize the FPU setting and  vector table location
  *         configuration.
  * @param  None
  * @retval None
  */
void SystemInit(void)
{
      #if defined (DATA_IN_D2_SRAM)
       __IO uint32_t tmpreg;
      #endif /* DATA_IN_D2_SRAM */

        /* FPU settings ------------------------------------------------------------*/
        #if (__FPU_PRESENT == 1) && (__FPU_USED == 1)
          SCB->CPACR |= ((3UL << (10*2))|(3UL << (11*2)));  /* set CP10 and CP11 Full Access */
        #endif
        /* Reset the RCC clock configuration to the default reset state ------------*/

         /* Increasing the CPU frequency */
        if(FLASH_LATENCY_DEFAULT  > (READ_BIT((FLASH->ACR), FLASH_ACR_LATENCY)))
        {
          /* Program the new number of wait states to the LATENCY bits in the FLASH_ACR register */
              MODIFY_REG(FLASH->ACR, FLASH_ACR_LATENCY, (uint32_t)(FLASH_LATENCY_DEFAULT));
        }

        /* Set HSION bit */
        RCC->CR |= RCC_CR_HSION;

        /* Reset CFGR register */
        RCC->CFGR = 0x00000000;

        /* Reset HSEON, HSECSSON, CSION, HSI48ON, CSIKERON, PLL1ON, PLL2ON and PLL3ON bits */
        RCC->CR &= 0xEAF6ED7FU;
        
         /* Decreasing the number of wait states because of lower CPU frequency */
        if(FLASH_LATENCY_DEFAULT  < (READ_BIT((FLASH->ACR), FLASH_ACR_LATENCY)))
        {
          /* Program the new number of wait states to the LATENCY bits in the FLASH_ACR register */
              MODIFY_REG(FLASH->ACR, FLASH_ACR_LATENCY, (uint32_t)(FLASH_LATENCY_DEFAULT));
        }

      #if defined(D3_SRAM_BASE)
        /* Reset D1CFGR register */
        RCC->D1CFGR = 0x00000000;

        /* Reset D2CFGR register */
        RCC->D2CFGR = 0x00000000;

        /* Reset D3CFGR register */
        RCC->D3CFGR = 0x00000000;
      #else
        /* Reset CDCFGR1 register */
        RCC->CDCFGR1 = 0x00000000;

        /* Reset CDCFGR2 register */
        RCC->CDCFGR2 = 0x00000000;

        /* Reset SRDCFGR register */
        RCC->SRDCFGR = 0x00000000;
      #endif
        /* Reset PLLCKSELR register */
        RCC->PLLCKSELR = 0x02020200;

        /* Reset PLLCFGR register */
        RCC->PLLCFGR = 0x01FF0000;
        /* Reset PLL1DIVR register */
        RCC->PLL1DIVR = 0x01010280;
        /* Reset PLL1FRACR register */
        RCC->PLL1FRACR = 0x00000000;

        /* Reset PLL2DIVR register */
        RCC->PLL2DIVR = 0x01010280;

        /* Reset PLL2FRACR register */

        RCC->PLL2FRACR = 0x00000000;
        /* Reset PLL3DIVR register */
        RCC->PLL3DIVR = 0x01010280;

        /* Reset PLL3FRACR register */
        RCC->PLL3FRACR = 0x00000000;

        /* Reset HSEBYP bit */
        RCC->CR &= 0xFFFBFFFFU;

        /* Disable all interrupts */
        RCC->CIER = 0x00000000;

      #if (STM32H7_DEV_ID == 0x450UL)
        /* dual core CM7 or single core line */
        if((DBGMCU->IDCODE & 0xFFFF0000U) < 0x20000000U)
        {
          /* if stm32h7 revY*/
          /* Change  the switch matrix read issuing capability to 1 for the AXI SRAM target (Target 7) */
          *((__IO uint32_t*)0x51008108) = 0x000000001U;
        }
      #endif

      #if defined (DATA_IN_D2_SRAM)
        /* in case of initialized data in D2 SRAM (AHB SRAM) , enable the D2 SRAM clock (AHB SRAM clock) */
      #if defined(RCC_AHB2ENR_D2SRAM3EN)
        RCC->AHB2ENR |= (RCC_AHB2ENR_D2SRAM1EN | RCC_AHB2ENR_D2SRAM2EN | RCC_AHB2ENR_D2SRAM3EN);
      #elif defined(RCC_AHB2ENR_D2SRAM2EN)
        RCC->AHB2ENR |= (RCC_AHB2ENR_D2SRAM1EN | RCC_AHB2ENR_D2SRAM2EN);
      #else
        RCC->AHB2ENR |= (RCC_AHB2ENR_AHBSRAM1EN | RCC_AHB2ENR_AHBSRAM2EN);
      #endif /* RCC_AHB2ENR_D2SRAM3EN */

        tmpreg = RCC->AHB2ENR;
        (void) tmpreg;
      #endif /* DATA_IN_D2_SRAM */

      #if defined(DUAL_CORE) && defined(CORE_CM4)
        /* Configure the Vector Table location add offset address for cortex-M4 ------------------*/
      #ifdef VECT_TAB_SRAM
        SCB->VTOR = D2_AXISRAM_BASE | VECT_TAB_OFFSET; /* Vector Table Relocation in Internal SRAM */
      #else
        SCB->VTOR = FLASH_BANK2_BASE | VECT_TAB_OFFSET; /* Vector Table Relocation in Internal FLASH */
      #endif /* VECT_TAB_SRAM */

      #else

        /*
         * Disable the FMC bank1 (enabled after reset).
         * This, prevents CPU speculation access on this bank which blocks the use of FMC during
         * 24us. During this time the others FMC master (such as LTDC) cannot use it!
         */
        FMC_Bank1_R->BTCR[0] = 0x000030D2;

        /* Configure the Vector Table location add offset address for cortex-M7 ------------------*/
      #ifdef VECT_TAB_SRAM
        SCB->VTOR = D1_AXISRAM_BASE  | VECT_TAB_OFFSET; /* Vector Table Relocation in Internal AXI-RAM */
      #else
        SCB->VTOR = FLASH_BANK1_BASE | VECT_TAB_OFFSET; /* Vector Table Relocation in Internal FLASH */
      #endif

      #endif /*DUAL_CORE && CORE_CM4*/
      /*Set value for register configure SystemCoreClock--------------------------------------------*/  
       SetSystemClock();
  
}

/**
   * @brief  Update SystemCoreClock variable according to Clock Register Values.
  *         The SystemCoreClock variable contains the core clock , it can
  *         be used by the user application to setup the SysTick timer or configure
  *         other parameters.
  *
  * @note   Each time the core clock changes, this function must be called
  *         to update SystemCoreClock variable value. Otherwise, any configuration
  *         based on this variable will be incorrect.
  *
  * @note   - The system frequency computed by this function is not the real
  *           frequency in the chip. It is calculated based on the predefined
  *           constant and the selected clock source:
  *
  *           - If SYSCLK source is CSI, SystemCoreClock will contain the CSI_VALUE(*)
  *           - If SYSCLK source is HSI, SystemCoreClock will contain the HSI_VALUE(**)
  *           - If SYSCLK source is HSE, SystemCoreClock will contain the HSE_VALUE(***)
  *           - If SYSCLK source is PLL, SystemCoreClock will contain the CSI_VALUE(*),
  *             HSI_VALUE(**) or HSE_VALUE(***) multiplied/divided by the PLL factors.
  *
  *         (*) CSI_VALUE is a constant defined in stm32h7xx_hal.h file (default value
  *             4 MHz) but the real value may vary depending on the variations
  *             in voltage and temperature.
  *         (**) HSI_VALUE is a constant defined in stm32h7xx_hal.h file (default value
  *             64 MHz) but the real value may vary depending on the variations
  *             in voltage and temperature.
  *
  *         (***)HSE_VALUE is a constant defined in stm32h7xx_hal.h file (default value
  *              25 MHz), user has to ensure that HSE_VALUE is same as the real
  *              frequency of the crystal used. Otherwise, this function may
  *              have wrong result.
  *
  *         - The result of this function could be not correct when using fractional
  *           value for HSE crystal.
  * @param  None
  * @retval None
  */
void SystemCoreClockUpdate (void)
{
        uint32_t pllp, pllsource, pllm, pllfracen, hsivalue, tmp;
        uint32_t common_system_clock;
        float_t fracn1, pllvco;


        /* Get SYSCLK source -------------------------------------------------------*/

        switch (RCC->CFGR & RCC_CFGR_SWS)
        {
        case RCC_CFGR_SWS_HSI:  /* HSI used as system clock source */
          common_system_clock = (uint32_t) (HSI_VALUE >> ((RCC->CR & RCC_CR_HSIDIV)>> 3));
          break;

        case RCC_CFGR_SWS_CSI:  /* CSI used as system clock  source */
          common_system_clock = CSI_VALUE;
          break;

        case RCC_CFGR_SWS_HSE:  /* HSE used as system clock  source */
          common_system_clock = HSE_VALUE;
          break;

        case RCC_CFGR_SWS_PLL1:  /* PLL1 used as system clock  source */

          /* PLL_VCO = (HSE_VALUE or HSI_VALUE or CSI_VALUE/ PLLM) * PLLN
          SYSCLK = PLL_VCO / PLLR
          */
          pllsource = (RCC->PLLCKSELR & RCC_PLLCKSELR_PLLSRC);
          pllm = ((RCC->PLLCKSELR & RCC_PLLCKSELR_DIVM1)>> 4)  ;
          pllfracen = ((RCC->PLLCFGR & RCC_PLLCFGR_PLL1FRACEN)>>RCC_PLLCFGR_PLL1FRACEN_Pos);
          fracn1 = (float_t)(uint32_t)(pllfracen* ((RCC->PLL1FRACR & RCC_PLL1FRACR_FRACN1)>> 3));

          if (pllm != 0U)
          {
            switch (pllsource)
            {
              case RCC_PLLCKSELR_PLLSRC_HSI:  /* HSI used as PLL clock source */

              hsivalue = (HSI_VALUE >> ((RCC->CR & RCC_CR_HSIDIV)>> 3)) ;
              pllvco = ( (float_t)hsivalue / (float_t)pllm) * ((float_t)(uint32_t)(RCC->PLL1DIVR & RCC_PLL1DIVR_N1) + (fracn1/(float_t)0x2000) +(float_t)1 );

              break;

              case RCC_PLLCKSELR_PLLSRC_CSI:  /* CSI used as PLL clock source */
                pllvco = ((float_t)CSI_VALUE / (float_t)pllm) * ((float_t)(uint32_t)(RCC->PLL1DIVR & RCC_PLL1DIVR_N1) + (fracn1/(float_t)0x2000) +(float_t)1 );
              break;

              case RCC_PLLCKSELR_PLLSRC_HSE:  /* HSE used as PLL clock source */
                pllvco = ((float_t)HSE_VALUE / (float_t)pllm) * ((float_t)(uint32_t)(RCC->PLL1DIVR & RCC_PLL1DIVR_N1) + (fracn1/(float_t)0x2000) +(float_t)1 );
              break;

            default:
                pllvco = ((float_t)CSI_VALUE / (float_t)pllm) * ((float_t)(uint32_t)(RCC->PLL1DIVR & RCC_PLL1DIVR_N1) + (fracn1/(float_t)0x2000) +(float_t)1 );
              break;
            }
            pllp = (((RCC->PLL1DIVR & RCC_PLL1DIVR_P1) >>9) + 1U ) ;
            common_system_clock =  (uint32_t)(float_t)(pllvco/(float_t)pllp);
          }
          else
          {
            common_system_clock = 0U;
          }
          break;

        default:
          common_system_clock = CSI_VALUE;
          break;
        }

        /* Compute SystemClock frequency --------------------------------------------------*/
      #if defined (RCC_D1CFGR_D1CPRE)
        tmp = D1CorePrescTable[(RCC->D1CFGR & RCC_D1CFGR_D1CPRE)>> RCC_D1CFGR_D1CPRE_Pos];

        /* common_system_clock frequency : CM7 CPU frequency  */
        common_system_clock >>= tmp;

        /* SystemD2Clock frequency : CM4 CPU, AXI and AHBs Clock frequency  */
        SystemD2Clock = (common_system_clock >> ((D1CorePrescTable[(RCC->D1CFGR & RCC_D1CFGR_HPRE)>> RCC_D1CFGR_HPRE_Pos]) & 0x1FU));

      #else
        tmp = D1CorePrescTable[(RCC->CDCFGR1 & RCC_CDCFGR1_CDCPRE)>> RCC_CDCFGR1_CDCPRE_Pos];

        /* common_system_clock frequency : CM7 CPU frequency  */
        common_system_clock >>= tmp;

        /* SystemD2Clock frequency : AXI and AHBs Clock frequency  */
        SystemD2Clock = (common_system_clock >> ((D1CorePrescTable[(RCC->CDCFGR1 & RCC_CDCFGR1_HPRE)>> RCC_CDCFGR1_HPRE_Pos]) & 0x1FU));

      #endif

      #if defined(DUAL_CORE) && defined(CORE_CM4)
        SystemCoreClock = SystemD2Clock;
      #else
        SystemCoreClock = common_system_clock;
      #endif /* DUAL_CORE && CORE_CM4 */
}

void SetSystemClock(){
  
        FLASH->ACR = FLASH_ACR_LATENCY_4WS;
          
        MODIFY_REG (PWR->CR3, PWR_SUPPLY_CONFIG_MASK, PWR_LDO_SUPPLY);
        
        MODIFY_REG(PWR->D3CR, PWR_D3CR_VOS, PWR_REGULATOR_VOLTAGE_SCALE1);                          
        /* Enable the PWR overdrive */                                         
        SET_BIT(SYSCFG->PWRCR, SYSCFG_PWRCR_ODEN);                     

        while(PWR->D3CR & PWR_D3CR_VOSRDY != PWR_FLAG_VOSRDY);       
           
	RCC->CR |= RCC_CR_HSEON;
	// Wait till HSE is ready
	while((RCC->CR & RCC_CR_HSERDY) == 0);
	
	// CSI selected to PLL1 clock
	RCC->PLLCKSELR &= ~RCC_PLLCKSELR_PLLSRC;
	RCC->PLLCKSELR |= 0x02;
	
	// Prescaler reset & set /5
	RCC->PLLCKSELR &= ~ 0x03FC;
	RCC->PLLCKSELR |= 0x05<<4;
        RCC->PLLCKSELR |= 0x02;
	
	// Medium VCO (192-836) MHz
	RCC->PLLCFGR &= ~0x03;
	// PLL1 input fqu between 4 and 8 MHz
	RCC->PLLCFGR |= RCC_PLLCFGR_PLL1RGE;
	// fractional latch = 0
	RCC->PLLCFGR &= ~RCC_PLLCFGR_PLL1FRACEN;
	
	// PLL1 DIVP divider setup, first check PLL1 state, if on or ready -> shutdown
	if(((RCC->CR)&RCC_CR_PLL1ON) == RCC_CR_PLL1ON || ((RCC->CR)&RCC_CR_PLL1RDY) == RCC_CR_PLL1RDY){
		RCC->CR &= ~RCC_CR_PLL1ON;
		while(((RCC->CR)&RCC_CR_PLL1RDY) != RCC_CR_PLL1RDY){};
	}
	RCC->PLLCFGR &= ~RCC_PLLCFGR_DIVP1EN;
	RCC->PLLCFGR |= RCC_PLLCFGR_DIVP1EN;
	
	//PLL1 DIVQ divider enable
	RCC->PLLCFGR &= ~RCC_PLLCFGR_DIVQ1EN;
	RCC->PLLCFGR |= RCC_PLLCFGR_DIVQ1EN;
	
	//PLL1 DIVR divider enable
	RCC->PLLCFGR &= ~RCC_PLLCFGR_DIVR1EN;
	RCC->PLLCFGR |= RCC_PLLCFGR_DIVR1EN;
	
	RCC->PLL1DIVR &= ~RCC_PLL1DIVR_N1;
	RCC->PLL1DIVR |= 0xBF;
        
	RCC->PLL1DIVR &= ~RCC_PLL1DIVR_P1;
        RCC->PLL1DIVR |= 0x01<<9;
        
	RCC->PLL1DIVR &= ~RCC_PLL1DIVR_Q1;
         RCC->PLL1DIVR |= 0x01<<16;
         
	RCC->PLL1DIVR &= ~RCC_PLL1DIVR_R1;
        RCC->PLL1DIVR |= 0x01<<24;
	// PLL1 enable and wait ready flag to continue
	RCC->CR |= RCC_CR_PLL1ON;
	while(((RCC->CR)&RCC_CR_PLL1RDY) != RCC_CR_PLL1RDY);
  
     /* RCC->CFGR &= ~0x00000007; //0
        RCC->CFGR |=  0x00000003; //1
        while((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL1);
         
        RCC->D1CFGR &= ~0x00000F7F;
        RCC->D1CFGR |= 0x00000048;
        
        RCC -> D2CFGR &= ~0x00000770;
        RCC -> D2CFGR |= 0x00000440;
          
        RCC -> D3CFGR &= ~0x00000070;
        RCC -> D3CFGR |=  0x00000040;*/
      
      /* Increasing the BUS frequency divider */
      /*-------------------------- D1PCLK1/CDPCLK1 Configuration --------------*/
          MODIFY_REG(RCC->D1CFGR, RCC_D1CFGR_D1PPRE, 0x00000040 );
      /*-------------------------- PCLK1 Configuration ------------------------*/
          MODIFY_REG(RCC->D2CFGR, RCC_D2CFGR_D2PPRE1, 0x00000040 );
      /*-------------------------- PCLK2 Configuration ------------------------*/
          MODIFY_REG(RCC->D2CFGR, RCC_D2CFGR_D2PPRE2, 0x00000400 );
      /*-------------------------- D3PCLK1 Configuration ----------------------*/
          MODIFY_REG(RCC->D3CFGR, RCC_D3CFGR_D3PPRE,  0x00000040 );
       /*-------------------------- HCLK Configuration ------------------------*/
          MODIFY_REG(RCC->D1CFGR, RCC_D1CFGR_HPRE, 0x00000008);
       /*------------------------- SYSCLK Configuration ----------------------*/
          MODIFY_REG(RCC->D1CFGR, RCC_D1CFGR_D1CPRE, 0x00000000);
         
          MODIFY_REG(RCC->CFGR, RCC_CFGR_SW, 0x00000003);

          while ((RCC->CFGR & 0x0038) != 0x00000018);

          SystemCoreClockUpdate();
            
          /* Configure the source of time base considering new system clocks settings*/
          SysTick->LOAD  = (uint32_t)(480 - 1UL);                           /* set reload register */
          NVIC_SetPriority (SysTick_IRQn, (1UL << __NVIC_PRIO_BITS) - 1UL); /* set Priority for Systick Interrupt */
          SysTick->VAL   = 0UL;                                             /* Load the SysTick Counter Value */
          SysTick->CTRL  = 0x07;                                            /* Enable SysTick IRQ and SysTick Timer */
                 
}

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/