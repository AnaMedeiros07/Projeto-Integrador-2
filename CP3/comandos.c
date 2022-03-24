/*
 * comandos.c
 *
 *  Created on: 14/03/2022
 *      Author: Ines
 */
#include "comandos.h"
#include "PWM.h"
#include "tim.h"
#include "usart.h"
#include "string.h"
#include "math.h"
#include "stdlib.h"
#include "stdbool.h"
#include "stdio.h"

#define PWM_mode 0
#define Control_mode 1
#define invalid 1
#define Valid 1


uint8_t b =0x20;

/*_Bool Output=0;
_Bool stop = 0;
_Bool start = 0;
_Bool Sample_K = 0;*/

_Bool operation_mode = 0;

int adc_validation = 0;
int valid=1,index_count=0, K_value = 0, prompt_flag;
_Bool (*fun_ptr_arr[])(uint8_t*) = {Operation_Mode, Enable};
_Bool (*fun_ptr_arr1[])() = {Invalid};

_Bool Check_Comand(uint8_t *buffer)
 {
	_Bool return_flag = 0;
	switch(buffer[0]){
		case 'C':
		case 'c':
			switch(buffer[1]){
				case 'S':
				case 's':   return_flag = (buffer[2] == b)? (*fun_ptr_arr[0])(buffer): (*fun_ptr_arr1[0])();
							break;
				default:	return_flag = (*fun_ptr_arr1[0])(); break;
			}
			break;
		case 'E':
		case 'e':
			switch(buffer[1]){
				case 'N':
				case 'n':	return_flag = ( buffer[2] == b)? (*fun_ptr_arr[1])(buffer): (*fun_ptr_arr1[0])();
							break;
				default:	return_flag = (*fun_ptr_arr1[0])(); break;
			}
			break;
		case 'U':
		case 'u':
			if(operation_mode == PWM_mode){
				switch(buffer[1]){
					case 'n':
					case 'N':
						return_flag = (buffer[2] == b)? Normalized_Tension(buffer): (*fun_ptr_arr1[0])();
						break;
					default: return_flag = (*fun_ptr_arr1[0])(); break;
				}
			}
			else{
				Write_Tx_Buffer("Modo de Controlo de Velocidade ativo!!", 0);
				return_flag = invalid;
			}
			break;
		case 'V':
		case 'v':
			if(operation_mode == Control_mode){
				switch(buffer[1]){
					case 'r':
					case 'R':
						return_flag = (buffer[2] == b)? (*fun_ptr_arr[1])(buffer): (*fun_ptr_arr1[0])();
						break;
					default: return_flag = (*fun_ptr_arr1[0])(); break;
				}
			}
			else{
				Write_Tx_Buffer("Modo de PWM ativo!!", 0);
				return_flag = invalid;
			}
			break;
		case '\0': Write_Tx_Buffer("Insira um comando!!!",0);
				   return_flag = invalid;
				   break;
		default: return_flag = (*fun_ptr_arr1[0])(); break;
	}
	return return_flag;

 }

_Bool Invalid()
{
	Write_Tx_Buffer("Comando Invalido",0);
	valid = 0;
	return invalid;
}

void Print(){
	 if(transmite_flag == 1){
		Tx_Transmition();
		while(transmite_flag == 1){}
	 }
}

int Parsing(uint8_t *message_to_parse, char string_array[6][6]){

	int counter=0, aux_counter=0, counter_space=0;

	while(message_to_parse[counter]!='\0')
	{
		if(message_to_parse[counter]==b)
		{
			if((message_to_parse[counter+1] != '\0') && (message_to_parse[counter+1] != b)){
				++counter_space;
				aux_counter=0;
			}
		}
		else if(counter_space != 0)
			string_array[counter_space][aux_counter++]=message_to_parse[counter];
		counter++;
	}

	return counter_space;
}

void clean_string_array(char string_array[6][6]){
	int row, columns;

	for(row = 0; row < 6; row++){
		for(columns = 0; columns < 6; columns++)
			string_array[row][columns] = '\0';
	}
}

_Bool Operation_Mode(uint8_t *buffer1)
{
	_Bool return_flag;
	int row_number = 0;
	char string_array[6][6];

	clean_string_array(string_array);
	row_number = Parsing(buffer1, string_array);

	if(row_number != 1){
		Write_Tx_Buffer("Dados inseridos invalidos!! Escreva novamente...", 0);
		return invalid;
	}

	switch(string_array[row_number][0]){
		case '0':
			if(string_array[row_number][1] == '\0'){
				operation_mode = PWM_mode;
				return_flag = Valid;
			}
			else
				return_flag = (*fun_ptr_arr1[1])();
			break;
		case '1':
			if(string_array[row_number][1] == '\0'){
				operation_mode = Control_mode;
				return_flag = Valid;
			}
			else
				return_flag = (*fun_ptr_arr1[1])();
			break;
		default:
			return_flag =  (*fun_ptr_arr1[1])();
			break;
	}

	row_number = 0;
	return return_flag;
}

_Bool Enable(uint8_t *buffer1){
	_Bool return_flag;
	int row_number = 0;
	char string_array[6][6];

	clean_string_array(string_array);
	row_number = Parsing(buffer1, string_array);

	if(row_number != 1){
		Write_Tx_Buffer("Dados inseridos invalidos!! Escreva novamente...", 0);
		return invalid;
	}

	switch(string_array[row_number][0]){
		case '0':
			if(string_array[row_number][1] == '\0'){
				(operation_mode == PWM_mode)? DeInit_PWM_Control(): 0;
				return_flag = Valid;
			}
			else
				return_flag = (*fun_ptr_arr1[1])();
			break;
		case '1':
			if(string_array[row_number][1] == '\0'){
				(operation_mode == PWM_mode)? Init_PWM_Control(): 0;
				return_flag = Valid;
			}
			else
				return_flag = (*fun_ptr_arr1[1])();
			break;
		default:
			return_flag =  (*fun_ptr_arr1[1])();
			break;
	}
}
