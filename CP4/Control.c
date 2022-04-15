/*
 * Control.c
 *
 *  Created on: Apr 15, 2022
 *      Author: Ines
 */

/* INCLUDES */
#include "Control.h"
/*__________*/

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

_Bool Sampling_Time(uint8_t *buffer)
{
    int prescaler=0, row_number = 0;
    long int time_3_clock=0,autoreload=0;
    char string_array[6][6];

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

_Bool Operation_Mode(uint8_t *buffer) {

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

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef* htim2){  // ISR_H
	if(htim2->Channel == HAL_TIM_ACTIVE_CHANNEL_1){
	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim){ // ISR_S
	if(htim == &htim3){
		Output=1;
	}
}
