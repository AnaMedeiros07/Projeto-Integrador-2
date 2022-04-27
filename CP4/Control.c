/*
 * Control.c
 *
 *  Created on: Apr 18, 2022
 *      Author: Ines
 */

#include "Control.h"

/*_________Defines__________*/
#define b 0x20
/*__________________________*/

/*________Variables_________*/
#define U_sat_a  1
#define U_sat_b -1

_Bool Output;
_Bool Mode = 0;

float yr = 1.0;
int pulses = 0;
float KP=0;
float KD=0;
float KI=0;
float a=0;
float y=0.0;
float y_ant=0.0;
float e=0.0;
float e_ant=0.0;
float sum_e=0.0;
float sum_e_bkp=0.0;
float u=0.0;
float u_d=0.0;
float u_d_ant =0.0;
float Kd_h=0.0;
float Ki_h=0.0;
float Kp_h=0.0;
float period=0;
float ref_position=0;
float Velocity_Buffer[256] = {0.0};
float Position_Buffer[256]={0.0};

int index_output_wave = 0;
int index_count = 0;

/*__________________________*/

void clean_string_array(char string_array[6][6]){
	int row, columns;

	for(row = 0; row < 6; row++){
		for(columns = 0; columns < 6; columns++)
			string_array[row][columns] = '\0';
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

_Bool Sampling_Time(uint8_t *buffer){

	/*____Function_Variables____*/
	int prescaler=0, row_number = 0;
	long int time_3_clock=0,autoreload=0;
	char string_array[6][6];
	/*__________________________*/

   //--------------------------------------------
	clean_string_array(string_array);
	row_number = Parsing(buffer, string_array);

	if(row_number !=2){
		Write_Tx_Buffer("Dados inseridos invalidos!! Escreva novamente...", 0);
		return 1;
	}
	//---------------------------------------------

	if(Units_Processement(string_array, &period))
		return 1;


	if(period > 0){
		time_3_clock = (65536/(period));
		prescaler = ((108000000/time_3_clock));
		autoreload=((108000000*period)/(prescaler+1))-1;

		if(autoreload > 65535)
			autoreload = 65535;

		Timer_Configuration(autoreload,prescaler);
	}

	row_number = 0;
	return 1;
}

_Bool Operation_Mode(uint8_t* buffer){

	/* VARIÁVEIS DA FUNÇÃO */
	int  row_number = 0;
	char string_array[6][6];
	_Bool return_variable;
	/*_____________________*/

   //--------------------------------------------
	clean_string_array(string_array);
	row_number = Parsing(buffer, string_array);

	if(row_number !=1){
		Write_Tx_Buffer("Dados inseridos invalidos!! Escreva novamente...", 0);
		return 1;
	}
	//---------------------------------------------

	switch(string_array[row_number][0]){
		case '0':
			Mode = Manual;
			sum_e=0;
			u_d=0;
			u=0;
			sum_e_bkp=0;
			e_ant=0;
			y_ant=0;
			return_variable = 1;
			break;
		case '1':
			Mode = Automatic;
			return_variable = 1;
			break;
		default: return_variable = 1; break;
	}

	return return_variable;
}

void Reset()
{
	int index=0;
	sum_e=0;
	u_d=0;
	u=0;
	sum_e_bkp=0;
	e_ant=0;
	y_ant=0;
	index_count=0;

	for(index = 0; index <=255;index++)
	{
		Velocity_Buffer[index]=0.0;
	}
	for(index = 0; index <=255;index++)
	{
		Position_Buffer[index]=0.0;
	}
}

_Bool Get_Constants(uint8_t *buffer1){
	int row_number = 0;
	char string_array[6][6];

	clean_string_array(string_array);
	row_number = Parsing(buffer1, string_array);

	if(row_number != 1){
		Write_Tx_Buffer("Dados inseridos invalidos!! Escreva novamente...", 0);
		return 1;
	}
	switch(buffer1[1]){
			case 'D':
			case 'd':
				KD = atof(string_array[row_number]);
				if(KD>200 || KD<0)
				{
					KD=0;
					Write_Tx_Buffer("Dados inseridos invalidos!! KD >200 ou KD <0, escreva novamente ", 0);
				}
			break;
			case 'P':
			case 'p':
				KP = atof(string_array[row_number]);
				if(KP>200 || KP<0)
				{
					KP=0;
					Write_Tx_Buffer("Dados inseridos invalidos!! KP >200 ou KP <0, escreva novamente ", 0);
				}
			break;
			case 'I':
			case 'i':
				KI = atof(string_array[row_number]);
				if(KI>200 || KI<0)
				{
					KI=0;
					Write_Tx_Buffer("Dados inseridos invalidos!! KI >200 ou KI <0, escreva novamente ", 0);
				}
			break;
			default: break;
	}
	switch(buffer1[0]){
			case 'a':
			case 'A':
				a = atof(string_array[row_number]);
			break;
			default: break;
	}
	return 1;
}

_Bool Reference_Position(uint8_t *buffer1){
	int row_number = 0;
	char string_array[6][6];

	clean_string_array(string_array);
	row_number = Parsing(buffer1, string_array);

	if(row_number != 1){
		Write_Tx_Buffer("Dados inseridos invalidos!! Escreva novamente...", 0);
		return 1;
	}

	ref_position  = atoi(string_array[row_number]);
	if(ref_position >= 720 || ref_position <= 0){
		Write_Tx_Buffer("Valores fora dos limites!! Posicao > 720 ou <0", 0);
	}

	return 1;
}

void Control()
{

	 Kp_h=KP;
	 Ki_h=(float)(KI*period);
	 Kd_h=(float)((KD*(1-a))/period);
	 e=yr-y;
			if (Mode == Automatic)
			{
				sum_e_bkp=sum_e;
				sum_e=sum_e+e_ant;
				u_d=Kd_h*(y-y_ant)+a*u_d_ant;
				u=Kp_h*e+Ki_h*sum_e-u_d;
				e_ant=e;
				y_ant=y;
				u_d_ant=u_d;
			if(u>U_sat_a)
			{
				u=U_sat_a;
				sum_e=sum_e_bkp;
			}
			else if(u<U_sat_b)
			{
				u=U_sat_b;
				sum_e=sum_e_bkp;
			}
				output_wave[index_output_wave]=u;
				++index_output_wave;
			}
			else
			{
				y_ant=y;
				e_ant=e;
				if(index_output_wave==51)
					index_output_wave=0;
			}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim){ // ISR_S
	if(htim == &htim3){
		float position=0;
		float Velocity=0;
		if((Sample_K == 1) && (K_value != 0))
		{
			K_value--;
			position=(pulses*960)/2*M_PI;
			Velocity= (float)(2*M_PI/960)*pulses*(float)(1/period);

			Position_Buffer[index_count]=position*(float)Direction;
			Velocity_Buffer[index_count]=Velocity*(float)Direction;

			++index_count;
			index_count &= ~(1<<7);
			pulses = 0;
			Output = 1;

		}
		else if((Sample_K == 1) && (K_value == 0))
			Stop();
		else
		{
			position=(pulses*960)/2*M_PI;
			Velocity= (float)(2*M_PI/960)*pulses*(float)(1/period);

			Position_Buffer[index_count]=position*(float)Direction;
			Velocity_Buffer[index_count]=Velocity*(float)Direction;
			++index_count;
			index_count&= ~(1<<7);
			pulses = 0;
			Output = 1;
		}

	}
}

void direction() //PA6 = sensor B
{
	if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_6)==0)
	{
		Direction=Clock;
	}
	else
		Direction=Anti_Clock;
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef* htim2){  // ISR_H
	if(htim2->Channel == HAL_TIM_ACTIVE_CHANNEL_3){
		HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_7);
		pulses++;
		direction();

	}
}
