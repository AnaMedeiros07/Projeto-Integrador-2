/*
 * comandos.h
 *
 *  Created on: 15/04/2022
 *      Author: Ines
 */

#ifndef INC_COMANDOS_H_
#define INC_COMANDOS_H_


#ifdef __cplusplus
extern "C" {
#endif
#define PWM_mode 0
#define Control_mode 1
#define Valid 1
#define Clock 1
#define Anti_Clock -1
/* INCLUDES */
#include "main.h"
/*__________*/

/* VARIÁVEIS DESTE MÓDULO */
extern _Bool Output;
extern int prompt_flag;
extern _Bool Direction;
/*________________________*/

/* FUNÇÕES DESTE MÓDULO */
_Bool Check_Comand(uint8_t *buffer);
_Bool Invalid();
_Bool Start(uint8_t *buffer);
_Bool Stop();
void Print();
void Increment();
void Decrement();
/*______________________*/

#endif /* INC_COMANDOS_H_ */
