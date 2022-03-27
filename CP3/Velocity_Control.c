/*
 * Velocity_Control.c
 *
 *  Created on: Mar 25, 2022
 *      Author: Ines
 */

#include "Velocity_Control.h"
int velocity = 0;

_Bool Reference_Velocity(uint8_t *buffer1){
	_Bool return_flag, Validation;
	int row_number = 0;
	int velocity = 0;
	char string_array[6][6];

	clean_string_array(string_array);
	row_number = Parsing(buffer1, string_array);

	if(row_number != 1){
		Write_Tx_Buffer("Dados inseridos invalidos!! Escreva novamente...", 0);
		return invalid;
	}

	velocity = atoi(string_array[row_number], NULL, 10);
	Validation = Change_Velocity();
	if(Validation == 0){
		Write_Tx_Buffer("Valores fora dos limites!!", 0);
		return_flag = invalid;
	}

	return return_flag;
}

void Increment_Velocity(){
	velocity +=5;
	Change_Velocity();
}

void Decrement_Velocity(){
	velocity -=5;
	Change_Velocity();
}

_Bool Change_Velocity(){
	_Bool return_flag;

	if((velocity >= -160) && (velocity < 0)){
		Direction = Anti_Clock;
		return_flag = 1;
	}
	else if((velocity >= 0) && (velocity <= 160)){
		Direction = Clock;
		return_flag = 1;
	}
	else{
		return_flag = 0;
	}

	return return_flag;
}
