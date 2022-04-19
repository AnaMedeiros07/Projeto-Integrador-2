/*
 * Comandos.h
 *
 *  Created on: Apr 18, 2022
 *      Author: Ines
 */

#ifndef INC_COMANDOS_H_
#define INC_COMANDOS_H_

#ifdef __cplusplus
extern "C"{
#endif

/*_________Includes__________*/
#include "stdio.h"
#include "usart.h"
#include "Control.h"
#include "PWM.h"
/*___________________________*/

/*__________Defines__________*/
#define Clock 1
#define Anti_Clock -1
/*___________________________*/

/*_________Variable__________*/
extern _Bool prompt_flag;
extern _Bool valid;
extern _Bool Sample_K;
extern _Bool stop;
extern _Bool start;
extern _Bool Direction;

extern int K_value;
/*___________________________*/

/*_________Functions_________*/
_Bool Check_Comand(uint8_t *buffer);
void Print();
_Bool Start(uint8_t* buffer);
_Bool Stop();
_Bool Invalid();
/*___________________________*/

#ifdef __cplusplus
}
#endif

#endif /* INC_COMANDOS_H_ */
