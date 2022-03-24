/*
 * comandos.h
 *
 *  Created on: 14/03/2022
 *      Author: Ines
 */

#ifndef INC_COMANDOS_H_
#define INC_COMANDOS_H_


#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"
#define PWM_mode 0
#define Control_mode 1
#define invalid 1
#define Valid 1
#define Clock 1
#define Anti_Clock 1


extern uint8_t b;
extern _Bool stop;
extern _Bool start;
extern _Bool Output;
extern int valid;
extern int index_count;
extern int K_value;
extern _Bool Sample_K;
extern int adc_validation;
extern int prompt_flag;

_Bool Check_Comand(uint8_t *buffer);
_Bool Invalid();
_Bool Sampling_Period(uint8_t *buffer1);
_Bool Operation_Mode(uint8_t *buffer1);
void Print_Trama(double temp);
_Bool Enable(uint8_t *buffer1);
void Print();

#endif /* INC_COMANDOS_H_ */
