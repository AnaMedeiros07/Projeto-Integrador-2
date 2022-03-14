/*
 * comandos.h
 *
 *  Created on: 13/03/2022
 *      Author: anaim
 */

#ifndef INC_COMANDOS_H_
#define INC_COMANDOS_H_


#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

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

int Check_Comand(uint8_t *buffer);
int Invalid();
int Sampling_Period(uint8_t *buffer1);
int Analog_Channel(uint8_t *buffer1);
void Print_Trama(double temp);
int Start(uint8_t *buffer1);
int Stop();
void Print();

#endif /* INC_COMANDOS_H_ */
