/*
 * PWM.h
 *
 *  Created on: Apr 19, 2022
 *      Author: Ines
 */

#ifndef INC_PWM_H_
#define INC_PWM_H_

#ifdef __cplusplus
extern "C" {
#endif

/*_________Includes__________*/
#include "Comandos.h"
#include "usart.h"
#include "tim.h"
#include "string.h"
#include "Control.h"
/*___________________________*/

/*_________Functions_________*/
void Init_PWM_Control();
void DeInit_PWM_Control ();
_Bool Normalized_Tension(uint8_t *buffer1);
_Bool Change_Duty();
/*___________________________*/


#ifdef __cplusplus
}
#endif

#endif /* INC_PWM_H_ */
