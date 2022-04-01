/*
 * PWM.c
 *
 *  Created on: 24/03/2022
 *      Author: Ines
 */

#include "PWM.h"
int duty_cycle;

void Init_PWM_Control (){
	HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_1);
}

void DeInit_PWM_Control (){
	HAL_TIM_PWM_Stop(&htim4,TIM_CHANNEL_2);
	duty_cycle = 0;
	Change_Duty();
}

_Bool Normalized_Tension(uint8_t *buffer1){
	_Bool return_flag, Validation;
	int row_number = 0;
	char string_array[6][6];

	clean_string_array(string_array);
	row_number = Parsing(buffer1, string_array);

	if(row_number != 1){
		Write_Tx_Buffer("Dados inseridos invalidos!! Escreva novamente...", 0);
		return invalid;
	}

	duty_cycle = atoi(string_array[row_number], NULL, 10);
	Validation = Change_Duty();
	if(Validation == 0){
		Write_Tx_Buffer("Valores fora dos limites!!", 0);
		return_flag = invalid;
	}

	return return_flag;
}

void Increment_Duty(){
	duty_cycle +=5;
	Change_Duty();
}

void Decrement_Duty(){
	duty_cycle -=5;
	Change_Duty();
}

_Bool Change_Duty(){
	_Bool return_flag;

	if((duty_cycle >= -100) && (duty_cycle < 0)){
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, 0);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, 1);
		Direction = Anti_Clock;
		float duty = (float)(duty_cycle*(-1))/100;
		htim4.Instance->CCR1 = duty*(htim4.Instance->ARR);
		return_flag = Valid;
	}
	else if((duty_cycle >= 0) && (duty_cycle <= 100)){
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, 0);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, 1);
		Direction = Clock;
		float duty = (float)duty_cycle/100;
		htim4.Instance->CCR1 = duty*(htim4.Instance->ARR);
		return_flag = Valid;
	}
	else{
		return_flag = 0;
	}

	return return_flag;
}
