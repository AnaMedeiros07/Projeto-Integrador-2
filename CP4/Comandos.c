/*
 * Comandos.c
 *
 *  Created on: Apr 18, 2022
 *      Author: Ines
 */

/*_________Includes__________*/
#include "Comandos.h"
/*___________________________*/

/*_________Variable__________*/
_Bool prompt_flag;
_Bool valid;
_Bool Sample_K;
_Bool stop;
_Bool start;
_Bool Direction;

_Bool (*fun_ptr_arr[])(uint8_t*) = {Sampling_Time, Start, Operation_Mode};
_Bool (*fun_ptr_arr1[])() = {Invalid, Stop};

int K_value = 0;
/*___________________________*/

_Bool Check_Comand(uint8_t *buffer){

	/*____Function_Variables____*/
	_Bool return_variable;
	valid = 1;
	/*__________________________*/

	switch(buffer[0]){
		case 'S':
		case 's':
			switch(buffer[1]){
				case 'T':
				case 't':
					return_variable = (buffer[2] == ' ')?  (*fun_ptr_arr[0])(buffer) :  (*fun_ptr_arr1[0])();
					break;
				case 'P':
				case 'p':
					return_variable = (buffer[2] == '\0')?  (*fun_ptr_arr1[1])() :  (*fun_ptr_arr1[0])();
					break;
				case ' ':
				case '\0':
					return_variable = (*fun_ptr_arr[1])(buffer);
					break;
				default: return_variable = (*fun_ptr_arr1[0])();
					break;
			}
			break;
		case 'M':
		case 'm':
			switch(buffer[1]){
				case 'D':
				case 'd':
					return_variable = (buffer[2] == ' ')?  (*fun_ptr_arr[2])(buffer) :  (*fun_ptr_arr1[0])();
					break;
				default: return_variable = (*fun_ptr_arr1[0])();
					break;
			}
			break;
		default: return_variable = (*fun_ptr_arr1[0])();
			break;
	}
	return return_variable;
}

void Print(){
	if(transmite_flag == 1){
		Tx_Transmition();
		while(transmite_flag == 1){}
	}
}

_Bool Invalid(){
	Write_Tx_Buffer("Comando Invalido", 0);
	valid = 0;
	return 1;
}

_Bool Stop(){

	HAL_TIM_IC_Stop_IT(&htim2, TIM_CHANNEL_1);
//	HAL_TIM_Base_Stop(&htim3);
//	HAL_TIM_Base_Stop_IT(&htim3);
	HAL_TIM_Base_Stop_IT(&htim6);


	stop = 1;

	return 1;
}

_Bool Start(uint8_t* buffer){

	/*____Function_Variables____*/
	char string_array[6][6];
	int row_number = 0;
	/*__________________________*/

	start = 1;
	clean_string_array(string_array);
	row_number = Parsing(buffer, string_array);
	//Reset();
	switch(row_number){
		case 0:
			Sample_K = 0;
			prompt_flag = 0;
			HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_1);
			HAL_TIM_Base_Start(&htim2);
			HAL_TIM_Base_Start_IT(&htim6);

			//HAL_TIM_Base_Start_IT(&htim3);
			break;
		case 1:
			K_value = atoi(string_array[1]);
			Sample_K = 1;
			prompt_flag = 0;
			HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_1);
			HAL_TIM_Base_Start(&htim2);
			HAL_TIM_Base_Start_IT(&htim6);

			//HAL_TIM_Base_Start_IT(&htim3);
			break;
		default:
			Write_Tx_Buffer("Dados inseridos invalidos!! Escreva novamente...", 0);
			break;

	}
	return 1;
}
