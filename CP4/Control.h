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
#include "math.h"
#include "Comandos.h"
#include "gpio.h"
#include "math.h"
/*___________________________*/

/* DEFINES */
#define Manual 0
#define Automatic 1
/*_________*/

/*_________Variable__________*/
extern _Bool Output;
extern _Bool Mode;
extern int index_output_wave;
extern float output_wave[51];
extern float Velocity_Buffer[256];
extern float Position_Buffer[256];
extern int index_count;
extern float duty_cycle;
/*___________________________*/

/*_________Functions_________*/
_Bool Sampling_Time(uint8_t *buffer);
_Bool Operation_Mode(uint8_t* buffer);
void clean_string_array(char string_array[6][6]);
int Parsing(uint8_t *message_to_parse, char string_array[6][6]);
int Units_Processement(char string_array[6][6], float *period);
_Bool Reference_Position(uint8_t *buffer1);
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim);
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef* htim2);
void Reset();
void direction();
void Velocity_Mesure();
void Position_Mesure();
_Bool Get_Constants(uint8_t *buffer1);

/*___________________________*/

#ifdef __cplusplus
}
#endif

#endif /* INC_CONTROL_H_ */
