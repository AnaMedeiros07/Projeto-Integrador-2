/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usart.c
  * @brief   This file provides code for the configuration
  *          of the USART instances.
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
#include "usart.h"

/* USER CODE BEGIN 0 */
#include "stdio.h"
#include "stdlib.h"
#include "stdbool.h"
#include "string.h"
#define esc 0x1B
#define bckspc 0x08
#define $ 0x24
	uint8_t UART3Rx_Buffer[256]; //guarda as mensagens para a main;
	uint8_t Rx_Buffer[256];
    uint8_t	Rx_Buffer_Old[256];
	char UART3Tx_Buffer[256];
	uint8_t Tx_Buffer[256];
	int receive_flag= 0;
	int tamanho;
	uint8_t index_ = 0;
	volatile uint8_t UART3Rx_index=0;// index do ultimo caracter
	int transmite_flag =0;

/* USER CODE END 0 */

UART_HandleTypeDef huart3;

/* USART3 init function */

void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  huart3.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart3.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

}

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
  if(uartHandle->Instance==USART3)
  {
  /* USER CODE BEGIN USART3_MspInit 0 */

  /* USER CODE END USART3_MspInit 0 */
  /** Initializes the peripherals clock
  */
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USART3;
    PeriphClkInitStruct.Usart3ClockSelection = RCC_USART3CLKSOURCE_PCLK1;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
    {
      Error_Handler();
    }

    /* USART3 clock enable */
    __HAL_RCC_USART3_CLK_ENABLE();

    __HAL_RCC_GPIOD_CLK_ENABLE();
    /**USART3 GPIO Configuration
    PD8     ------> USART3_TX
    PD9     ------> USART3_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART3;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    /* USART3 interrupt Init */
    HAL_NVIC_SetPriority(USART3_IRQn, 2, 0);
    HAL_NVIC_EnableIRQ(USART3_IRQn);
  /* USER CODE BEGIN USART3_MspInit 1 */

  /* USER CODE END USART3_MspInit 1 */
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

  if(uartHandle->Instance==USART3)
  {
  /* USER CODE BEGIN USART3_MspDeInit 0 */

  /* USER CODE END USART3_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART3_CLK_DISABLE();

    /**USART3 GPIO Configuration
    PD8     ------> USART3_TX
    PD9     ------> USART3_RX
    */
    HAL_GPIO_DeInit(GPIOD, GPIO_PIN_8|GPIO_PIN_9);

    /* USART3 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART3_IRQn);
  /* USER CODE BEGIN USART3_MspDeInit 1 */

  /* USER CODE END USART3_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
void init_UART3()
	{
		HAL_UART_Receive_IT(&huart3 , &UART3Rx_Buffer[UART3Rx_index] , 1);
		receive_flag=0;
		transmite_flag=0;
	}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef* huart) // função de interrupção da uart que guarda os comando no Rx_Buffer
{
    if(huart->Instance == USART3){
        if(receive_flag == 0){
            if((UART3Rx_Buffer[UART3Rx_index] != '\r')&&(UART3Rx_Buffer[UART3Rx_index] != '\n')&&(UART3Rx_Buffer[UART3Rx_index] != bckspc) && (UART3Rx_Buffer[UART3Rx_index] != esc)&&(UART3Rx_Buffer[UART3Rx_index] != $)){
                Rx_Buffer[UART3Rx_index] = UART3Rx_Buffer[UART3Rx_index];
                UART3Rx_index++;
                UART3Rx_index &= ~(1<<7);
                HAL_UART_Receive_IT(&huart3, &UART3Rx_Buffer[UART3Rx_index], 1);
            }
            else if(UART3Rx_Buffer[UART3Rx_index] == bckspc) // limpa do array o caracter recebido
            {
            	Rx_Buffer[--UART3Rx_index]='\0';
                HAL_UART_Receive_IT(&huart3, &UART3Rx_Buffer[UART3Rx_index], 1);

            }
            else if(UART3Rx_Buffer[UART3Rx_index] == $) // limpa do array todos os caracteres recebidos e
            											// repete o último comando válido
            {
            	Limpar_Tx_Buffer();
            	Limpar_Rx_Buffer();
            	strcpy(Rx_Buffer,Rx_Buffer_Old);		// copiamos do Rx_Buffer_Old(guarda todos os comados válidos)
            											// para o Rx_Buffer
                HAL_UART_Receive_IT(&huart3, &UART3Rx_Buffer[UART3Rx_index], 1);
            }
            else if(UART3Rx_Buffer[UART3Rx_index] == esc) // limpa do array todos os caracteres recebidos
            {
            	Limpar_Tx_Buffer();
            	Limpar_Rx_Buffer();
                HAL_UART_Receive_IT(&huart3, &UART3Rx_Buffer[UART3Rx_index], 1);

            }
            else{
                receive_flag = 1;
                UART3Rx_index = 0;
                HAL_UART_Receive_IT(&huart3, &UART3Rx_Buffer[UART3Rx_index], 1);
            }
        }
    }
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart){
	if(huart->Instance == USART3){
		if(transmite_flag == 1){
			if(Tx_Buffer[index_] != '\0'){
				HAL_UART_Transmit_IT(&huart3, &Tx_Buffer[index_], 1);
				index_++;
				index_ &= ~(1<<7);
			}
			else{
				transmite_flag = 0;
				Limpar_Tx_Buffer();
			}
		}
	}
}

void Write_Tx_Buffer(char *pdata, int flag){ //Escreve no Tx_Buffer os caracteres a enviar para o terminal
	int local_index = 0;
	while(pdata[local_index] != '\0'){
		Tx_Buffer[tamanho] = pdata[local_index];
		local_index++;
		tamanho++;
	}
	if(flag == 0)
		Tx_Buffer[tamanho++] = '\r';
	else
		Tx_Buffer[tamanho++] = ' ';

	local_index = 0;
}

void Tx_Transmition(){
	if(Tx_Buffer[index_] != '\0'){
		HAL_UART_Transmit_IT(&huart3,&Tx_Buffer[index_], 1);
		index_++;
	}
}

void Limpar_Tx_Buffer(){ // Limpa o array Tx_Buffer
	int local_index = 0;
	tamanho = 0;
	index_ = 0;
	while(Tx_Buffer[local_index] != '\0'){
		Tx_Buffer[local_index++] = '\0';
	}
}

void Limpar_Rx_Buffer(){ // Limpa o array Rx_Buffer
	int local_index=0;
	while(Rx_Buffer[local_index] != '\0'){
		Rx_Buffer[local_index++] = '\0';
	}
}
void Limpar_Rx_Buffer_Old() // Limpa o array Rx_Buffer_Old
{
	int local_index=0;
		while(Rx_Buffer_Old[local_index] != '\0'){
				Rx_Buffer_Old[local_index++] = '\0';
			}
}

void Data_Reset(){ // Atualiza Rx_Buffer_Old(guarda os comandos antigos) caso o comando seja válido
    Limpar_Rx_Buffer_Old();
    strcpy(Rx_Buffer_Old,Rx_Buffer);
}
/* USER CODE END 1 */
