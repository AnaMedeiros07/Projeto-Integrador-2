/*
 * Velocity_Control.h
 *
 *  Created on: Mar 25, 2022
 *      Author: Ines
 */

#ifndef INC_VELOCITY_CONTROL_H_
#define INC_VELOCITY_CONTROL_H_


#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"
#include "comandos.h"
#include "usart.h"
#include "string.h"

_Bool Reference_Velocity(uint8_t *buffer1);
void Increment_Velocity();
void Decrement_Velocity();
_Bool Change_Velocity();
#endif /* INC_VELOCITY_CONTROL_H_ */
