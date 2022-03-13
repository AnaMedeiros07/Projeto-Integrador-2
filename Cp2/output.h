/*
 * output.h
 *
 *  Created on: Mar 3, 2022
 *      Author: anaim
 */

#ifndef INC_OUTPUT_H_
#define INC_OUTPUT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

extern int N_Buffer[128];
extern double X_Buffer[128];
extern double Y_Buffer[128];
extern _Bool filter_on;

void Save_X_Buffer(double value);
int Save_N_Buffer();
void Save_X_ant();
void Save_Y_ant();
double Save_Y();
double Filtro_IIR();
double Filtro_FIR();
void Reset();
#endif /* INC_OUTPUT_H_ */

