/*
 * PWM.c
 *
 *  Created on: 24/03/2022
 *      Author: Ines
 */

#include "PWM.h"


void Init_PWM_Control (){
	HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_2);
}

void DeInit_PWM_Control (){
	HAL_TIM_PWM_Stop(&htim4,TIM_CHANNEL_2);
}

_Bool Normalized_Tension(uint8_t *buffer1){
	_Bool return_flag;
	int row_number = 0;
	int duty_cycle = 0;
	char string_array[6][6];

	clean_string_array(string_array);
	row_number = Parsing(buffer1, string_array);

	if(row_number != 1){
		Write_Tx_Buffer("Dados inseridos invalidos!! Escreva novamente...", 0);
		return invalid;
	}

	duty_cycle = atoi(string_array[row_number], NULL, 10);
	if((duty_cycle >= -100) && (duty_cycle < 0)){
		Direction = Anti_Clock;
		duty_cycle*=-1;
		float duty = (float)duty_cycle/100;
		htim4.Instance->CCR2 = duty*(htim4.Instance->ARR);
		return_flag = Valid;
	}
	else if((duty_cycle >= 0) && (duty_cycle <= 100)){
		Direction = Clock;
		float duty = (float)duty_cycle/100;
		htim4.Instance->CCR2 = duty*(htim4.Instance->ARR);
		return_flag = Valid;
	}
	else
		return_flag = invalid;

	return return_flag;

}
