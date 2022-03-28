/*
 * Velocity_Mesure.c
 *
 *  Created on: Mar 25, 2022
 *      Author: Ines
 */

#include "Velocity_Mesure.h"
#define RPS 1
#define RPM 1
#define Hz 1
#define rad 1
#define Pulses 1
#define Timer 2
float constant=0;
_Bool start=0;
_Bool stop=0;
_Bool Sample_K=0;
int K_value=0;
static int count_pulses=0;
uint32_t previous_time = 0;
uint32_t next_time = 0;
uint32_t result = 0;
float freq = 0;
_Bool first = 0;
int Freq_Calcul = 0;



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

_Bool Sampling_Period(uint8_t *buffer1)
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

_Bool Start(uint8_t *buffer1)
{
	char string_array[6][6];
	int row_number = 0;
	_Bool return_value = 0;

	start = 1;
	return_value = 0;
	clean_string_array(string_array);
	row_number = Parsing(buffer1, string_array);

	switch(row_number){
		case 0:
			Sample_K = 0;
			prompt_flag = 0;
			//HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_1);
			//HAL_TIM_Base_Start(&htim2);
			HAL_TIM_Base_Start_IT(&htim3);
			break;
		case 1:
			K_value = atoi(string_array[1]);
			Sample_K = 1;
			prompt_flag = 0;
			HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_1);
			HAL_TIM_Base_Start(&htim2);
			HAL_TIM_Base_Start_IT(&htim3);
			break;
		default:
			Write_Tx_Buffer("Dados inseridos invalidos!! Escreva novamente...", 0);
			return_value = invalid;
			break;

	}
	return return_value;
}

_Bool Stop(){

		HAL_TIM_Base_Stop_IT(&htim2);
		HAL_TIM_IC_Stop_IT(&htim2, TIM_CHANNEL_1);
		HAL_TIM_Base_Stop(&htim3);
		HAL_TIM_Base_Stop_IT(&htim3);
		count_pulses = 0;
		previous_time = 0;
		next_time = 0;
		stop = 1;

     return Valid;
}

_Bool Units_Processement_Velocity(char string_array[6][6])
{
    _Bool return_variable;
    return_variable = 0;
    switch(string_array[1][0])
    {
    case 'r':
    case 'R':
                switch(string_array[1][1])
                {
                case 'p':
                case 'P':
                    switch(string_array[1][2])
                    {
                    case 'm':
                    case 'M':
                        if (string_array[1][3] == '\0')
                        {
                            constant =RPM;
                        }
                        else
                        {
                         Write_Tx_Buffer("Dados inseridos invalidos!! Escreva novamente...", 0);
                          return_variable = invalid;
                        }
                    break;
                    case 's':
                    case 'S':
                        if (string_array[1][3] == '\0')
                        {
                            constant =RPS;
                        }
                        else
                        {
                         Write_Tx_Buffer("Dados inseridos invalidos!! Escreva novamente...", 0);
                          return_variable = invalid;
                        }
                    }
                case 'a':
                case 'A':
                    switch(string_array[1][2])
                    {
                    case 'd':
                    case 'D':
                        if (string_array[1][3] == '\0')
                        {
                            constant =rad;
                        }
                        else
                        {
                         Write_Tx_Buffer("Dados inseridos invalidos!! Escreva novamente...", 0);
                          return_variable = invalid;
                        }
                    }
                }
     break;
    case 'h':
    case 'H':
        case 'z':
        case 'Z':
            if (string_array[1][2] == '\0')
            {
                constant =Hz;
            }
            else
            {
             Write_Tx_Buffer("Dados inseridos invalidos!! Escreva novamente...", 0);
              return_variable = invalid;
            }
        break;
    default:
            Write_Tx_Buffer("Dados inseridos invalidos!! Escreva novamente...", 0);
            return_variable = invalid;
            break;
    }

    return return_variable;
}

_Bool Velocity_Units(uint8_t *buffer1)
{
	char string_array[6][6];
	int row_number = 0;
	_Bool return_value = 0;

	clean_string_array(string_array);
	row_number = Parsing(buffer1, string_array);
	if(Units_Processement_Velocity(string_array))
	        return 1;
    if(row_number !=2){
        Write_Tx_Buffer("Dados inseridos invalidos!! Escreva novamente...", 0);
        return 1;
    }
    return 1;
}

void direction() //PD1 = sensor A, PD0 = sensor B
{
	if(HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_0)==0)
	{
		Direction=Clock;
	}
	else
		Direction=Anti_Clock;
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef* htim2){
	if(htim2->Channel == HAL_TIM_ACTIVE_CHANNEL_1){
		if(first ==0)
		{
			previous_time=HAL_TIM_ReadCapturedValue(htim2,TIM_CHANNEL_1);
			first=1;
		}
		else
		{
			next_time=HAL_TIM_ReadCapturedValue(htim2,TIM_CHANNEL_1);
			if(next_time>previous_time)
			{
				result=next_time-previous_time;
			}
			else if(next_time<previous_time)
			{
				result=(0xffffffff-previous_time)+next_time;
			}
			__HAL_TIM_SET_COUNTER(htim2,0);
			first=0;
		}

		direction();
		++count_pulses;
	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim){
	if(htim == &htim3){
		int limit_pulses = 2;
		HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_7);
		if(count_pulses > limit_pulses){
			Freq_Calcul = Pulses;
			//Calculo de pulsos
			count_pulses = 0;
		}
		else{
			Freq_Calcul = Timer;
			float refClock=108000000.0/(TIM2->PSC);
			freq= refClock/result;
		}

	}
}