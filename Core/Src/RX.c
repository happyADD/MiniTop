/**
 * @file RX.c
 * @author DCY
 * @brief 接收信号函数，不必改动
 * @version 0.1
 * @date 2024-07-21
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include "rx.h"
#include "can.h"
#include "PID.h"
#include "motor.h"


Motor_TypeDef Chassis_Motor[4];


void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
	static uint8_t CAN_RxdData[8];
	static CAN_RxHeaderTypeDef CAN_RxdHeader;

	if (hcan->Instance == CAN1)
	{
		HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &CAN_RxdHeader, CAN_RxdData);

		// Update Motor Data
		if (CAN_RxdHeader.StdId == Chassis_Motor[0].CANID)
			Motor_UpdateData(&Chassis_Motor[0], CAN_RxdData);
		if (CAN_RxdHeader.StdId == Chassis_Motor[1].CANID)
			Motor_UpdateData(&Chassis_Motor[1], CAN_RxdData);
		if (CAN_RxdHeader.StdId == Chassis_Motor[2].CANID)
			Motor_UpdateData(&Chassis_Motor[2], CAN_RxdData);
		if (CAN_RxdHeader.StdId == Chassis_Motor[3].CANID)
			Motor_UpdateData(&Chassis_Motor[3], CAN_RxdData);

	}
}
