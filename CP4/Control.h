/*
 * Control.h
 *
 *  Created on: Apr 18, 2022
 *      Author: Ines
 */

#ifndef INC_CONTROL_H_
#define INC_CONTROL_H_

#ifdef __cplusplus
extern "C"{
#endif

/*_________Includes__________*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tim.h"
#include "Comandos.h"
/*___________________________*/

/*_________Variable__________*/
extern _Bool Output;
/*___________________________*/

/*_________Functions_________*/
_Bool Sampling_Time(uint8_t *buffer);
_Bool Operation_Mode(uint8_t* buffer);
void clean_string_array(char string_array[6][6]);
int Parsing(uint8_t *message_to_parse, char string_array[6][6]);
int Units_Processement(char string_array[6][6], float *period);
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim);
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef* htim2);
/*___________________________*/

#ifdef __cplusplus
}
#endif

#endif /* INC_CONTROL_H_ */
