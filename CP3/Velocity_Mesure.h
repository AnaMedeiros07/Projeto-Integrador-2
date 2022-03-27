/*
 * Velocity_Mesure.h
 *
 *  Created on: Mar 25, 2022
 *      Author: Ines
 */

#ifndef INC_VELOCITY_MESURE_H_
#define INC_VELOCITY_MESURE_H_


#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"
#include "tim.h"
#include "comandos.h"
#include "usart.h"

extern _Bool stop;
extern _Bool start;
extern _Bool Sample_K;
void Direction();
extern int K_value;

int Units_Processement(char string_array[6][6], float *period);
_Bool Sampling_Period(uint8_t *buffer1);
_Bool Velocity_Units(uint8_t *buffer1);
_Bool Start(uint8_t *buffer1);
_Bool Stop();
#endif /* INC_VELOCITY_MESURE_H_ */
