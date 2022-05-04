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

_Bool (*fun_ptr_arr[])(uint8_t*) = {Sampling_Time, Start, Operation_Mode,Get_Constants,Reference_Position,Enable};
_Bool (*fun_ptr_arr1[])() = {Invalid, Stop};

int K_value = 0;
/*___________________________*/
_Bool Check_Comand(uint8_t *buffer){

	/* VARIÁVEIS DA FUNÇÃO */
	_Bool return_variable;
	/*_____________________*/

	switch(buffer[0]){
		case 'S':
		case 's':
			switch(buffer[1]){
				case 'P':
				case 'p':
					return_variable = (buffer[2] == '\0')? (*fun_ptr_arr1[1])() : (*fun_ptr_arr1[0])();
					break;
				case ' ':
				case '\0':
					return_variable = (*fun_ptr_arr[1])(buffer);
					break;
				default: return_variable = (*fun_ptr_arr1[0])(); break;
			}
			break;
		case 'H':
		case 'h':
			switch(buffer[1]){
				case 'W':
				case 'w':
					return_variable = (buffer[2] == ' ')? (*fun_ptr_arr[0])(buffer) : (*fun_ptr_arr1[0])();
					break;
				default: return_variable = (*fun_ptr_arr1[0])(); break;
			}
			break;
		case 'P':
		case 'p':
			switch(buffer[1]){
				case 'R':
				case 'r':
					return_variable = (buffer[2] == ' ')? (*fun_ptr_arr[4])(buffer) : (*fun_ptr_arr1[0])();
					break;
				default: return_variable = (*fun_ptr_arr1[0])(); break;
			}
			break;
		case 'E':
		case 'e':
			switch(buffer[1]){
				case 'N':
				case 'n':	return_variable = ( buffer[2] == ' ')? (*fun_ptr_arr[5])(buffer): (*fun_ptr_arr1[0])();
							break;
				default:	return_variable = (*fun_ptr_arr1[0])(); break;
			}
			break;
		break;
		case 'M':
		case 'm':
			switch(buffer[1]){
				case 'D':
				case 'd':
					return_variable = (buffer[2] == ' ')? (*fun_ptr_arr[2])(buffer) : (*fun_ptr_arr1[0])();
					break;
				default: return_variable = (*fun_ptr_arr1[0]); break;
			}
		break;
		case 'K':
		case 'k':
			switch(buffer[1]){
				case 'D':
				case 'd':
				case 'P':
				case 'p':
				case 'I':
				case 'i':
					return_variable = (buffer[2] == ' ')? (*fun_ptr_arr[3])(buffer) : (*fun_ptr_arr1[0])();
					break;
				default: return_variable = (*fun_ptr_arr1[0])(); break;
			}
			break;
		case 'a':
		case 'A':
			return_variable = (buffer[1] == ' ')? (*fun_ptr_arr[3])(buffer) : (*fun_ptr_arr1[0])();
		break;
		default: return_variable = (*fun_ptr_arr1[0])(); break;
	}
	return return_variable;
}


void Print(){
	if(transmite_flag == 1){
		Tx_Transmition();
		while(transmite_flag == 1){}
	}
}

_Bool Enable(uint8_t *buffer1){
	_Bool return_flag;
	int row_number = 0;
	char string_array[6][6];

	clean_string_array(string_array);
	row_number = Parsing(buffer1, string_array);

	if(row_number != 1){
		Write_Tx_Buffer("Dados inseridos invalidos!! Escreva novamente...", 0);
		return 1;
	}

	switch(string_array[row_number][0]){
		case '0':
			if(string_array[row_number][1] == '\0'){
				(Mode == Manual)? DeInit_PWM_Control(): 0;
				Reset();
				return_flag = 1;
			}
			else
				return_flag = (*fun_ptr_arr1[1])();
			break;
		case '1':
			if(string_array[row_number][1] == '\0'){
				(Mode == Automatic)? Init_PWM_Control(): 0;
				return_flag = 1;
			}
			else
				return_flag = (*fun_ptr_arr1[1])();
			break;
		default:
			return_flag =  (*fun_ptr_arr1[1])();
			break;
	}

	return return_flag;
}

_Bool Invalid(){
	Write_Tx_Buffer("Comando Invalido", 0);
	valid = 0;
	return 1;
}

_Bool Stop(){

	HAL_TIM_IC_Stop_IT(&htim2, TIM_CHANNEL_3);
	HAL_TIM_Base_Stop_IT(&htim3);
	duty_cycle = 0;
	DeInit_PWM_Control();
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
	Reset();
	switch(row_number){
		case 0:
			Sample_K = 0;
			prompt_flag = 0;
			HAL_TIM_Base_Start_IT(&htim3);
			HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_3);
			HAL_TIM_Base_Start(&htim2);
			break;
		case 1:
			K_value = atoi(string_array[1]);
			Sample_K = 1;
			prompt_flag = 0;
			HAL_TIM_Base_Start_IT(&htim3);
			HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_3);
			HAL_TIM_Base_Start(&htim2);
		default:
			Write_Tx_Buffer("Dados inseridos invalidos!! Escreva novamente...", 0);
			break;

	}
	return 1;
}
