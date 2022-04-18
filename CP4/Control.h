/*
 * Control.h
 *
 *  Created on: Apr 15, 2022
 *      Author: Ines
 */

#ifndef INC_CONTROL_H_
#define INC_CONTROL_H_

#ifdef __cplusplus
extern "C" {
#endif

/* INCLUDES */
#include "tim.h"
#include "usart.h"
#include "main.h"
#include "gpio.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "math.h"
/*__________*/

/* DEFINES */
#define Manual 0
#define Automatic 1
/*_________*/

/* VARIÁVEIS DESTE MÓDULO */
#define invalid 1
_Bool Mode;
extern _Bool Sample_K;
extern int K_value;
extern float period;
extern _Bool stop;
extern _Bool start;
extern _Bool Output;
/*________________________*/

/* FUNÇÕES DESTE MÓDULO */
_Bool Sampling_Time(uint8_t*);
int Parsing(uint8_t *message_to_parse, char string_array[6][6]);
void clean_string_array(char string_array[6][6]);
_Bool Operation_Mode(uint8_t *buffer);
void Reset();
int Units_Processement(char string_array[6][6], float *period);
_Bool Get_Constants(uint8_t *buffer1);
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef*);
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef*);
/*______________________*/

#endif /* INC_CONTROL_H_ */
