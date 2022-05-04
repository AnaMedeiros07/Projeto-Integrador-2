/*
 * PWM.c
 *
 *  Created on: Apr 19, 2022
 *      Author: Ines
 */

/*_________Includes__________*/
#include "PWM.h"
/*___________________________*/

/*_________Variable__________*/

/*___________________________*/

void Init_PWM_Control (){
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, 1);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, 1);
}

void DeInit_PWM_Control (){
	duty_cycle = 0;
	Change_Duty();
	HAL_TIM_PWM_Stop(&htim4,TIM_CHANNEL_1);

	HAL_TIM_PWM_Stop(&htim4,TIM_CHANNEL_2);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, 0);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, 0);
}

_Bool Normalized_Tension(uint8_t *buffer1){
	_Bool return_flag, Validation;
	int row_number = 0;
	char string_array[6][6];

	clean_string_array(string_array);
	row_number = Parsing(buffer1, string_array);

	if(row_number != 1){
		Write_Tx_Buffer("Dados inseridos invalidos!! Escreva novamente...", 0);
		return 1;
	}

	duty_cycle = atoi(string_array[row_number]);
	Validation = Change_Duty();
	if(Validation == 0){
		Write_Tx_Buffer("Valores fora dos limites!!", 0);
		return_flag = 1;
	}

	return return_flag;
}

_Bool Change_Duty(){
	_Bool return_flag;

		if((duty_cycle >= -100) && (duty_cycle < 0)){
			Direction = Anti_Clock;
			float duty = (float)(duty_cycle*(-1))/100;
			htim4.Instance->CCR2 = duty*(htim4.Instance->ARR);
			htim4.Instance->CCR1 =0;
			HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_2);
			HAL_TIM_PWM_Stop(&htim4,TIM_CHANNEL_1);
			return_flag = 1;
		}
		else if((duty_cycle >= 0) && (duty_cycle <= 100)){
			Direction = Clock;
			float duty = (float)duty_cycle/100;
			htim4.Instance->CCR1 = duty*(htim4.Instance->ARR);
			htim4.Instance->CCR2 =0;
			HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_1);
			HAL_TIM_PWM_Stop(&htim4,TIM_CHANNEL_2);
			return_flag = 1;
		}
		else
			return_flag = 0;


		return return_flag;
}
