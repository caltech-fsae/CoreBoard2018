/*
 * sm_functions.c
 *
 *  Created on: Mar 6, 2018
 *      Author: shwet
 */


#include "sm_functions.h"
#include "mycan.h"

void RTDS()
{
	//assert RTDS GPIO active
}
// Functions for state transitions. Takes in CAN message and current state. Function names in form of SmState_Event.
void PEDAL_ACEL()
{

    // occurs when acel value changed enough to care
}
void PEDAL_BRAKE_PUSHED()
{
    // occurs when brake pressed enough to turn on brake light, send CAN
}
void PEDAL_BRAKE_RELEASED()
{
    // occurs when brake released, light should be turned off, send CAN
}
void PWR_80()
{
    // occurs when power over 80 kW
}
void BPPC_RST()
{
	sending_FLT_CAN(MID_FAULT_CAUSE, BPPC_FLT);
}
void IMD_NO_RST()
{
	sending_FLT_CAN(MID_FAULT_CAUSE, IMD_FLT);
}
void BSPD_NO_RST()
{
	sending_FLT_CAN(MID_FAULT_CAUSE, BSPD_FLT);
}
void APPS_NO_RST()
{
	sending_FLT_CAN(MID_FAULT_CAUSE, APPS_FLT);
}
void BSE_NO_RST()
{
	sending_FLT_CAN(MID_FAULT_CAUSE, BSE_FLT);

}
void BMS_NO_RST()
{
	sending_FLT_CAN(MID_FAULT_CAUSE, BMS_FLT);
}

void RST()
{
	sending_FLT_CAN(MID_FAULT_CAUSE, GENERIC_RST);
}
void NO_RST()
{
	sending_FLT_CAN(MID_FAULT_CAUSE, GENERIC_NO_RST);
}


void sending_CAN(uint16_t MID, uint16_t message)
{
	// helper function for all the other functions, also for transitions that do nothing but change state
	// will send the car state over can
	// transmit CAN message idiom
	can_msg_t can_msg;
	CAN_short_msg(&can_msg, create_ID(BID_CORE, MID), message);
	CAN_queue_transmit(&can_msg);
}

void sending_FLT_CAN(uint16_t MID, uint16_t message)
{
	// helper function to send CAN specifically for faults
	sending_CAN(MID, message);

	// sending 0 torque command
	can_msg_t can_msg;
	CAN_short_msg(&can_msg, create_ID(BID_CORE, MID_TORQUE_COMMAND), 0);
	CAN_queue_transmit(&can_msg);

}

void do_nothing()
{
    // do nothing
}

void BRAKE_LIGHT_ON()
{
	//use to turn on brake light, no CAN
}

void BRAKE_LIGHT_OFF()
{
	//use to turn off brake light, no CAN
}
