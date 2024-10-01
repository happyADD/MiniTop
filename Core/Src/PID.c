/**
 * @file PID.c
 * @author DCY
 * @brief
 * @version 0.1
 * @date 2024-07-04
 *
 * @copyright Copyright (c) 2024
 *
 */

/* Includes and Defines ------------------------------------------------------*/
#include "pid.h"
#define abs(x) ((x > 0) ? (x) : (-x))
/**
 * @brief PID赋值用这个
 * 
 */
void PID_Set(PID_TypeDef *pid,PID_Mode_TypeDef mode,float kp,float ki,float kd,float maxOut,float integralLim,float iErrLim,float deadBand)
{
	pid->Mode = mode;
	pid->kp = kp;
	pid->ki = ki;
	pid->kd = kd;
	pid->MaxOut = maxOut;
	pid->IntegralLim = integralLim;
	pid->IErrLim = iErrLim;
	pid->DeadBand = deadBand;
}

void PID_xMoveInit_Target(PID_TypeDef *pid)
{
	pid->Mode = PID_Mode_Angle;
	pid->kp = 10;
	pid->ki = 0.1;
	pid->kd = 0;
	pid->MaxOut = 5000;
	pid->IntegralLim = 200;
	pid->IErrLim = 10;
	pid->DeadBand = 0;
}

void PID_yMoveInit_Target(PID_TypeDef *pid)
{
	pid->Mode = PID_Mode_Angle;
	pid->kp = 1;
	pid->ki = 0;
	pid->kd = 0;
	pid->MaxOut = 5000;
	pid->IntegralLim = 5;
	pid->IErrLim = 5;
	pid->DeadBand = 2;
}

void PID_wMoveInit_Target(PID_TypeDef *pid)
{
	pid->Mode = PID_Mode_Angle;
	pid->kp = 1;
	pid->ki = 0;
	pid->kd = 0;
	pid->MaxOut = 5000;
	pid->IntegralLim = 5;
	pid->IErrLim = 5;
	pid->DeadBand = 2;
}

void PID_DefultReset(PID_TypeDef *pid)
{
	pid->Err = 0;
	pid->LastErr = 0;
	pid->PrevErr = 0;
	pid->Pout = 0;
	pid->Iout = 0;
	pid->Dout = 0;
	pid->Out = 0;
	pid->Target = 0;
	pid->Measure = 0;
}

void PID_DefultInit(PID_TypeDef *pid)
{
	pid->Mode = PID_Mode_Speed;
	pid->kp = 0;
	pid->ki = 0;
	pid->kd = 0;
	pid->MaxOut = 0;
	pid->IntegralLim = 0;
	pid->IErrLim = 0;
	pid->DeadBand = 0;
}

void PID_Copy(PID_TypeDef *pid,PID_TypeDef *pid_copy)
{
	pid_copy->Mode = pid->Mode;
	pid_copy->kp = pid->kp;
	pid_copy->ki = pid->ki;
	pid_copy->kd = pid->kd;
	pid_copy->MaxOut = pid->MaxOut;
	pid_copy->IntegralLim = pid->IntegralLim;
	pid_copy->IErrLim = pid->IErrLim;
	pid_copy->DeadBand = pid->DeadBand;
}
inline int PID_DeCalculate(PID_TypeDef *SpeedPid,PID_TypeDef *PositionPID,int PID_Out)
{
	return (PID_Out/(SpeedPid->kp + SpeedPid->ki + SpeedPid->kd))/(PositionPID->kp + PositionPID->ki + PositionPID->kd);
}
void PID_Calculate(PID_TypeDef *pid)
{
	pid->PrevErr = pid->LastErr;
	pid->LastErr = pid->Err;
	pid->Err = pid->Target - pid->Measure;

	if (abs(pid->Err) > pid->DeadBand)
	{
		if (pid->Mode == PID_Mode_Speed) // delta PID
		{
			pid->Pout = pid->kp * (pid->Err - pid->LastErr);
			if (abs(pid->Err) < pid->IErrLim)
			{
				pid->Iout = pid->ki * pid->Err;
				pid->Iout = pid->Iout > pid->IntegralLim ? pid->IntegralLim : pid->Iout;
				pid->Iout = pid->Iout < -pid->IntegralLim ? -pid->IntegralLim : pid->Iout;
			}
			pid->Dout = pid->kd * (pid->Err - (pid->LastErr << 1) + pid->PrevErr);

			pid->Out += pid->Pout + pid->Iout + pid->Dout;
		}
		if (pid->Mode == PID_Mode_Angle)
		{
			pid->Pout = pid->kp * pid->Err;
			if (abs(pid->Err) < pid->IErrLim)
			{
				pid->Iout += pid->ki * pid->Err;
				pid->Iout = pid->Iout > pid->IntegralLim ? pid->IntegralLim : pid->Iout;
				pid->Iout = pid->Iout < -pid->IntegralLim ? -pid->IntegralLim : pid->Iout;
			}
			pid->Dout = pid->kd * (pid->Err - pid->LastErr);

			pid->Out = pid->Pout + pid->Iout + pid->Dout;
		}

		pid->Out = pid->Out > pid->MaxOut ? pid->MaxOut : pid->Out;
		pid->Out = pid->Out < -pid->MaxOut ? -pid->MaxOut : pid->Out;
	}
}
