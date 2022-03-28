/*
 * Velocity_Mesure.h
 *
 *  Created on: Mar 25, 2022
 *      Author: Ines
 */

#ifndef INC_VELOCITY_MESUREH
#define INC_VELOCITY_MESUREH


#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"
#include "tim.h"
#include "comandos.h"
#include "usart.h"
#include "gpio.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

extern _Bool stop;
extern _Bool start;
extern _Bool Sample_K;
extern int K_value;

int Units_Processement(char string_array[6][6], float *period);
_Bool Sampling_Period(uint8_t *buffer1);
_Bool Velocity_Units(uint8_t *buffer1);
void direction();
_Bool Start(uint8_t *buffer1);
_Bool Stop();
void time_calculator();
#endif / INC_VELOCITY_MESUREH */
