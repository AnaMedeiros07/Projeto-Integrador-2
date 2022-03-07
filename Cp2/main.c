/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
#include "main.h"
#include "dac.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include"stdio.h"
#include "stdlib.h"
#include "string.h"
#include "math.h"
#include "output.h"
#include "stdbool.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void init_UART3();
void Init_ADC();
int Check_Comand(uint8_t *buffer);
int Invalid();

int Sampling_Period(uint8_t *buffer1);
int Analog_Channel(uint8_t *buffer1);
void Arrays_Maker(double temp);
void ADC_Desconfig(int pin);
int config_ADC(int pin);

int Start(uint8_t *buffer1);
int Stop();
void Print();

double adc_value;
ADC_HandleTypeDef hadc1;
ADC_ChannelConfTypeDef adcChannel = {0};
GPIO_TypeDef*  Ports[]={GPIOA,GPIOB,GPIOC,GPIOD,GPIOE,GPIOF,GPIOG,GPIOI,GPIOJ,GPIOK};
uint16_t Pins[]={GPIO_PIN_0,GPIO_PIN_1,GPIO_PIN_2,GPIO_PIN_3,GPIO_PIN_4,GPIO_PIN_5,GPIO_PIN_6,GPIO_PIN_7,GPIO_PIN_8,GPIO_PIN_9,GPIO_PIN_10,GPIO_PIN_11,GPIO_PIN_12,GPIO_PIN_13,GPIO_PIN_14,GPIO_PIN_15};
uint32_t AdcChannels[]={ADC_CHANNEL_0,ADC_CHANNEL_1,ADC_CHANNEL_2,ADC_CHANNEL_3,ADC_CHANNEL_4,ADC_CHANNEL_5,ADC_CHANNEL_6,ADC_CHANNEL_7,ADC_CHANNEL_8,ADC_CHANNEL_9,ADC_CHANNEL_10,ADC_CHANNEL_11,ADC_CHANNEL_12,ADC_CHANNEL_13,ADC_CHANNEL_14,ADC_CHANNEL_15,ADC_CHANNEL_TEMPSENSOR,ADC_CHANNEL_17,ADC_CHANNEL_18};
uint16_t gpio_adc_pins[]={GPIO_PIN_0,GPIO_PIN_1,GPIO_PIN_2,GPIO_PIN_3,GPIO_PIN_4,GPIO_PIN_5,GPIO_PIN_6,GPIO_PIN_7,GPIO_PIN_0,GPIO_PIN_1,GPIO_PIN_0,GPIO_PIN_1,GPIO_PIN_2,GPIO_PIN_3,GPIO_PIN_4,GPIO_PIN_5};
GPIO_TypeDef* gpio_adc_ports[]={GPIOA,GPIOA,GPIOA,GPIOA,GPIOA,GPIOA,GPIOA,GPIOA,GPIOB,GPIOB,GPIOC,GPIOC,GPIOC,GPIOC,GPIOC,GPIOC};
int adc_validation = 0;
int valid=1;
uint8_t b =0x20;
_Bool output = 0;
int index_count = 0;

_Bool Sample_K = 0;
int K_value = 0;
int prompt_flag;


/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART3_UART_Init();
  MX_TIM6_Init();
  MX_DAC_Init();
  /* USER CODE BEGIN 2 */
  init_UART3();
  Init_ADC();
  receive_flag=0;
  prompt_flag = 1;

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  if(prompt_flag == 1)
	  {
		  Write_Tx_Buffer(">", 1);
		  transmite_flag=1;
		  Print();
	  }
	  prompt_flag = 0;


	  if(receive_flag==1)
	  {
		  prompt_flag = 1;
		  Write_Tx_Buffer(Rx_Buffer, 0);
		  transmite_flag=1;
		  Print();
		  receive_flag=0;
		  transmite_flag = Check_Comand(Rx_Buffer);
		  Print();
		  if(valid==1)
			  Data_Reset();
		  else
			  valid=1;
		  Limpar_Rx_Buffer();
	  }

	  if(output == 1){
		  char result[5];
		Write_Tx_Buffer("n", 2);
		Write_Tx_Buffer(itoa(index_count,NULL,10),1);
		Write_Tx_Buffer("va",2);
		snprintf(result, 5, "%f", X_Buffer[index_count]);
		Write_Tx_Buffer(result,1);
		Write_Tx_Buffer("vf",2);
		snprintf(result, 5, "%f", Y_Buffer[index_count]);
		Write_Tx_Buffer(result,0);
		transmite_flag=1;
		Print();
		output = 0;
	  }
  }
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 216;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Activate the Over-Drive mode
  */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

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
int Check_Comand(uint8_t *buffer)
 {
	if ((buffer[0] == 'S' && buffer[1]=='P') || (buffer[0] == 's' && buffer[1]=='p'))
	{
		return Sampling_Period(buffer);
	}
	else if((buffer[0] == 'S' && buffer[1] == 'T' ) || (buffer[0] == 's' && buffer[1] == 't'))
	{
		return Stop();
	}
	else if((buffer[0] == 'A' && buffer[1] == 'C' ) || (buffer[0] == 'a' && buffer[1] == 'c'))
	{
		return Analog_Channel(buffer);
	}
	else if(buffer[0] == 'S' || buffer[0] == 's')
	{
		return Start(buffer);
	}
	else if((buffer[0] == 'F' && buffer[1] == 'F' ) || (buffer[0] == 'f' && buffer[1] == 'f'))
	{
		filter_on=0;
		return 1;
	}
	else if((buffer[0] == 'F' && buffer[1] == 'O' ) || (buffer[0] == 'f' && buffer[1] == 'o'))
	{
		filter_on=1;
		return 1;
	}
	else if(buffer[0] == '\0')
	{
		Write_Tx_Buffer("Insira um comando",0);
		return 1;
	}

	return Invalid();
 }

int Invalid()
{
	Write_Tx_Buffer("Comando Invalido",0);
	transmite_flag=1;
	valid=0;
	Print();
	return 0;
}

void Print(){
	 if(transmite_flag == 1){
		Tx_Transmition();
		while(transmite_flag == 1){}
	 }
}

int Analog_Channel(uint8_t *buffer1)
{
	char channel[4] = {"0"};
	int counter=0, aux_counter=0, counter_space=0;
	long channel_num = 0;

	while(buffer1[counter]!='\0')
	{
		if(buffer1[counter]==b)
		{
			++counter_space;
			aux_counter=0;
		}
		else if(counter_space==1)
			channel[aux_counter++]=buffer1[counter];
		counter++;
	}

	if(counter_space != 1)
		return 0;

	ADC_Desconfig(channel_num);

	channel_num = strtol(channel, NULL, 16);

	if(config_ADC(channel_num) == 0)
		return 0;
	return 1;
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
	uint32_t adc_value = 0;

	if((Sample_K == 1) && (K_value != 0)){
		K_value--;
		adc_value = HAL_ADC_GetValue(hadc1);
		if( adcChannel.Channel == ADC_CHANNEL_TEMPSENSOR)
		{
			adc_value=((((double)adc_value*3300/4095)-760.0)/2.5)+25;
		}
		else
		{
			 adc_value=((double)adc_value*3.3/4095);
		}
	}
	else if((Sample_K == 1) && (K_value == 0))
		Stop();
	else {
		adc_value = HAL_ADC_GetValue(hadc1);
		if( adcChannel.Channel == ADC_CHANNEL_TEMPSENSOR)
		{
			adc_value=((((double)adc_value*3300/4095)-760.0)/2.5)+25;
		}
		else
		{
			 adc_value=((double)adc_value*3.3/4095);
		}
	}
	HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_7);
	Arrays_Maker(adc_value);
 }

int Sampling_Period(uint8_t *buffer1)
{
	char time_unit[4] = {"\0"}, units[4]={"\0"};
	int counter=0, aux_counter=0, counter_space=0,prescaler=0;
	float period=0;
	long int time_6_clock = 0;
   //--------------------------------------------
	while(buffer1[counter]!='\0')
	{
		if(buffer1[counter]==b)
		{
			++counter_space;
			aux_counter=0;
		}
		else if(counter_space==1)
			time_unit[aux_counter++]=buffer1[counter];
		else if(counter_space==2)
			units[aux_counter++]=buffer1[counter];
		counter++;
	}
	if(counter_space!=2)
		return 0;
	//---------------------------------------------

	if(!strcmp(time_unit,"s"))
	{
		period=atof(units);
	}
	else if(!strcmp(time_unit,"ms"))
	{
		period=atof(units)*0.001;
	}
	else if(!strcmp(time_unit,"micro"))
	{
		period=atof(units)*0.000001;
	}

	time_6_clock = (65536/period);
	prescaler = ((216000000/time_6_clock));
	Timer_Configuration(prescaler);
	return 1;
}

int Start(uint8_t *buffer1)
{
	char Kvalues[4] = {"\0"};
	int counter=0, aux_counter=0, counter_space=0;

	while(buffer1[counter]!='\0')
	{
		if(buffer1[counter]==b)
		{
			++counter_space;
			aux_counter=0;
		}
		else if(counter_space==1)
			Kvalues[aux_counter++]=buffer1[counter];
		counter++;
	}

	if(counter_space == 0)
		Sample_K = 0;
	else
	{
		K_value = atoi(Kvalues);
		Sample_K = 1;
	}

	HAL_TIM_Base_Start_IT(&htim6);
	prompt_flag = 0;

	return 0;

}

int Stop(){
	 Reset();
	 HAL_TIM_Base_Stop_IT(&htim6);

	 prompt_flag = 1;
}

void Arrays_Maker(double temp)
{
	index_count=(Save_N_Buffer()-1);

	Save_X_Buffer(temp);
	Save_X_ant();
	Save_Y_ant();

	output = 1;

	/* if(HAL_DAC_GetState(&hdac)!= HAL_DAC_STATE_READY)
			  HAL_DAC_Stop(&hdac, DAC1_CHANNEL_1);
	  if(HAL_DAC_Start(&hdac, DAC1_CHANNEL_1)== HAL_OK)
	  {
		 HAL_DAC_SetValue(&hdac, DAC1_CHANNEL_1,DAC_ALIGN_12B_R,((Save_Y()*4095)/3.3));
	  }*/
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

