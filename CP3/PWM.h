/*
 * PWM.h
 *
 *  Created on: 24/03/2022
 *      Author: Ines
 */

#ifndef INC_PWM_H_
#define INC_PWM_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"
#include "comandos.h"
#include "tim.h"

_Bool Direction;

void Init_PWM_Control();
void DeInit_PWM_Control ();
_Bool Normalized_Tension(uint8_t *buffer1);

#endif /* INC_PWM_H_ */
