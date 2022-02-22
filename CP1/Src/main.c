
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
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include"stdio.h"
#include "stdlib.h"
#include "string.h"
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
char help[]="MR = Memory Read\n"
		    "MW = Memory Write"
			"MI=Make port pin Input\n";

GPIO_TypeDef*  Ports[]={GPIOA,GPIOB,GPIOC,GPIOD,GPIOE,GPIOF,GPIOG,GPIOI,GPIOJ,GPIOK};
uint16_t Pins[]={GPIO_PIN_0,GPIO_PIN_1,GPIO_PIN_2,GPIO_PIN_3,GPIO_PIN_4,GPIO_PIN_5,GPIO_PIN_6,GPIO_PIN_7,GPIO_PIN_8,GPIO_PIN_9,GPIO_PIN_10,GPIO_PIN_11,GPIO_PIN_12,GPIO_PIN_13,GPIO_PIN_14,GPIO_PIN_15};
uint8_t b =0x20;
uint8_t esc =0x1B;
uint8_t bckspc =0x08;


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

  init_UART3();
  receive_flag=0;
  int prompt_flag = 1;
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART3_UART_Init();
  init_UART3();
  receive_flag=0;
  /* USER CODE BEGIN 2 */


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  if(prompt_flag == 1)
	  {
		  Write_Tx_Buffer(">", 1);
		  transmite_flag=1;
		  if(transmite_flag == 1){
			 Tx_Transmition();
			 while(transmite_flag == 1){}
		  }
		  prompt_flag = 0;
	  }


	  if(receive_flag==1)
	  {
		  prompt_flag = 1;
		  Write_Tx_Buffer(Rx_Buffer, 0);
		  transmite_flag=1;
		  if(transmite_flag == 1){
			 Tx_Transmition();
			 while(transmite_flag == 1){}
		  }
		  receive_flag=0;
		  transmite_flag = validar_comando(Rx_Buffer);
		  if(transmite_flag == 1){
			 Tx_Transmition();
			 while(transmite_flag == 1){}
		  }

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

	}
	else if(buffer[0] == b)
	{

	}
	else if(buffer[0] == esc)
	{

	}
	else if(buffer[0] == '$')
	{

	}
	else if(buffer[0] == 'V' && buffer[1]=='E' && buffer[2]=='R')
	{

	}
	else if(buffer[0] == '?')
	{
		 Write_Tx_Buffer(help, 0);
		 return 1;
	}
	return 0;
 }

int Memory_read(uint8_t *buffer1)
{
	char addr[2];
	char lenght[2];
	char *ptr_teste1;
	long *ptr;

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

	long hex_addr = strtol(addr, &ptr_teste1, 16);
	long value = strtol(lenght, &ptr_teste1, 16);
	ptr = hex_addr;

	while(value>0)
	{
		itoa(*ptr, ptr_teste1, 16);
		Write_Tx_Buffer(ptr_teste1, 0);
		ptr++;
		value--;
	}
	return 1;
}

int Memory_write(uint8_t *buffer1){
	char addr[4], lenght[2], byte[2], *message, Mensagem[] = "Memory Write";

	int counter = 0, aux_counter = 0, counter_space = 0;
	long addr_ = 0, lenght_ = 0, byte_ = 0, *ptr;

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
	byte_ = strtol(byte, &message,16);
	ptr = addr_;

	while(lenght_> 0){
		*ptr = byte_;
		ptr++;
		lenght_--;
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
		if(binary[counter] == '1'){
			itoa(HAL_GPIO_ReadPin(Ports[hex_addr], Pins[aux_counter]), Message, 10);
			Write_Tx_Buffer(Message, 1);
		}
		else{
			Write_Tx_Buffer("0", 1);
		}
		aux_counter--;
		counter++;
	}
	Write_Tx_Buffer("\n",0);
	return 1;
}

int Write_Dig_Output(uint8_t *buffer1){

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
