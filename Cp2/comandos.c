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
int valid=1,index_count=0, K_value = 0, prompt_flag;
_Bool Output=0;
int adc_validation = 0;
_Bool stop = 0;
uint8_t b =0x20;
_Bool Sample_K = 0;

int Check_Comand(uint8_t *buffer)
 {
	if ((buffer[0] == 'S' && buffer[1]=='P') || (buffer[0] == 's' && buffer[1]=='p'))
	{
		return Sampling_Period(buffer);
	}
	else if((buffer[0] == 'S' && buffer[1] == 'T' ) || (buffer[0] == 's' && buffer[1] == 't'))
	{
		return Stop();
	}
	else if((buffer[0] == 'A' && buffer[1] == 'C' ) || (buffer[0] == 'a' && buffer[1] == 'c'))
	{
		return Analog_Channel(buffer);
	}
	else if(buffer[0] == 'S' || buffer[0] == 's')
	{
		return Start(buffer);
	}
	else if((buffer[0] == 'F' && buffer[1] == 'F' ) || (buffer[0] == 'f' && buffer[1] == 'f'))
	{
		filter_on=0;
		return 1;
	}
	else if((buffer[0] == 'F' && buffer[1] == 'O' ) || (buffer[0] == 'f' && buffer[1] == 'o'))
	{
		filter_on=1;
		return 1;
	}
	else if(buffer[0] == '\0')
	{

		Write_Tx_Buffer("Insira um comando",0);
		return 1;
	}

	return Invalid();
 }
int Invalid()
{
	Write_Tx_Buffer("Comando Invalido",0);
	transmite_flag=1;
	valid=0;
	Print();
	return 0;
}

void Print(){
	 if(transmite_flag == 1){
		Tx_Transmition();
		while(transmite_flag == 1){}
	 }
}
int Analog_Channel(uint8_t *buffer1)
{
	char channel[4] = {"0"};
	int counter=0, aux_counter=0, counter_space=0;
	long channel_num = 0;

	while(buffer1[counter]!='\0')
	{
		if(buffer1[counter]==b)
		{
			++counter_space;
			aux_counter=0;
		}
		else if(counter_space==1)
			channel[aux_counter++]=buffer1[counter];
		counter++;
	}

	if(counter_space != 1)
		return 0;

	ADC_Desconfig(channel_num);

	channel_num = strtol(channel, NULL, 16);

	if(config_ADC(channel_num) == 0)
		return 0;
	return 1;
}
int Sampling_Period(uint8_t *buffer1)
{
	char time_unit[4] = {"\0"}, units[4]={"\0"};
	int counter=0, aux_counter=0, counter_space=0;
	int prescaler=0;
	long int time_6_clock=0,autoreload=0;
	float period=0;

   //--------------------------------------------
	while(buffer1[counter]!='\0')
	{
		if(buffer1[counter]==b)
		{
			++counter_space;
			aux_counter=0;
		}
		else if(counter_space==1)
			time_unit[aux_counter++]=buffer1[counter];
		else if(counter_space==2)
			units[aux_counter++]=buffer1[counter];
		counter++;
	}
	if(counter_space!=2)
		return 0;
	//---------------------------------------------

	if(!strcmp(time_unit,"s"))
	{
		period=atof(units);
	}
	else if(!strcmp(time_unit,"ms"))
	{
		period=atof(units)*0.001;
	}
	else if(!strcmp(time_unit,"micro"))
	{
		period=atof(units)*0.000001;
	}

	time_6_clock = (65536/(period));
	prescaler = ((108000000/time_6_clock));
	autoreload=((108000000*period)/(prescaler+1))-1;

	if(autoreload > 65535)
		autoreload = 65535;

	Timer_Configuration(autoreload,prescaler);
	return 1;
}
int Start(uint8_t *buffer1)
{
	char Kvalues[4] = {"\0"};
	int counter=0, aux_counter=0, counter_space=0;

	while(buffer1[counter]!='\0')
	{
		if(buffer1[counter]==b)
		{
			++counter_space;
			aux_counter=0;
		}
		else if(counter_space==1)
			Kvalues[aux_counter++]=buffer1[counter];
		counter++;
	}

	if(counter_space == 0)
	{
		Sample_K = 0;
		HAL_TIM_Base_Start_IT(&htim6);
	}
	else
	{
		K_value = atoi(Kvalues);
		Sample_K = 1;
		HAL_TIM_Base_Start_IT(&htim6);
	}

	prompt_flag = 0;

	return 0;

}

int Stop(){

	 Reset();
	 HAL_TIM_Base_Stop_IT(&htim6);

	 stop = 1;
	 prompt_flag = 1;
}
void Print_Trama(double temp)
{
	index_count=(Save_N_Buffer());
	if(index_count==0)
		index_count=127;
	else
		index_count= index_count-1;
	Save_X_ant();
	Save_X_Buffer(temp);
	if(HAL_DAC_Start(&hdac, DAC1_CHANNEL_1)== HAL_OK)
			  {
				 HAL_DAC_SetValue(&hdac, DAC1_CHANNEL_1,DAC_ALIGN_12B_R,(Save_Y()*4095)/3.3);
			  }

	Save_Y_ant();
	Output=1;

}



