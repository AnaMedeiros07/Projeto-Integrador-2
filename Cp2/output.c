/*
 * output.c
 *
 *  Created on: Mar 3, 2022
 *      Author: anaim
 */
#include "output.h"
#include"stdio.h"

#define a 0.4
int N_Buffer[128];
double X_Buffer[128];
double X_ant_Buffer[128]={0};
double Y_ant_Buffer[128]={0};
double Y_Buffer[128]={0};
_Bool filter_on=0;
static int count_n=0;
static int count_x=0;
static int count_y=0;
int Save_N_Buffer()
{
	static int local_index_n=0;
	N_Buffer[local_index_n++]=(++count_n);
	local_index_n &= ~(1<<7); //(128-1)
	count_n &= ~(1<<7);
	return count_n;
}

void Save_X_Buffer(double value)
{
	X_Buffer[count_x++]=value;
	count_x &= ~(1<<7); //(128-1)
}
void Save_X_ant()
{
	for(int index =count_x; index > 1;index--)
	{
		X_ant_Buffer[index]=X_ant_Buffer[index-1];
	}
	X_ant_Buffer[0]=X_Buffer[count_x-1];

}
void Save_Y_ant()
{
	for(int index =count_y; index > 1;index--)
	{
		Y_ant_Buffer[index]=Y_ant_Buffer[index-1];
	}
	Y_ant_Buffer[0]=Y_Buffer[count_x-1];
}
double Save_Y()
{
	if(filter_on==1)
		Y_Buffer[count_y++]=Filtro_IIR();
	else
		Y_Buffer[count_y++]=X_Buffer[count_y];

	count_y &= ~(1<<7); //(128-1)

	return Y_Buffer[count_y];

}
double Filtro_IIR()
{
	double y=0;
	for(int index =0; index < count_x;index++)
	{
		y=y+(1-a)*X_ant_Buffer[index];
	}
	for(int index =1; index < count_y;index++)
	{
		y=y+a*Y_ant_Buffer[index];
	}
	return y;
}
void Reset()
{
	count_n &=0;
	count_x &=0;
	count_y &=0;
}


