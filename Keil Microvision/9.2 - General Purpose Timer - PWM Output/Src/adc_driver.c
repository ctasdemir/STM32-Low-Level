
#include "stm32f0xx_hal.h"
#include "adc_driver.h"


#define __CHECK_FLAG(__HANDLE__, __FLAG__)                               \
  (((__HANDLE__) & (__FLAG__)) == (__FLAG__))

volatile int32_t adc_result[4] = {0};
int32_t adc_index =0 ;

void adc_driver_init(void)
{
	
	//ADC INPUT PIN ADC_IN0 ==>  GPIOA - 0. pin
	//ADC INPUT PIN ADC_IN1 ==>  GPIOA - 1. pin
	//ADC INPUT PIN ADC_IN4 ==>  GPIOA - 4. pin
	//ADC INPUT PIN ADC_IN8 ==>  GPIOB - 0. pin
	
	// Enable Clock
	__HAL_RCC_GPIOA_CLK_ENABLE();	
	__HAL_RCC_GPIOB_CLK_ENABLE();	
	
	// Select Mode: Input
	
	GPIOA->MODER   |= (3UL);  // GPIOA A0 - set 11 ==> Analog mode	
	
	GPIOA->MODER   |= (3UL<<2);  //  GPIOA A1 set 11 ==> Analog mode	
	
	GPIOA->MODER   |= (3UL<<8);  //  GPIOA A4 set 11 ==> Analog mode	
	
	GPIOB->MODER   |= (3UL);  //  GPIOA B0 set 11 ==> Analog mode	
	
	
		// enable ADC1 clock
	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN; 
	
	// enable ADC 14 Mhz Clock
	RCC->CR2 |= RCC_CR2_HSI14ON; 
	
	// wait for HSI 14 MHZ Clock ready
	while ((RCC->CR2 & RCC_CR2_HSI14RDY) == 0) 
	{
	
	}
	// Enable ADC
//	ADC1->CR |= ADC_CR_ADEN;
	
	ADC1->CFGR2 &= (~ADC_CFGR2_CKMODE); 
  
	// Channel Selection
	ADC1->CHSELR = ADC_CHSELR_CHSEL0 | ADC_CHSELR_CHSEL1 | ADC_CHSELR_CHSEL4 | ADC_CHSELR_CHSEL8 |  ADC_CHSELR_CHSEL17; 
	
	//Sampling Time 
	ADC1->SMPR |= 7; // 111  	==> 
	
	// Enable VREF
	ADC->CCR |= ADC_CCR_VREFEN; 
	
	ADC1->IER |= ADC_IER_EOCIE;
	ADC1->IER |= ADC_IER_EOSIE;
	
	
	NVIC_EnableIRQ(ADC1_IRQn);
	NVIC_SetPriority(ADC1_IRQn,0);
}


void adc_print_results()
{
	
	printf("ADC1=%d ADC2=%d ADC3=%d ADC4=%d\n\r",adc_result[0],adc_result[1],adc_result[2],adc_result[3]);
}

int32_t adc_start_conversion(void)
{
	
	
	/* Performs the AD conversion */
	ADC1->CR |= ADC_CR_ADSTART; /* Start the ADC conversion */
//	ADC1->CFGR1 |= ADC_CFGR1_CONT; // Contionous mode
	
	return 0;
}

int32_t adc_return_conversion_result(void)
{
	return 0;
}

void adc_start_calibration(void)
{
	if ((ADC1->CR & ADC_CR_ADEN) != 0) 
	{
		ADC1->CR &= (uint32_t)(~ADC_CR_ADEN); 
	}
	
	ADC1->CR &= (uint32_t)(~ADC_CR_ADEN);

	ADC1->CR |= ADC_CR_ADCAL; 
	
	while ((ADC1->CR & ADC_CR_ADCAL) != 0) 
	{
		// TODO: timeout control
	}
	
	// Enable ADC
	ADC1->CR |= ADC_CR_ADEN;
}


void ADC1_IRQHandler()
{
	// ADC OVERRUN
	if(( (ADC1->ISR & ADC_ISR_OVR) !=0 ) && ((ADC1->IER & ADC_IER_OVRIE) != 0)) 
	{		
		ADC1->ISR |= ADC_ISR_OVR;
		return;
	}
	
	
//	if( (( ADC1->ISR & ADC_ISR_EOC) !=0 ) && ((ADC1->IER & ADC_IER_EOCIE) != 0)) || (( (ADC1->ISR & ADC_ISR_EOS) !=0 ) && ((ADC1->IER & ADC_IER_EOSIE) != 0))) 
	
	if( (__CHECK_FLAG(ADC1->ISR,ADC_ISR_EOC) && __CHECK_FLAG(ADC1->IER,ADC_IER_EOCIE)) || (__CHECK_FLAG(ADC1->ISR,ADC_ISR_EOS) && __CHECK_FLAG(ADC1->IER,ADC_IER_EOSIE)) )
		
	{
		adc_result[adc_index++] = ADC1->DR;
  
	if( (ADC1->ISR & ADC_ISR_EOS) !=0 )
	{
		adc_index = 0;		
	
	}
	
	//ADC1->ISR |= ADC_ISR_EOS;
	ADC1->ISR |= ADC_ISR_EOC;		
	}
}
