/******************************************************************************/
/*----------------------------------Includes----------------------------------*/
/******************************************************************************/
#include "InfineonRacer.h"
#include "Basic.h"

/******************************************************************************/
/*-----------------------------------Macros-----------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*--------------------------------Enumerations--------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*-----------------------------Data Structures--------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*------------------------------Global variables------------------------------*/
/******************************************************************************/

InfineonRacer_t IR_Ctrl  /**< \brief  global data */
		={64, 64, 64};
/******************************************************************************/
/*-------------------------Function Prototypes--------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*------------------------Private Variables/Constants-------------------------*/
/******************************************************************************/

/******************************************************************************/
/*-------------------------Function Implementations---------------------------*/
/******************************************************************************/
void InfineonRacer_init(void){
	;
}

void InfineonRacer_detectLane(void){
	/* IR_LineScan.adcResult 의 정보를 읽어들여서
	 * IR_Ctrl.Ls0Margin, IR_Ctrl.Ls1Margin 정보를 계산한다
	 */
	for(int i=4; i<124 ; i++)
	{
		IR_Ctrl.LCamera[i-4]= IR_LineScan.adcResult[0][i];
		IR_Ctrl.RCamera[i-4]= IR_LineScan.adcResult[1][i];
	}
	IR_Ctrl.LMAX =0;
	IR_Ctrl.RMAX =0;
	IR_Ctrl.LMIN =4096;
	IR_Ctrl.RMIN =4096;
	for(int i=0; i<120; i++)
	{
		if(IR_Ctrl.LCamera[i]>IR_Ctrl.LMAX)
		{
			IR_Ctrl.LMAX=IR_Ctrl.LCamera[i];
		}
		if(IR_Ctrl.LCamera[i]<IR_Ctrl.LMIN)
		{
			IR_Ctrl.LMIN=IR_Ctrl.LCamera[i];
		}
		if(IR_Ctrl.RCamera[i]>IR_Ctrl.RMAX)
		{
			IR_Ctrl.RMAX=IR_Ctrl.RCamera[i];
		}
		if(IR_Ctrl.RCamera[i]<IR_Ctrl.RMIN)
		{
			IR_Ctrl.RMIN=IR_Ctrl.RCamera[i];
		}
	}
	IR_Ctrl.LAVR=(IR_Ctrl.LMAX+IR_Ctrl.LMIN)/2;
	IR_Ctrl.RAVR=(IR_Ctrl.RMAX+IR_Ctrl.RMIN)/2;
	for(int i=0; i<120; i++)
	{
		if(IR_Ctrl.LCamera[i]>IR_Ctrl.LAVR)
		{
			IR_Ctrl.LCon[i] =1;
		}
		else
		{
			IR_Ctrl.LCon[i] =0;
		}

		if(IR_Ctrl.RCamera[i]>IR_Ctrl.RAVR)
		{
			IR_Ctrl.RCon[i] =1;
		}
		else
		{
			IR_Ctrl.RCon[i] =0;
		}
	}
	for(int i=0; i<118; i++)
	{
		IR_Ctrl.LEdge[i] = IR_Ctrl.LCon[i]*(-1) +
							IR_Ctrl.LCon[i+1]*2 +
							IR_Ctrl.LCon[i+2]*(-1);
	}
	for(int i=0; i<118; i++)
	{
		IR_Ctrl.REdge[i] = IR_Ctrl.RCon[i]*(-1) +
							IR_Ctrl.RCon[i+1]*2 +
							IR_Ctrl.RCon[i+2]*(-1);
	}
	for(int i=0; i<118; i++)
	{
		if(IR_Ctrl.LEdge[i] == 1)
		{
			IR_Ctrl.LLin = i;
		}
	}
	for(int i=117; i>=0; i--)
	{
		if(IR_Ctrl.REdge[i] == 1)
		{
			IR_Ctrl.RLin = i;
		}
	}
}

void InfineonRacer_detectCrossWalk(void){
	/* IR_LineScan.adcResult 의 정보를 읽어들여서
	 * 횡단보도를 검출한다.
	 */
	/*for(int i=0; i<126 ; i++)
	{
		IR_Ctrl.LPF1_3[i] = (float32)IR_LineScan.adcResult[0][i]*0.25
							+ (float32)IR_LineScan.adcResult[0][i+1]*0.5
							+ (float32)IR_LineScan.adcResult[0][i+2]*0.25;

	}*/
	IR_Ctrl.Count=0;
	IR_Ctrl.MAX =0;

	for(int i=0; i<124 ; i++)
	{
		IR_Ctrl.LPF1_5[i] = (float32)IR_LineScan.adcResult[2][i]*0.1
							+ (float32)IR_LineScan.adcResult[2][i+1]*0.2
							+ (float32)IR_LineScan.adcResult[2][i+2]*0.4
							+ (float32)IR_LineScan.adcResult[2][i+3]*0.2
							+ (float32)IR_LineScan.adcResult[2][i+4]*0.1;

	}

	for(int i=0; i<124 ; i++)
	{
		if(IR_Ctrl.LPF1_5[i]>IR_Ctrl.MAX)
		{
			IR_Ctrl.MAX = IR_Ctrl.LPF1_5[i];
		}
	}
	for(int i =0 ; i<124 ; i++)
	{
		if(IR_Ctrl.LPF1_5[i]>IR_Ctrl.MAX*4/7)
		{
			IR_Ctrl.ConHL[i]=1;
		}
		else
		{
			IR_Ctrl.ConHL[i]=0;
		}
	}
	for(int i=0; i<122 ; i++)
	{
		IR_Ctrl.Edge[i] = IR_Ctrl.ConHL[i]*(-1)
							+ IR_Ctrl.ConHL[i+1]*2
							+ IR_Ctrl.ConHL[i+2]*(-1);
	}
	for(int i=0; i<122 ; i++)
	{

		if(IR_Ctrl.Edge[i]==1)
		{
			IR_Ctrl.Count++;
		}

	}
} // Max 값은 유동적

void InfineonRacer_control(void){
	;
}
