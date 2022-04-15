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

/* INCLUDES */
#include "Control.h"
/*__________*/

/* VARIÁVEIS DESTE MÓDULO */
/*________________________*/

/* FUNÇÕES DESTE MÓDULO */
_Bool Check_Comand(uint8_t);
_Bool Invalid();
_Bool Start(uint8_t);
_Bool Stop();
/*______________________*/

#endif /* INC_COMANDOS_H_ */
