/*
 * Control.c
 *
 *  Created on: Apr 18, 2022
 *      Author: Ines
 */

#include "Control.h"

/*_________Defines__________*/
#define b 0x20
#define angulo_radianos (2*M_PI)/960.0
#define angulo_graus (2*M_PI)/360.0
/*__________________________*/

/*________Variables_________*/
#define U_sat_a  6
#define U_sat_b -6
#define n 15
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
float old_speed_medio[n] = {0.0};
float speed_medio = 0.0;
int index_output_wave = 0;
int index_count = 0;
float duty_cycle=0;
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
	ref_position=ref_position*angulo_graus;
	ref_position+=y_ant;
	Direction = (ref_position < 0)? Anti_Clock: Clock;

	if(ref_position >= 12.57 || ref_position <= -12.57){
		Write_Tx_Buffer("Valores fora dos limites!! Posicao > 720 ou <-720", 0);
	}


	return 1;
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim){ // ISR_S
	if(htim == &htim3){
		static float position = 0.0;
		float Velocity = 0.0;

			position =(pulses* angulo_radianos*Direction);
			Velocity = Velocity_Calculation(pulses);
			Kp_h=KP;
			Ki_h=(float)(KI*period);
			Kd_h=(float)((KD*(1-a))/period);

			e=ref_position - (position+y_ant);
			if (Mode == Automatic)
			{
				position +=y_ant;
				sum_e_bkp=sum_e;
				sum_e=sum_e+e_ant;
				u_d=Kd_h*(position-y_ant)+a*u_d_ant;
				u=Kp_h*e+Ki_h*sum_e-u_d;
				e_ant=e;
				y_ant=position;
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
			}
			else
			{
				y_ant=position;
				e_ant=e;
			}

			duty_cycle=(u*100.0)/U_sat_a;
			Change_Duty();
			Position_Buffer[index_count]=position;
			Velocity_Buffer[index_count]=Velocity*(float)Direction;
			++index_count;
			index_count&= ~(1<<7);
			pulses = 0;
			Output = 1;
	}
}

float Velocity_Calculation(int pulses){
	float Velocity = 0.0;
	static float speed_avg=0;
	static float sum_speed=0.0;
	static int index = 0;

	Velocity= (pulses*angulo_radianos)*(1/period);
	sum_speed=sum_speed +Velocity- old_speed_medio[index];
	old_speed_medio[index]=Velocity;
	speed_avg=(float)sum_speed/n;
	index++;

	if(index == n)
		index = 0;

	return speed_avg;
}

void direction() //PA6 = sensor A
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
		pulses++;
		direction();

	}
}
