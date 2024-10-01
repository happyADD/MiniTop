/**
  ******************************************************************************
  * @file    pid.h
  * @author  Karolance Future
  * @version V2.3.2
  * @date    2021/10/05
  * @brief   Header file of pid.c
  ******************************************************************************
  * @attention CtrlCV
  *
  ******************************************************************************
  */
	
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __PID_H__
#define __PID_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stdint.h"

typedef enum
{
	PID_Mode_Angle = 1,
	PID_Mode_Speed = 2,
} PID_Mode_TypeDef;

typedef struct
{
	PID_Mode_TypeDef Mode;
	
	float kp;
	float ki;
	float kd;

	int32_t Err;          
	int32_t LastErr;      
	int32_t PrevErr;      
	
	float   Pout;
	float   Iout;
	float   Dout;
	int32_t Out;
	
	int32_t MaxOut;       
	int32_t IntegralLim;  
	int32_t IErrLim;      
	uint8_t DeadBand;     
	int32_t Target;       
	float Measure;      
} PID_TypeDef;


void PID_Set(PID_TypeDef *pid,PID_Mode_TypeDef mode,float kp,float ki,float kd,float maxOut,float integralLim,float iErrLim,float deadBand);

void PID_DefultReset(PID_TypeDef *pid);
void PID_DefultInit(PID_TypeDef *pid);
void PID_Copy(PID_TypeDef *pid,PID_TypeDef *pid_copy);
int PID_DeCalculate(PID_TypeDef *SpeedPid,PID_TypeDef *PositionPID,int PID_Out);
void PID_Calculate(PID_TypeDef *pid);

void PID_xMoveInit_Target(PID_TypeDef *pid);
void PID_yMoveInit_Target(PID_TypeDef *pid);
void PID_wMoveInit_Target(PID_TypeDef *pid);

#ifdef __cplusplus
}
#endif

#endif /* __PID_H__ */

