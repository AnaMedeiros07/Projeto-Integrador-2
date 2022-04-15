/*
 * Comandos.c
 *
 *  Created on: Apr 15, 2022
 *      Author: Ines
 */

/* INCLUDES */
#include "Comandos.h"
/*__________*/

_Bool (*fun_ptr_arr[])(uint8_t*) = {Sampling_Time, Start, Operation_Mode};
_Bool (*fun_ptr_arr1[])() = {Invalid, Stop};

_Bool Check_Comand(uint8_t *buffer){

	/* VARIÁVEIS DA FUNÇÃO */
	_Bool return_variable;
	/*_____________________*/

	switch(buffer[0]){
		case 'S':
		case 's':
			switch(buffer[1]){
				case 'T':
				case 't':
					return_variable = (buffer[2] == ' ')? fun_ptr_arr[0] : fun_ptr_arr1[0];
					break;
				case 'P':
				case 'p':
					return_variable = (buffer[2] == '\0')? fun_ptr_arr1[1] : fun_ptr_arr1[0];
					break;
				case ' ':
				case '\0':
					return_variable = fun_ptr_arr[1];
				default: return_variable = fun_ptr_arr1[0]; break;
			}
			break;
		case 'M':
		case 'm':
			switch(buffer[1]){
				case 'D':
				case 'd':
					return_variable = (buffer[2] == ' ')? fun_ptr_arr[2] : fun_ptr_arr1[0];
					break;
				default: return_variable = fun_ptr_arr1[0]; break;
			}
		default: return_variable = fun_ptr_arr1[0]; break;
	}
}

_Bool Start(uint8_t *buffer)
{
	/* VARIAVEIS DA FUNÇÃO */
	char string_array[6][6];
	int row_number = 0;
	_Bool return_value = 0;
	/*_____________________*/

	start = 1;
	return_value = 0;
	clean_string_array(string_array);
	row_number = Parsing(buffer, string_array);
	Reset();

	switch(row_number){
		case 0:
			Sample_K = 0;
			prompt_flag = 0;
			HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_1);
			HAL_TIM_Base_Start_IT(&htim3);
			break;
		case 1:
			K_value = atoi(string_array[1]);
			Sample_K = 1;
			prompt_flag = 0;
			HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_1);
			HAL_TIM_Base_Start_IT(&htim3);
			break;
		default:
			Write_Tx_Buffer("Dados inseridos invalidos!! Escreva novamente...", 0);
			return_value = 1;
			break;

	}
	return return_value;
}

_Bool Stop(){

	HAL_TIM_IC_Stop_IT(&htim2, TIM_CHANNEL_1);
	HAL_TIM_Base_Stop_IT(&htim3);
	stop = 1;

     return 1;
}

_Bool Invalid(){
	Write_Tx_Buffer("Comando Invalido",0);
	//valid = 0;
	return 1;
}
