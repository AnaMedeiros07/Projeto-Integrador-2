/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    adc.c
  * @brief   This file provides code for the configuration
  *          of the ADC instances.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "adc.h"

/* USER CODE BEGIN 0 */
#include "comandos.h"
#include "dac.h"
#include "output.h"

ADC_ChannelConfTypeDef adcChannel = {0};
GPIO_TypeDef*  Ports[]={GPIOA,GPIOB,GPIOC,GPIOD,GPIOE,GPIOF,GPIOG,GPIOI,GPIOJ,GPIOK};
uint16_t Pins[]={GPIO_PIN_0,GPIO_PIN_1,GPIO_PIN_2,GPIO_PIN_3,GPIO_PIN_4,GPIO_PIN_5,GPIO_PIN_6,GPIO_PIN_7,GPIO_PIN_8,GPIO_PIN_9,GPIO_PIN_10,GPIO_PIN_11,GPIO_PIN_12,GPIO_PIN_13,GPIO_PIN_14,GPIO_PIN_15};
uint32_t AdcChannels[]={ADC_CHANNEL_0,ADC_CHANNEL_1,ADC_CHANNEL_2,ADC_CHANNEL_3,ADC_CHANNEL_4,ADC_CHANNEL_5,ADC_CHANNEL_6,ADC_CHANNEL_7,ADC_CHANNEL_8,ADC_CHANNEL_9,ADC_CHANNEL_10,ADC_CHANNEL_11,ADC_CHANNEL_12,ADC_CHANNEL_13,ADC_CHANNEL_14,ADC_CHANNEL_15,ADC_CHANNEL_TEMPSENSOR,ADC_CHANNEL_17,ADC_CHANNEL_18};
uint16_t gpio_adc_pins[]={GPIO_PIN_0,GPIO_PIN_1,GPIO_PIN_2,GPIO_PIN_3,GPIO_PIN_4,GPIO_PIN_5,GPIO_PIN_6,GPIO_PIN_7,GPIO_PIN_0,GPIO_PIN_1,GPIO_PIN_0,GPIO_PIN_1,GPIO_PIN_2,GPIO_PIN_3,GPIO_PIN_4,GPIO_PIN_5};
GPIO_TypeDef* gpio_adc_ports[]={GPIOA,GPIOA,GPIOA,GPIOA,GPIOA,GPIOA,GPIOA,GPIOA,GPIOB,GPIOB,GPIOC,GPIOC,GPIOC,GPIOC,GPIOC,GPIOC};
uint32_t adc_value = 0;
_Bool int_adc=0;


/* USER CODE END 0 */

ADC_HandleTypeDef hadc1;

/* ADC1 init function */
void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */
  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISING;
  hadc1.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T6_TRGO;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.DMAContinuousRequests = DISABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_0;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

void HAL_ADC_MspInit(ADC_HandleTypeDef* adcHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(adcHandle->Instance==ADC1)
  {
  /* USER CODE BEGIN ADC1_MspInit 0 */

  /* USER CODE END ADC1_MspInit 0 */
    /* ADC1 clock enable */
    __HAL_RCC_ADC1_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**ADC1 GPIO Configuration
    PA0/WKUP     ------> ADC1_IN0
    */
    GPIO_InitStruct.Pin = GPIO_PIN_0;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* ADC1 interrupt Init */
    HAL_NVIC_SetPriority(ADC_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(ADC_IRQn);
  /* USER CODE BEGIN ADC1_MspInit 1 */

  /* USER CODE END ADC1_MspInit 1 */
  }
}

void HAL_ADC_MspDeInit(ADC_HandleTypeDef* adcHandle)
{

  if(adcHandle->Instance==ADC1)
  {
  /* USER CODE BEGIN ADC1_MspDeInit 0 */

  /* USER CODE END ADC1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_ADC1_CLK_DISABLE();

    /**ADC1 GPIO Configuration
    PA0/WKUP     ------> ADC1_IN0
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_0);

    /* ADC1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(ADC_IRQn);
  /* USER CODE BEGIN ADC1_MspDeInit 1 */

  /* USER CODE END ADC1_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
void Init_ADC()
{
	  hadc1.Instance = ADC1;
	  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
	  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
	  hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
	  hadc1.Init.ContinuousConvMode = DISABLE;
	  hadc1.Init.DiscontinuousConvMode = DISABLE;
	  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISING;
	  hadc1.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T6_TRGO;
	  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	  hadc1.Init.NbrOfConversion = 1;
	  hadc1.Init.DMAContinuousRequests = DISABLE;
	  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
	  if (HAL_ADC_Init(&hadc1) != HAL_OK)
	  {
	    Error_Handler();
	  }
}
int config_ADC(int pin)
{
	 GPIO_InitTypeDef gpioInit = {0};

	 gpioInit.Pin =gpio_adc_pins[pin];
	 gpioInit.Mode = GPIO_MODE_ANALOG;
	 gpioInit.Pull = GPIO_NOPULL;
	 HAL_GPIO_Init(gpio_adc_ports[pin], &gpioInit);

	 adcChannel.Channel = AdcChannels[pin];
	 adcChannel.Rank =  ADC_REGULAR_RANK_1;
	 adcChannel.SamplingTime = ADC_SAMPLETIME_3CYCLES;

	 __HAL_RCC_ADC1_CLK_ENABLE();
	 switch(pin){
	 	 case 0:
	 	 case 1:
	 	 case 2:
	 	 case 3:
	 	 case 4:
	 	 case 5:
	 	 case 6:
	 	 case 7:
	 		 __HAL_RCC_GPIOA_CLK_ENABLE();
	 		 break;
	 	 case 8:
	 	 case 9:
	 		 __HAL_RCC_GPIOB_CLK_ENABLE();
	 		 break;
	 	 case 10:
	 	 case 11:
	 	 case 12:
	 	 case 13:
	 	 case 14:
	 	 case 15:
	 		 __HAL_RCC_GPIOC_CLK_ENABLE();
	 		 break;
	 	 case 16:
	 		  __HAL_RCC_GPIOD_CLK_ENABLE();
	 		  break;
	 	 default:
	 		 return 0;

	 }
	 HAL_NVIC_SetPriority(ADC_IRQn, 0, 0);
	 HAL_NVIC_EnableIRQ(ADC_IRQn);

	 if (HAL_ADC_ConfigChannel(&hadc1, &adcChannel) != HAL_OK){
		 return 0;
	 }

	 if(HAL_ADC_Start_IT(&hadc1) != HAL_OK){
		 return 0;
	 }

	 return 1;

}

void ADC_Desconfig(int pin){
	__HAL_RCC_ADC1_CLK_DISABLE();
	HAL_GPIO_DeInit(gpio_adc_ports[pin], gpio_adc_pins[pin]);
	HAL_NVIC_DisableIRQ(ADC_IRQn);
}


void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc1)
{
	adc_value = HAL_ADC_GetValue(hadc1);
	int_adc=1;
	  Converter_Valor();

 }
void Converter_Valor()
{
	int_adc=0;
	double temp=0;
	if((Sample_K == 1) && (K_value != 0))
	{
		K_value--;
		Print_Trama(temp);
		temp=((double)adc_value*3.3/4095);
	}
	else if((Sample_K == 1) && (K_value == 0))
		Stop();
	else
	{
		temp=((double)adc_value*3.3/4095);
		Print_Trama(temp);
	}
}

/* USER CODE END 1 */
