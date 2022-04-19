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
_Bool Output;
int pulses = 0;
int KP=0;
int KD=0;
int KI=0;
float a=0;
float U_sat_a=0;
float U_sat_b=0;
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
float Velocity_Buffer[256];
int index_velocity=0;
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
	long int time_6_clock=0,autoreload=0;
	float period=0;
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

	 Kp_h=(float)(KP/period);
	 Ki_h=(float)(KI/period);
	 Kd_h=(float)(KD/period);

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
	for(index = 0; index <=255;index++)
	{
		Velocity_Buffer[index]=0.0;
	}
	index_velocity=0;
}

_Bool Get_Constants(uint8_t *buffer1){
	_Bool return_flag, Validation;
	int row_number = 0;
	char string_array[6][6];

	clean_string_array(string_array);
	row_number = Parsing(buffer1, string_array);

	if(row_number != 1){
		Write_Tx_Buffer("Dados inseridos invalidos!! Escreva novamente...", 0);
		return invalid;
	}
	switch(buffer1[1]){
			case 'D':
			case 'd':
				KD = atoi(string_array[row_number]);
			break;
			case 'P':
			case 'p':
				KP = atoi(string_array[row_number]);
			break;
			case 'I':
			case 'i':
				KI = atoi(string_array[row_number]);
			break;
			default: break;
	}
	switch(buffer1[1]){
			case 'a':
			case 'A':
				a = atof(string_array[row_number]);
			break;
			default: break;
	}
	switch(buffer1[2]){
			case 'a':
			case 'A':
				U_sat_a = atof(string_array[row_number]);
			break;
			case 'b':
			case 'B':
				U_sat_b = atof(string_array[row_number]);
			break;
			default: break;
	}
	return return_flag;
}

/*void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim){ // ISR_S
	if(htim == &htim3){
		int error = 0, sum_e = 0, e_ant = 0;
		float u_d = 0.0;
        HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_7);
		if((Sample_K == 1) && (K_value != 0))
		    {
		        K_value--;
		        error = reference - pulses;
				if(aut){
					sum_e_bkp = sum_e;
					sum_e += e_ant;
					u_d = kd_h*(pulses-pulses_ant)+ a*u_d_ant;
					u = Kp_h*error + Ki_h*sum_e - u_d;
					e_ant = e;
					pulses_ant = pulses;
					u_d_ant = u_d;
					if(u > U_sat_a){
						u = U_sat_a;
						sum_e = sum_e_bkp;
					}
					else if(u < U_sat_b){
						u = U_sat_b;
						sum_e = sum_e_bkp;
				//madar para fora;
					}
					else{
						e_ant = error;
						pulses_ant = pulses;
					}
				Output=1;


		    }
		    else if((Sample_K == 1) && (K_value == 0))
		        Stop();
		    else
		    {
		    	error = reference - pulses;
		    	if(aut){
		    		sum_e_bkp = sum_e;
		    		sum_e += e_ant;
		    		u_d = kd_h*(pulses-pulses_ant)+ a*u_d_ant;
		    		u = Kp_h*error + Ki_h*sum_e - u_d;
		    		e_ant = e;
		    		pulses_ant = pulses;
		    		u_d_ant = u_d;
		    		if(u > U_sat_a){
		    			u = U_sat_a;
		    			sum_e = sum_e_bkp;
		    		}
		    		else if(u < U_sat_b){
		    			u = U_sat_b;
		    			sum_e = sum_e_bkp;
		    	//madar para fora;
		    		}
		    		else{
		    			e_ant = error;
		    			pulses_ant = pulses;
		    		}
		    	}


			}
		Output=1;
	}
}*/

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc1)
{
		Output=1;
		y = HAL_ADC_GetValue(hadc1);
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
			if(HAL_DAC_Start(&hdac, DAC1_CHANNEL_1)== HAL_OK)
			  {
				 HAL_DAC_SetValue(&hdac, DAC1_CHANNEL_1,DAC_ALIGN_12B_R,(u*4095)/3.3);
			  }
		else
		{
			y_ant=y;
			e_ant=e;
		}
	}
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef* htim2){  // ISR_H
	if(htim2->Channel == HAL_TIM_ACTIVE_CHANNEL_1){
		pulses++;
	}
}
