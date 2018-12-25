
#include "stm32f0xx_hal.h"
#include "timer_driver.h"
#include "led_driver.h"


TIM_HandleTypeDef    TimHandle;

/* Prescaler declaration */
uint32_t uwPrescalerValue = 0;


void timer14_init(void)
{
	
TIM_OC_InitTypeDef TIM_OCStruct;
GPIO_InitTypeDef GPIO_InitStruct;	
	
	
__HAL_RCC_GPIOB_CLK_ENABLE();
	
// Set up GPIOB1 (timer 3, channel 4) in alternate function (AF) mode

GPIO_InitStruct.Mode  = GPIO_MODE_AF_PP;
GPIO_InitStruct.Pull  = GPIO_PULLUP;
GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
GPIO_InitStruct.Pin   = GPIO_PIN_1;
HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);	

	  /*##-1- Enable peripherals and GPIO Clocks #################################*/
  /* TIMx Peripheral clock enable */
 __HAL_RCC_TIM3_CLK_ENABLE();
  
  /*##-2- Configure the NVIC for TIMx ########################################*/
  /* Set the TIMx priority */
  HAL_NVIC_SetPriority(TIM3_IRQn, 2, 0);

  /* Enable the TIMx global Interrupt */
  HAL_NVIC_EnableIRQ(TIM3_IRQn);
	
	/*##-1- Configure the TIM peripheral #######################################*/
  /* -----------------------------------------------------------------------
    In this example TIM14 input clock (TIM14CLK)  is set to APB1 clock (PCLK1),
    since APB1 prescaler is equal to 1.
      TIM14CLK = PCLK1
      PCLK1 = HCLK
      => TIM2CLK = HCLK = SystemCoreClock
    To get TIM2 counter clock at 10 KHz, the Prescaler is computed as following:
    Prescaler = (TIM14CLK / TIM14 counter clock) - 1
    Prescaler = (SystemCoreClock /10 KHz) - 1

    Note:
     SystemCoreClock variable holds HCLK frequency and is defined in system_stm32f0xx.c file.
     Each time the core clock (HCLK) changes, user had to update SystemCoreClock
     variable value. Otherwise, any configuration based on this variable will be incorrect.
     This variable is updated in three ways:
      1) by calling CMSIS function SystemCoreClockUpdate()
      2) by calling HAL API function HAL_RCC_GetSysClockFreq()
      3) each time HAL_RCC_ClockConfig() is called to configure the system clock frequency
  ----------------------------------------------------------------------- */

  /* Compute the prescaler value to have TIMx counter clock equal to 10000 Hz */
  uwPrescalerValue = (uint32_t)(SystemCoreClock / 10000) - 1;

  /* Set TIMx instance */
  TimHandle.Instance = TIM3;

  /* Initialize TIMx peripheral as follows:
       + Period = 10000 - 1
       + Prescaler = (SystemCoreClock/10000) - 1
       + ClockDivision = 0
       + Counter direction = Up
  */
  TimHandle.Init.Period            = 1250-1;
  TimHandle.Init.Prescaler         = uwPrescalerValue;
  TimHandle.Init.ClockDivision     = 0;
  TimHandle.Init.CounterMode       = TIM_COUNTERMODE_UP;
  TimHandle.Init.RepetitionCounter = 0;
  TimHandle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	
	TimHandle.Channel = HAL_TIM_ACTIVE_CHANNEL_4;
	
	TIM_OCStruct.OCMode = TIM_OCMODE_TOGGLE;
	TIM_OCStruct.Pulse = 0;
	
	HAL_TIM_OC_ConfigChannel(&TimHandle, &TIM_OCStruct, TIM_CHANNEL_4);
	
  if (HAL_TIM_Base_Init(&TimHandle) != HAL_OK)
  {
    /* Initialization Error */
   // Error_Handler();
  }

  /*##-2- Start the TIM Base generation in interrupt mode ####################*/
  /* Start Channel1 */
  if (HAL_TIM_OC_Start(&TimHandle,TIM_CHANNEL_4) != HAL_OK)
  {
    /* Starting Error */
   // Error_Handler();
  }
	
	


}


/**
  * @brief  Period elapsed callback in non blocking mode
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  user_led_toggle();
}


