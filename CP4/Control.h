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
/*__________*/

/* DEFINES */
#define Manual 0
#define Automatic 1
/*_________*/

/* VARIÁVEIS DESTE MÓDULO */
_Bool Mode;
uint8_t b = 0x20;
/*________________________*/

/* FUNÇÕES DESTE MÓDULO */
_Bool Sampling_Time(uint8_t*);
int Parsing(uint8_t*, char);
void clean_string_array(char);
int Units_Processement(char string_array[6][6], float *period);
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef*);
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef*);
/*______________________*/

#endif /* INC_CONTROL_H_ */
