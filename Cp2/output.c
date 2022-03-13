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
	int i=0;
	for(i =count_x; i >= 1;i--)
	{
		X_ant_Buffer[i]=X_ant_Buffer[i-1];
	}
	if(count_x == 0)
		X_ant_Buffer[0]=X_Buffer[127];
	else
		X_ant_Buffer[0]=X_Buffer[count_x-1];

}
void Save_Y_ant()
{
	int index=0;
	for(index =count_y; index >0;index--)
	{
		Y_ant_Buffer[index]=Y_ant_Buffer[index-1];
	}
	if(count_y == 0)
		Y_ant_Buffer[0]=Y_Buffer[127];
	else
		Y_ant_Buffer[0]=Y_Buffer[count_y-1];
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

	y=(a*Y_ant_Buffer[0])+((1-a)*X_ant_Buffer[0]);

	return y;
}

/*double Filtro_FIR()
{
	double y=0;
	for(int index =0; index < count_x;index++)
	{
		y=y+Mudar(1-a)*X_ant_Buffer[index];
	}
	for(int index =0; index < count_y;index++)
	{
		y=y+Mudara*Y_ant_Buffer[index];
	}
	return y;
}*/

void Reset()
{

	int index=0;
	for(index =0; index <= count_y;index++)
	{
		Y_ant_Buffer[index]=0;
	}
	for(index =0; index <= count_x;index++)
	{
			X_ant_Buffer[index]=0;
	}

	count_n = 0;
	count_x = 0;
	count_y = 0;
}
