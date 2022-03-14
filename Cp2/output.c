/*
 * output.c
 *
 *  Created on: Mar 3, 2022
 *      Author: anaim
 */
#include "output.h"
#include"stdio.h"

#define a 1
#define ak (double[]){-3.75851366572717e-18, -0.00665083304372324, -0.0113881272151967, -0.00966088188712048, 5.98324927892853e-18, 0.0136717608030573, 0.0229186645446406, 0.0192297324248841, -7.93601786897886e-18, -0.0275153371422712, -0.0473561112426106, -0.0416231564379072, 9.27118580834223e-18, 0.0729584711821116, 0.157195709928694, 0.224384339381762, 0.250000000000000, 0.224384339381762, 0.157195709928694, 0.0729584711821116, 9.27118580834223e-18, -0.0416231564379072, -0.0473561112426106, -0.0275153371422712, -7.93601786897886e-18, 0.0192297324248841, 0.0229186645446406, 0.0136717608030573, 5.98324927892853e-18, -0.00966088188712048, -0.0113881272151967, -0.00665083304372324, -3.75851366572717e-18}
#define bk (double[]) {-3.75851366572717e-18, -0.00665083304372324, -0.0113881272151967, -0.00966088188712048, 5.98324927892853e-18, 0.0136717608030573, 0.0229186645446406, 0.0192297324248841, -7.93601786897886e-18, -0.0275153371422712, -0.0473561112426106, -0.0416231564379072, 9.27118580834223e-18, 0.0729584711821116, 0.157195709928694, 0.224384339381762, 0.250000000000000, 0.224384339381762, 0.157195709928694, 0.0729584711821116, 9.27118580834223e-18, -0.0416231564379072, -0.0473561112426106, -0.0275153371422712, -7.93601786897886e-18, 0.0192297324248841, 0.0229186645446406, 0.0136717608030573, 5.98324927892853e-18, -0.00966088188712048, -0.0113881272151967, -0.00665083304372324, -3.75851366572717e-18}
#define N 31
#define M 31

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
		Y_Buffer[count_y++]=Filtro_FIR();
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

double Filtro_FIR()
{
	double y=0;
	int index;
	for(index =0; index < M ;index++)
	{
		y=y+bk[index]*X_ant_Buffer[index];
	}

	return y;
}

double Filtro()
{
	double y=0;
	int index;
	for(index =0; index < M ;index++)
	{
		y=y+bk[index]*X_ant_Buffer[index];
	}
	for(index =0; index < N ;index++)
	{
		y=y+ak[index]*Y_ant_Buffer[index];
	}

	return y;
}

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

