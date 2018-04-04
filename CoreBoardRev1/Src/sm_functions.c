/*
 * sm_functions.c
 *
 *  Created on: Mar 6, 2018
 *      Author: shwet
 */


#include "sm_functions.h"
#include "mycan.h"
#include "gpio.h"
#include "can.h"
#include "tim.h"
#include "stm32f4xx_hal.h"

extern uint16_t throttle_val;

void RESET_FAULTS()
{
	send_CAN(MID_RESET_FAULTS, 0);
}

void END_DRIVE()
{
	send_CAN(MID_END_DRIVE, 0);
	throttle_val = 0;
	send_CAN(MID_TORQUE_COMMAND, 0);
}

void RTDS()
{
	//assert RTDS GPIO active
	HAL_GPIO_WritePin(RTDS_GPIO_Port, RTDS_Pin, GPIO_PIN_SET);
	HAL_TIM_Base_Start_IT(&htim3); // starts timer, when timer interrupt occurs, will reset RTDS sound
}
// Functions for state transitions. Takes in CAN message and current state. Function names in form of SmState_Event.
void PEDAL_ACEL()
{
	send_CAN(MID_TORQUE_COMMAND, throttle_val);
}

void PWR_80()
{
	send_CAN(MID_REDUCE_PWR, 0);
}
void BPPC_RST()
{
	send_FLT_CAN(MID_FAULT_CAUSE, BPPC_FLT);
}
void IMD_NO_RST()
{
	send_FLT_CAN(MID_FAULT_CAUSE, IMD_FLT);
}
void BSPD_NO_RST()
{
	send_FLT_CAN(MID_FAULT_CAUSE, BSPD_FLT);
}
void APPS_NO_RST()
{
	send_FLT_CAN(MID_FAULT_CAUSE, APPS_FLT);
}
void BSE_NO_RST()
{
	send_FLT_CAN(MID_FAULT_CAUSE, BSE_FLT);

}
void BMS_NO_RST()
{
	send_FLT_CAN(MID_FAULT_CAUSE, BMS_FLT);
}

void RST()
{
	send_FLT_CAN(MID_FAULT_CAUSE, GENERIC_RST);
}
void NO_RST()
{
	send_FLT_CAN(MID_FAULT_CAUSE, GENERIC_NO_RST);
}


void send_CAN(uint16_t MID, uint16_t message)
{
	// helper function for all the other functions, also for transitions that do nothing but change state
	// will send the car state over can
	// transmit CAN message idiom
	can_msg_t can_msg;
	CAN_short_msg(&can_msg, create_ID(BID_CORE, MID), message);
	CAN_queue_transmit(&can_msg);
}

void send_FLT_CAN(uint16_t MID, uint16_t message)
{
	// helper function to send CAN specifically for faults
	// send_CAN(MID, message); // Don't want to send fault type, just want MC to get 0 torque command

	// sending 0 torque command
	throttle_val = 0;
	can_msg_t can_msg;
	CAN_short_msg(&can_msg, create_ID(BID_CORE, MID_TORQUE_COMMAND), 0);
	CAN_queue_transmit(&can_msg);

}

void do_nothing()
{
    return;
}

void PEDAL_BRAKE_PUSHED()
{
    // GPIOs
	// occurs when brake pressed enough to turn on brake light, send CAN
	HAL_GPIO_WritePin(BRAKELIGHT_GPIO_Port, BRAKELIGHT_Pin, GPIO_PIN_SET);
}

void PEDAL_BRAKE_RELEASED()
{
	// GPIOs
    // occurs when brake released, light should be turned off, send CAN

	HAL_GPIO_WritePin(BRAKELIGHT_GPIO_Port, BRAKELIGHT_Pin, GPIO_PIN_RESET);
}
