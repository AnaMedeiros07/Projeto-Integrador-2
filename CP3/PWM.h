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
#include "usart.h"
#include "tim.h"
#include "string.h"

void Init_PWM_Control();
void DeInit_PWM_Control ();
_Bool Normalized_Tension(uint8_t *buffer1);
void Increment_Duty();
void Decrement_Duty();
_Bool Change_Duty();

#endif /* INC_PWM_H_ */
