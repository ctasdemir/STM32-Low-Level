/**
  ******************************************************************************
  * @file    adc_driver.c 
  * @author  
  * @version 
  * @date    
  * @brief   ADC Driver Module 
  *******************************************************************************/
#include "stm32f0xx_hal.h"
#include "adc_driver.h"



void adc_driver_init(void)
{
	
	//ADC INPUT PIN A0 ==>  GPIOA - 0. pin
	
	// Enable GPIOA Clock
	__HAL_RCC_GPIOA_CLK_ENABLE();	
	
	// enable ADC1 clock
	__HAL_RCC_ADC1_CLK_ENABLE();
		
	// Select Mode: Input
	GPIOA->MODER   |= (3UL );  // set 11 ==> Analog mode	
		
	// enable ADC 14 Mhz Clock
	RCC->CR2 |= RCC_CR2_HSI14ON; 
	
	// wait for HSI 14 MHZ Clock ready
	while ((RCC->CR2 & RCC_CR2_HSI14RDY) == 0) 
	{
	
	}
	
	ADC1->CFGR2 &= (~ADC_CFGR2_CKMODE); 
  
	// Channel Selection (CHAN0 ==> A0, CHAN17 ==> VREF
	ADC1->CHSELR = ADC_CHSELR_CHSEL0 | ADC_CHSELR_CHSEL17; 
	
	// Set Sampling Time
	ADC1->SMPR |= 7; // 7 ==> 111  	
	
	// Enable VREF
	ADC->CCR |= ADC_CCR_VREFEN; 
}

int32_t adc_get_result(void)
{
	int32_t result = 0;

	/* Start the ADC conversion */
	ADC1->CR |= ADC_CR_ADSTART; 
		
	/* Wait end of conversion */	
	while ((ADC1->ISR & ADC_ISR_EOC) == 0) 
	{
		/* For robust implementation, add here time-out management */
	}
			
	result = ADC1->DR; /* Store the ADC conversion result */	
		
	return result;
}

void adc_start_calibration(void)
{
	//Disable ADC 
	if ((ADC1->CR & ADC_CR_ADEN) != 0) 
	{
		ADC1->CR &= (uint32_t)(~ADC_CR_ADEN); 
		//CLEAR_BIT(ADC1->CR,ADC1->ADC_CR_ADEN);
	}
	
  // Start Calibration
	ADC1->CR |= ADC_CR_ADCAL; 
	
	// Wait for calibration to finish
	while ((ADC1->CR & ADC_CR_ADCAL) != 0) 
	{
	
	}
	
	// Enable ADC
	ADC1->CR |= ADC_CR_ADEN;
}
