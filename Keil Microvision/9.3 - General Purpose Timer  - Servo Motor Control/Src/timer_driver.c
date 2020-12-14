
#include "stm32f0xx_hal.h"
#include "timer_driver.h"



void timer14_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;
	
	__HAL_RCC_TIM14_CLK_ENABLE();
	
	GPIO_InitStruct.Pin       = GPIO_PIN_7;
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull      = GPIO_NOPULL;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF4_TIM14;
  
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    
  
	
	
	TIM14->PSC  = 48000 - 1; // Timer clock = 48 mhz / 48000 = 1000 Hz (1 ms period)
	
	//OUTPUT COMPARE MODE - TOGGLE ON MATCH
	TIM14->CCMR1 |= TIM_CCMR1_OC1M_0;
	TIM14->CCMR1 |= (TIM_CCMR1_OC1M_1);
	TIM14->CCMR1 &= ~(TIM_CCMR1_OC1M_2);
	
	// OUTPUT COMPARE
	TIM14->CCMR1 &= ~TIM_CCMR1_CC1S_0;
	TIM14->CCMR1 &= ~TIM_CCMR1_CC1S_1;
	
	// Enable OC1REF OUTPUT
	TIM14->CCER |= TIM_CCER_CC1E;
	
	
	
 // TIM14->DIER |= TIM_DIER_UIE;
	TIM14->EGR |= TIM_EGR_UG;
	
	
	TIM14->ARR = 199; 
	TIM14->CCR1 = 199; 	
}

void timer14_enable(void)
{
	TIM14->CR1 |= TIM_CR1_CEN;
}

void timer14_disable(void)
{
	TIM14->CR1 &= ~(TIM_CR1_CEN);	
}



