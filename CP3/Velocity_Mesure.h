/*
 * Velocity_Mesure.h
 *
 *  Created on: Mar 25, 2022
 *      Author: Ines
 */

#ifndef INC_VELOCITY_MESURE_H_
#define INC_VELOCITY_MESURE_H_


#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

int Units_Processement(char string_array[6][6], float *period);
_Bool Sampling_Period(uint8_t buffer1);

#endif /* INC_VELOCITY_MESURE_H_ */
