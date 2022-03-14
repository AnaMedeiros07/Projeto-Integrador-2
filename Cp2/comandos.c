/*
 * comandos.c
 *
 *  Created on: 13/03/2022
 *      Author: anaim
 */
#include "comandos.h"

#include "tim.h"
#include "adc.h"
#include "dac.h"
#include "usart.h"
#include "output.h"
#include "string.h"
#include "math.h"
#include "stdlib.h"
#include "stdbool.h"
#include "stdio.h"

uint8_t b =0x20;

_Bool Output=0;
_Bool stop = 0;
_Bool start = 0;
_Bool Sample_K = 0;

int adc_validation = 0;
int valid=1,index_count=0, K_value = 0, prompt_flag;
int (*fun_ptr_arr[])(uint8_t*) = {Sampling_Period, Analog_Channel, Start};
int (*fun_ptr_arr1[])() = {Stop, Invalid};

int Check_Comand(uint8_t *buffer)
 {
	int return_variable = 0;
	switch(buffer[0]){
		case 'S':
		case 's':
			switch(buffer[1]){
				case 'P':
				case 'p':   return_variable = (buffer[2] == ' ')? (*fun_ptr_arr[0])(buffer): (*fun_ptr_arr1[1])();
							break;
				case 'T':
				case 't':   return_variable = (buffer[2] == '\0' || (buffer[2] == ' '))? (*fun_ptr_arr1[0])(buffer): (*fun_ptr_arr1[1])();
							break;
				case '\0':
				case ' ':   return_variable = (*fun_ptr_arr[2])(buffer);
							break;
				default:	return_variable = (*fun_ptr_arr1[1])(); break;
			}
			break;
		case 'A':
		case 'a':
			switch(buffer[1]){
				case 'C':
				case 'c':	return_variable = ( buffer[2] == ' ')? (*fun_ptr_arr[1])(buffer): (*fun_ptr_arr1[1])();
							break;
				default:	return_variable = (*fun_ptr_arr1[1])(); break;
			}
			break;
		case 'F':
		case 'f':
			switch(buffer[1]){
				case 'F':
				case 'f':
					if((buffer[2] == '\0') || (buffer[2] == ' ')){
						filter_on = 0;
						return_variable = 1;
					}
					else
						return_variable = (*fun_ptr_arr1[1])();
					break;
				case 'O':
				case 'o':
					if((buffer[2] == '\0') || (buffer[2] == ' ')){
						filter_on = 1;
						return_variable = 1;
					}
					else
						return_variable = (*fun_ptr_arr1[1])();
					break;
				default: return_variable = (*fun_ptr_arr1[1])(); break;
			}
			break;
		case '\0': Write_Tx_Buffer("Insira um comando!!!",0);
				   return_variable = 1;
				   break;
		default: return_variable = (*fun_ptr_arr1[1])(); break;
	}
	return return_variable;

 }

int Invalid()
{
	Write_Tx_Buffer("Comando Invalido",0);
	return 1;
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

int Analog_Channel(uint8_t *buffer1)
{
	long channel_num = 0;
	int row_number = 0;
	char string_array[6][6];

	clean_string_array(string_array);
	row_number = Parsing(buffer1, string_array);

	if(row_number != 1){
		Write_Tx_Buffer("Dados inseridos invalidos!! Escreva novamente...", 0);
		return 1;
	}

	ADC_Desconfig(channel_num);

	channel_num = strtol(string_array[row_number], NULL, 16);

	if(config_ADC(channel_num) == 0)
		return 0;

	row_number = 0;
	return 1;
}

int Units_Processement(char string_array[6][6], float *period){
	int return_variable;
	return_variable = 0;
	switch(string_array[1][0]){
			case 's':
			case 'S':
				if (string_array[1][1] == '\0')
					*period=atof(string_array[2]);
				else{
					Write_Tx_Buffer("Dados inseridos invalidos!! Escreva novamente...", 0);
					return_variable = 1;
				}
				break;
			case 'm':
			case 'M':
				switch(string_array[1][1]){
					case 's':
					case 'S':
						if (string_array[1][2] == '\0')
							*period=atof(string_array[2])*0.001;
						else{
							Write_Tx_Buffer("Dados inseridos invalidos!! Escreva novamente...", 0);
							return_variable = 1;
						}
						break;
					case 'i':
					case 'I':
						switch(string_array[1][2]){
							case 'c':
							case 'C':
								switch(string_array[1][3]){
									case 'r':
									case 'R':
										switch(string_array[1][4]){
											case 'o':
											case 'O':
												if (string_array[1][5] == '\0')
													*period=atof(string_array[2])*0.000001;
												else{
													Write_Tx_Buffer("Dados inseridos invalidos!! Escreva novamente...", 0);
													return_variable = 1;
												}
												break;
											default:
												Write_Tx_Buffer("Dados inseridos invalidos!! Escreva novamente...", 0);
												return_variable = 1;
												break;
										}
										break;
									default:
										Write_Tx_Buffer("Dados inseridos invalidos!! Escreva novamente...", 0);
										return_variable = 1;
										break;
								}
								break;
							default:
								Write_Tx_Buffer("Dados inseridos invalidos!! Escreva novamente...", 0);
								return_variable = 1;
								break;
						}
				}
		}
	return return_variable;
}

int Sampling_Period(uint8_t *buffer1)
{
	int prescaler=0, row_number = 0;
	long int time_6_clock=0,autoreload=0;
	float period=0;
	char string_array[6][6];

   //--------------------------------------------
	clean_string_array(string_array);
	row_number = Parsing(buffer1, string_array);

	if(row_number !=2){
		Write_Tx_Buffer("Dados inseridos invalidos!! Escreva novamente...", 0);
		return 1;
	}
	//---------------------------------------------
	if(Units_Processement(string_array, &period))
		return 1;

	if(period > 0){
		time_6_clock = (65536/(period));
		prescaler = ((108000000/time_6_clock));
		autoreload=((108000000*period)/(prescaler+1))-1;

		if(autoreload > 65535)
			autoreload = 65535;

		Timer_Configuration(autoreload,prescaler);
	}

	row_number = 0;
	return 1;
}

int Start(uint8_t *buffer1)
{
	char string_array[6][6];
	int row_number = 0, return_value = 0;

	start = 1;
	return_value = 0;
	clean_string_array(string_array);
	row_number = Parsing(buffer1, string_array);

	switch(row_number){
		case 0:
			Sample_K = 0;
			prompt_flag = 0;
			HAL_TIM_Base_Start_IT(&htim6);
			break;
		case 1:
			K_value = atoi(string_array[1]);
			Sample_K = 1;
			prompt_flag = 0;
			HAL_TIM_Base_Start_IT(&htim6);
			break;
		default:
			Write_Tx_Buffer("Dados inseridos invalidos!! Escreva novamente...", 0);
			return_value = 1;
			break;

	}
	return return_value;
}

int Stop(){

	 Reset();
	 HAL_TIM_Base_Stop_IT(&htim6);

	 stop = 1;

	 return 1;
}

void Print_Trama(double temp)
{
	index_count=(Save_N_Buffer());
	if(index_count==0)
		index_count=127;
	else
		index_count -= 1;

	switch(filter_on){
		case 0:
			Save_X_Buffer(temp);
			Save_Y();
			break;
		case 1:
			Save_X_ant();
			Save_X_Buffer(temp);
			Save_Y();
			Save_Y_ant();
			break;
		default: break;
	}

/*	if(HAL_DAC_Start(&hdac, DAC1_CHANNEL_1)== HAL_OK)
  {
	 HAL_DAC_SetValue(&hdac, DAC1_CHANNEL_1,DAC_ALIGN_12B_R,(Save_Y()*4095)/3.3);
  }*/

	Output=1;
}



