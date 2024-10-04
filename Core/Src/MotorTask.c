#include "FreeRTOS.h"
#include "task.h"
#include "math.h"
#include "can.h"
#include "stm32f4xx_hal_can.h"
#include "main.h"
#include "Motor.h"
#include "PID.h"
#include "rx.h"

enum Spin_Mode_
{
    STOP = 0,
    SLOW = 1,
    MIDDLE = 3,
    FAST = 5,
    VERY_FAST = 9,
};

int16_t spin_velocity = 0;
uint8_t ChassisMotorGiven[8];
int Spin_Mode = SLOW;
int can_state;
extern int count;
extern double sec;
PID_TypeDef Spin_PID;
PID_TypeDef Move_Speed_PID;
PID_TypeDef Move_Angle_PID;
extern Motor_TypeDef Chassis_Motor[4];
    int angle_cnt = 0;
    int target = 0;
    int point1_flag = 0;

int v=0;

void MotorTask(void const * argument)
{

	
	CAN_TxHeaderTypeDef CAN1_TxdHeader;

	uint32_t MailBox;
	
	CAN1_TxdHeader.StdId = Motor_CANID_200;
	CAN1_TxdHeader.IDE   = CAN_ID_STD;
	CAN1_TxdHeader.RTR   = CAN_RTR_DATA;
	CAN1_TxdHeader.DLC   = 8;

    PID_Set(&Spin_PID, PID_Mode_Angle, 50, 0.8, 5, 2500, 1000, 300, 1);
    PID_Set(&Move_Speed_PID, PID_Mode_Angle, 4, 0.1, 20, 3500, 1000, 300, 1);
    PID_Set(&Move_Angle_PID, PID_Mode_Angle, 4, 0, 3, 3500, 1000, 300, 1);

    Motor_Reset(&Chassis_Motor[0]);
	Motor_Reset(&Chassis_Motor[1]);
	Motor_Reset(&Chassis_Motor[2]);
	Motor_Reset(&Chassis_Motor[3]);
	Motor_SetCANID(&Chassis_Motor[0], Motor_CANID_1);
	Motor_SetCANID(&Chassis_Motor[1], Motor_CANID_2);
	Motor_SetCANID(&Chassis_Motor[2], Motor_CANID_3);
	Motor_SetCANID(&Chassis_Motor[3], Motor_CANID_4);

	vTaskDelay(1000);


		sec = 0;
    while(1)
    {
        spin_velocity = 55 + Spin_Mode * sin(sec/10) * 20 ;

        target = spin_velocity;
        Spin_PID.Target = spin_velocity;
        Spin_PID.Measure = Chassis_Motor[0].Governor.Speed/19 * 1;
        
        PID_Calculate(&Spin_PID);

        ChassisMotorGiven[0] = Spin_PID.Out>>8;
        ChassisMotorGiven[1] = Spin_PID.Out>>0;
//			    ChassisMotorGiven[0] = 0>>8;
//          ChassisMotorGiven[1] = 0>>0;

        float TotalAngle_Deg = Chassis_Motor[2].Data.TotalAngle/8192.0f/19.0f;


//        if((int)(sec*10) % 100 == 0)
//				{
//					Move_Angle_PID.Target = (angle_cnt++)*360;
//					Move_Angle_PID.Measure = TotalAngle_Deg;				
//				}

//        if(angle_cnt == 10 ) 
//        {
//            target = 0;
//            angle_cnt = 0;
//        }

				Move_Angle_PID.Target =-1440 + 8 * sin((sec + 3.1415f / 2.0f)/30) * 180;//1440
				Move_Angle_PID.Measure = TotalAngle_Deg * 360;	

        PID_Calculate(&Move_Angle_PID);
        Move_Speed_PID.Target = Move_Angle_PID.Out;
        Move_Speed_PID.Measure = Chassis_Motor[2].Governor.Speed;
        PID_Calculate(&Move_Speed_PID);

        ChassisMotorGiven[4] = Move_Speed_PID.Out>>8;
        ChassisMotorGiven[5] = Move_Speed_PID.Out>>0;
				
//				ChassisMotorGiven[4] = 0>>8;
//        ChassisMotorGiven[5] = 0>>0;

        can_state = HAL_CAN_AddTxMessage(&hcan1, &CAN1_TxdHeader, ChassisMotorGiven, &MailBox);
		vTaskDelay(5);
    }
}
