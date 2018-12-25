/**
  ******************************************************************************
  * @file    Templates/Src/main.c 
  * @author  C.T
  * @version 
  * @date    
  * @brief   Main program body
  ******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_hal.h"
#include "UART_driver.h"
#include "led_driver.h"
#include "button_driver.h"
#include "adc_driver.h"
#include "timer_driver.h"
#include "pwm_driver.h"

static void SystemClock_Config(void);
static void Error_Handler(void);

void send_adc_string(void);

void pwm_sweep(void);


/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  HAL_Init();

  /* Configure the system clock to have a system clock = 48 Mhz */
  SystemClock_Config();

	UART_Init();	
	user_led_init();
  button_init();	
	adc_driver_init();	
	adc_start_calibration();
	pwm_init();

  /* Infinite loop */
  while (1)
  {
		/*
			pwm_set_duty_cycle(1000,1);
			pwm_set_duty_cycle(1900,2);
			HAL_Delay(200);			
			
			pwm_set_duty_cycle(1100,1);
			pwm_set_duty_cycle(1500,2);
			HAL_Delay(200);			
		
			pwm_set_duty_cycle(1300,1);
			pwm_set_duty_cycle(1300,2);
			HAL_Delay(200);			
		
			pwm_set_duty_cycle(1700,1);
			pwm_set_duty_cycle(1100,2);
			HAL_Delay(200);			
		*/
		pwm_sweep();
			
  }
}


void pwm_sweep(void)
{
	static int32_t sweep =0 ;
	
	for(sweep = 0; sweep <2000; sweep++)
	{
		pwm_set_duty_cycle(sweep,1);
		pwm_set_duty_cycle(sweep,2);
		HAL_Delay(1);
	}
	
	for(sweep = 2000; sweep >=0; sweep--)
	{
		pwm_set_duty_cycle(sweep,1);
		pwm_set_duty_cycle(sweep,2);
		HAL_Delay(1);
	}
}

void send_adc_string(void)
{

	uint32_t n = 0;
	static uint32_t zaman;
	zaman++;
	n = adc_start_conversion();
	
	//printf("zaman:%d gelen_veri:%d Buton Durum:%d\n\r",zaman,n,button_get_state());
}


/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSI)
  *            SYSCLK(Hz)                     = 48000000
  *            HCLK(Hz)                       = 48000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 1
  *            HSI Frequency(Hz)              = 8000000
  *            PREDIV                         = 1
  *            PLLMUL                         = 6
  *            Flash Latency(WS)              = 1
  * @param  None
  * @retval None
  */
static void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;
  
  /* No HSE Oscillator on Nucleo, Activate PLL with HSI as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState          = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue  = RCC_HSICALIBRATION_DEFAULT;  
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL6;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct)!= HAL_OK)
  {
    Error_Handler();
  }

  /* Select PLL as system clock source and configure the HCLK, PCLK1 clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1)!= HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{
  /* User may add here some code to deal with this error */
  while(1)
  {
  }
}

