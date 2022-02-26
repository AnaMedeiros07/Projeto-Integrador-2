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
#include "adc.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include"stdio.h"
#include "stdlib.h"
#include "string.h"
#include "math.h"
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
int validar_comando(uint8_t *buffer);
int Memory_read(uint8_t *buffer1);
int Memory_write(uint8_t *buffer1);
int Make_Pin_Input(uint8_t *buffer1);
int Make_Pin_Output(uint8_t *buffer1);
int Read_Dig_Input(uint8_t *buffer1);
int Analog_Read(uint8_t *buffer1);
int Version();
int Help();
void Print();
int config_ADC (int pin);
void ADC_Desconfig(int pin);

ADC_HandleTypeDef hadc1;
ADC_ChannelConfTypeDef adcChannel = {0};
GPIO_TypeDef*  Ports[]={GPIOA,GPIOB,GPIOC,GPIOD,GPIOE,GPIOF,GPIOG,GPIOI,GPIOJ,GPIOK};
uint16_t Pins[]={GPIO_PIN_0,GPIO_PIN_1,GPIO_PIN_2,GPIO_PIN_3,GPIO_PIN_4,GPIO_PIN_5,GPIO_PIN_6,GPIO_PIN_7,GPIO_PIN_8,GPIO_PIN_9,GPIO_PIN_10,GPIO_PIN_11,GPIO_PIN_12,GPIO_PIN_13,GPIO_PIN_14,GPIO_PIN_15};
uint32_t AdcChannels[]={ADC_CHANNEL_0,ADC_CHANNEL_1,ADC_CHANNEL_2,ADC_CHANNEL_3,ADC_CHANNEL_4,ADC_CHANNEL_5,ADC_CHANNEL_6,ADC_CHANNEL_7,ADC_CHANNEL_8,ADC_CHANNEL_9,ADC_CHANNEL_10,ADC_CHANNEL_11,ADC_CHANNEL_12,ADC_CHANNEL_13,ADC_CHANNEL_14,ADC_CHANNEL_15,ADC_CHANNEL_TEMPSENSOR,ADC_CHANNEL_17,ADC_CHANNEL_18};
uint16_t gpio_adc_pins[]={GPIO_PIN_0,GPIO_PIN_1,GPIO_PIN_2,GPIO_PIN_3,GPIO_PIN_4,GPIO_PIN_5,GPIO_PIN_6,GPIO_PIN_7,GPIO_PIN_0,GPIO_PIN_1,GPIO_PIN_0,GPIO_PIN_1,GPIO_PIN_2,GPIO_PIN_3,GPIO_PIN_4,GPIO_PIN_5};
GPIO_TypeDef* gpio_adc_ports[]={GPIOA,GPIOA,GPIOA,GPIOA,GPIOA,GPIOA,GPIOA,GPIOA,GPIOB,GPIOB,GPIOC,GPIOC,GPIOC,GPIOC,GPIOC,GPIOC};
int adc_validation = 0;
uint8_t b =0x20;
uint16_t memory_buffer[1024];



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
  MX_ADC1_Init();
  /* USER CODE BEGIN 2 */
  init_UART3();
  receive_flag=0;
  int prompt_flag = 1;

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
		  transmite_flag = validar_comando(Rx_Buffer);
		  Print();
		  Limpar_Rx_Buffer();

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
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

int validar_comando(uint8_t *buffer)
 {
	if(buffer[0] == 'M' && buffer[1] == 'R'){
		return Memory_read(buffer);
	}
	else if(buffer[0] == 'M' && buffer[1] == 'W'){
		return Memory_write(buffer);
	}
	else if(buffer[0] == 'M' && buffer[1] == 'I'){
		return Make_Pin_Input(buffer);
	}
	else if(buffer[0] == 'M' && buffer[1] == 'O'){
		return Make_Pin_Output(buffer);
	}
	else if(buffer[0] == 'R' && buffer[1] == 'D'){
		return Read_Dig_Input(buffer);
	}
	else if(buffer[0] == 'W' && buffer[1] == 'D'){
		return Write_Dig_Output(buffer);
	}
	else if(buffer[0] == 'R' && buffer[1] == 'A')
	{
		return Analog_Read(buffer);
	}
	else if(buffer[0] == 'V' && buffer[1]=='E' && buffer[2]=='R')
	{
		return Version();
	}
	else if(buffer[0] == '?')
	{
		 return Help();
	}
	return 0;
 }

int Memory_read(uint8_t *buffer1)
{
	char addr[2];
	char lenght[2];
	char ptr_teste1;
	int counter=0;
	int aux_counter=0;
	int counter_space=0;

	while(buffer1[counter]!='\0')
	{
		if(buffer1[counter]==b)
		{
			++counter_space;
			aux_counter=0;
		}
		else if(counter_space==1)
			addr[aux_counter++]=buffer1[counter];

		else if(counter_space==2)
			lenght[aux_counter++]=buffer1[counter];
		++counter;
	}

	counter = 0;
	counter_space = 0;
	aux_counter = 0;

	long hex_addr = strtol(addr, NULL, 16);
	long value = strtol(lenght, NULL, 16);
	char aux;
	while(value>0)
	{
		itoa(memory_buffer[hex_addr], ptr_teste1, 16);
		Write_Tx_Buffer(memory_buffer[hex_addr], 0);
		hex_addr++;
		value--;
	}
	return 1;
}

int Memory_write(uint8_t *buffer1){
	char addr[4], lenght[2], byte[2], *message, Mensagem[] = "Memory Write";

	int counter = 0, aux_counter = 0, counter_space = 0;
	long addr_ = 0, lenght_ = 0;
	uint16_t byte_ = 0;

	while(buffer1[counter]!='\0'){
		if(buffer1[counter]==b){
			++counter_space;
			aux_counter=0;
		}
		else if(counter_space==1)
			addr[aux_counter++]=buffer1[counter];
		else if(counter_space==2)
			lenght[aux_counter++]=buffer1[counter];
		else if(counter_space == 3)
			byte[aux_counter++] = buffer1[counter];
		++counter;
	}

	counter = 0;
	aux_counter = 0;
	counter_space = 0;

	addr_ = strtol(addr, &message,16);
	lenght_ = strtol(lenght, &message, 16);
	strcpy(byte_,byte);

	while(lenght_> 0){
		memory_buffer[addr_]=byte_;
		addr_++;
		lenght_--;
		++counter;
	}

	Write_Tx_Buffer(Mensagem, 0);

	return 1;
}

int Make_Pin_Input(uint8_t *buffer1){
	char addr[4] = {"0"}, *Message = "Make_Pin_Input";
	char lenght1[4] = {"0"}, lenght2[4] = {"0"}, lenght3[4] = {"0"};
	char *binary;
	int counter=0, aux_counter=0, counter_space=0;
	long hex_addr = 0, value1 = 0, value2 = 0, value = 0;

	while(buffer1[counter]!='\0')
	{
		if(buffer1[counter]==b)
		{
			++counter_space;
			aux_counter=0;
		}
		else if(counter_space==1)
			addr[aux_counter++]=buffer1[counter];

		else if(counter_space==2)
			lenght1[aux_counter++]=buffer1[counter];
		++counter;
	}

	counter = 0;
	counter_space = 0;

	hex_addr = strtol(addr, &binary, 10);
	if((aux_counter = strlen(lenght1)) <=2)
		value = strtol(lenght1, &binary, 16);
	else{
		lenght2[0] = lenght1[0];
		lenght2[1] = lenght1[1];
		lenght3[0] = lenght1[2];
		lenght3[1] = lenght1[3];

		value1 = strtol(lenght2, &binary, 16);
		value2 = strtol(lenght3, &binary, 16);
		value = value1 << 8;
		value |= value2;
	}

	itoa(value, binary, 2);
	aux_counter = strlen(binary)-1;

	while(aux_counter >= 0){
		if(binary[counter] == '1'){
			GPIO_InitTypeDef GPIO_InitStruct = {0};
			__HAL_RCC_GPIOD_CLK_ENABLE();
			__HAL_RCC_GPIOB_CLK_ENABLE();

			GPIO_InitStruct.Pin = Pins[aux_counter];
			GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
			HAL_GPIO_Init(Ports[hex_addr], &GPIO_InitStruct);
		}
		aux_counter--;
		counter++;
	}
	Write_Tx_Buffer(Message,0);
	return 1;

}

int Make_Pin_Output(uint8_t *buffer1){
	char addr[4] = {"0"}, *Message = "Make_Pin_Output";
	char lenght1[4] = {"0"}, lenght2[4] = {"0"}, lenght3[4] = {"0"};
	char *binary;
	int counter=0, aux_counter=0, counter_space=0;
	unsigned long int hex_addr = 0, value1 = 0, value2 = 0, value = 0;

	while(buffer1[counter]!='\0')
	{
		if(buffer1[counter]==b)
		{
			++counter_space;
			aux_counter=0;
		}
		else if(counter_space==1)
			addr[aux_counter++]=buffer1[counter];

		else if(counter_space==2)
			lenght1[aux_counter++]=buffer1[counter];
		++counter;
	}

	counter = 0;
	counter_space = 0;

	hex_addr = strtol(addr, &binary, 10);
	if((aux_counter = strlen(lenght1)) <=2)
		value = strtol(lenght1, &binary, 16);
	else{
		lenght2[0] = lenght1[0];
		lenght2[1] = lenght1[1];
		lenght3[0] = lenght1[2];
		lenght3[1] = lenght1[3];

		value1 = strtol(lenght2, &binary, 16);
		value2 = strtol(lenght3, &binary, 16);
		value = value1 << 8;
		value |= value2;
	}

	itoa(value, binary, 2);
	aux_counter = strlen(binary)-1;

	while(aux_counter >= 0){
		if(binary[counter] == '1'){
			GPIO_InitTypeDef GPIO_InitStruct = {0};
			__HAL_RCC_GPIOD_CLK_ENABLE();
			__HAL_RCC_GPIOB_CLK_ENABLE();

			GPIO_InitStruct.Pin = Pins[aux_counter];
			GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
			HAL_GPIO_Init(Ports[hex_addr], &GPIO_InitStruct);
		}
		aux_counter--;
		counter++;
	}
	Write_Tx_Buffer(Message,0);
	return 1;

}

int Read_Dig_Input(uint8_t *buffer1){
	char addr[4] = {"0"}, *Message;
	char lenght1[4] = {"0"}, lenght2[4] = {"0"}, lenght3[4] = {"0"};
	char *binary;
	int counter=0, aux_counter=0, counter_space=0;
	long hex_addr = 0, value1 = 0, value2 = 0, value = 0;

	while(buffer1[counter]!='\0')
	{
		if(buffer1[counter]==b)
		{
			++counter_space;
			aux_counter=0;
		}
		else if(counter_space==1)
			addr[aux_counter++]=buffer1[counter];

		else if(counter_space==2)
			lenght1[aux_counter++]=buffer1[counter];
		++counter;
	}

	counter = 0;
	counter_space = 0;

	hex_addr = strtol(addr, &binary, 16);
	if((aux_counter = strlen(lenght1)) <=2)
		value = strtol(lenght1, &binary, 16);
	else{
		lenght2[0] = lenght1[0];
		lenght2[1] = lenght1[1];
		lenght3[0] = lenght1[2];
		lenght3[1] = lenght1[3];

		value1 = strtol(lenght2, &binary, 16);
		value2 = strtol(lenght3, &binary, 16);
		value = value1 << 8;
		value |= value2;
	}

	itoa(value, binary, 2);
	aux_counter = strlen(binary)-1;

	while(aux_counter >= 0){
		itoa(HAL_GPIO_ReadPin(Ports[hex_addr], Pins[aux_counter]), Message, 10);
		Write_Tx_Buffer(Message, 1);
		aux_counter--;
		counter++;
	}
	Write_Tx_Buffer("\n",0);
	return 1;
}

int Write_Dig_Output(uint8_t *buffer1){
	char addr[4] = {"0"}, lenght[4] = {"0"}, bits[4] = {"0"};
	char lenght1[2] = {"0"},lenght2[2] = {"0"}, bits1[2] = {"0"}, bits2[2] = {"0"};
	char *t;
	char *Message = "Write Digital Output";
	int counter=0, aux_counter=0, counter_space=0;
	int aux_counter1 = 0;
	long hex_addr = 0, value = 0, bits_ = 0;
	long value1 = 0, value2 = 0, bits_1 = 0, bits_2 = 0;

	while(buffer1[counter]!='\0')
	{
		if(buffer1[counter]==b)
		{
			++counter_space;
			aux_counter=0;
		}
		else if(counter_space==1)
			addr[aux_counter++]=buffer1[counter];

		else if(counter_space==2)
			lenght[aux_counter++]=buffer1[counter];
		else if(counter_space == 3)
			bits[aux_counter++]=buffer1[counter];
		++counter;
	}

	counter = 0;
	counter_space = 0;

	hex_addr = strtol(addr, &t, 16);

	if((aux_counter = strlen(lenght)) <=2)
		value = strtol(lenght, &t, 16);
	else{
		lenght1[0] = lenght[0];
		lenght1[1] = lenght[1];
		lenght2[0] = lenght[2];
		lenght2[1] = lenght[3];

		value1 = strtol(lenght1, &t, 16);
		value2 = strtol(lenght2, &t, 16);
		value = value1 << 8;
		value |= value2;
	}

	if((aux_counter1 = strlen(bits)) <=2)
		bits_ = strtol(bits, &t, 16);
	else{
		bits1[0] = bits[0];
		bits1[1] = bits[1];
		bits2[0] = bits[2];
		bits2[1] = bits[3];

		bits_1 = strtol(bits1, &t, 16);
		bits_2 = strtol(bits2, &t, 16);
		bits_ = bits_1 << 8;
		bits_|= bits_2;
	}

unsigned long int shift1 ;

	aux_counter = 15;
	int shift;

	while(aux_counter >= 0){
		shift =value >> aux_counter;
		if(shift & 1){
			shift1=bits_>>aux_counter;
			if( shift1 & 1)
					HAL_GPIO_WritePin(Ports[hex_addr], Pins[aux_counter],  GPIO_PIN_SET);
			else
				HAL_GPIO_WritePin(Ports[hex_addr], Pins[aux_counter], GPIO_PIN_RESET );
		}
		counter++;
		aux_counter--;
	}
	Write_Tx_Buffer(Message, 0);
	return 1;
}

int Version(){
	Write_Tx_Buffer("v1.6 grupo 2 PIEEIC-II DEIC UM2022",0);
	return 1;
}

int Help(){
	Write_Tx_Buffer("MR -> Memory Read", 0);
	transmite_flag=1;
	Print();

	Write_Tx_Buffer("MW -> Memory Write",0);
	transmite_flag=1;
	Print();

	Write_Tx_Buffer("MI -> Make Port Pin Input",0);
	transmite_flag=1;
	Print();

	Write_Tx_Buffer("MO -> Make Port Pin Output",0);
	transmite_flag=1;
	Print();

	Write_Tx_Buffer("RD -> Read Digital Input",0);
	transmite_flag=1;
	Print();

	Write_Tx_Buffer("WD -> Write Digital Output",0);
	transmite_flag=1;
	Print();

	Write_Tx_Buffer("BackSpace -> Limpa ultimo caracter recebido", 0);
	transmite_flag=1;
	Print();

	Write_Tx_Buffer("ESC -> Limpa todos caracteres recebidos", 0);
	transmite_flag=1;
	Print();

	Write_Tx_Buffer("$ -> Limpa todos os caracteres recebidos e repete o ultimo comando válido", 0);
	transmite_flag=1;
	Print();

	Write_Tx_Buffer("VER -> Ver informacoes do projeto", 0);
	transmite_flag=1;
	Print();

	return 0;
}

void Print(){
	 if(transmite_flag == 1){
		Tx_Transmition();
		while(transmite_flag == 1){}
	 }
}
int Analog_Read(uint8_t *buffer1)
{
		char addr[4] = {"0"};
		int counter=0, aux_counter=0, counter_space=0;
		long hex_addr = 0;

		while(buffer1[counter]!='\0')
		{
			if(buffer1[counter]==b)
			{
				++counter_space;
				aux_counter=0;
			}
			else if(counter_space==1)
				addr[aux_counter++]=buffer1[counter];
			counter++;
		}

		counter = 0;
		counter_space = 0;

		hex_addr = strtol(addr, NULL, 16);

		if(config_ADC(hex_addr) == 0)
			return 0;
		while(adc_validation != 1);
		ADC_Desconfig(hex_addr);
		return 1;
}
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc1)
{
	uint32_t adc_value = 0;
	char result[5];
	double temp;
	adc_value = HAL_ADC_GetValue(hadc1);


	if( adcChannel.Channel == ADC_CHANNEL_TEMPSENSOR)
	{
		temp=((((double)adc_value*3300/4095)-760.0)/2.5)+25;
		snprintf(result, 5, "%f", temp);
	}
	else
	{
		 temp=((double)adc_value*3.3/4095);
		 snprintf(result, 5, "%f", temp);
	}
	Write_Tx_Buffer(result, 0);
	adc_validation = 1;

 }
int config_ADC(int pin)
{
	 GPIO_InitTypeDef gpioInit = {0};

	  hadc1.Instance = ADC1;
	  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
	  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
	  hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
	  hadc1.Init.ContinuousConvMode = DISABLE;
	  hadc1.Init.DiscontinuousConvMode = DISABLE;
	  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
	  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
	  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	  hadc1.Init.NbrOfConversion = 1;
	  hadc1.Init.DMAContinuousRequests = DISABLE;
	  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
	  if (HAL_ADC_Init(&hadc1) != HAL_OK)
	  {
	    return 0;
	  }

	 adcChannel.Channel = AdcChannels[pin];
	 adcChannel.Rank =  ADC_REGULAR_RANK_1;
	 adcChannel.SamplingTime = ADC_SAMPLETIME_3CYCLES;
	 adcChannel.Offset = 0;

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
	 gpioInit.Pin =gpio_adc_pins[pin];
	 gpioInit.Mode = GPIO_MODE_ANALOG;
	 gpioInit.Pull = GPIO_NOPULL;
	 HAL_GPIO_Init(gpio_adc_ports[pin], &gpioInit);

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

