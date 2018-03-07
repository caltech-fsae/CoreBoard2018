/*
 * sm_functions.c
 *
 *  Created on: Mar 6, 2018
 *      Author: shwet
 */


#include "sm_functions.h"

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
    // send 0 torque command
	// send entering reset fault state from bppc
}
void IMD_NO_RST()
{
	// send 0 torque command
    // send entering reset fault state from imd
}
void BSPD_NO_RST()
{
	// send 0 torque command
	// send entering reset fault state from BSPD
}
void APPS_NO_RST()
{
    //
}
void BSE_NO_RST()
{
    //
}
void BMS_NO_RST()
{
    //
}

void RST()
{
	//
}
void NO_RST()
{
	//
}

void send_CAN()
{
	// helper function for all the other functions, also for transitions that do nothing but change state
	// will send the car state over can
	// transmit CAN message idiom
		  /*
		  msg = 0x00000 // update with appropriate msg, construct from common.h/c
		  CAN_short_msg(&can_msg, create_ID(0,0), msg);
		  CAN_queue_transmit(&can_msg);
		  */
}

void do_nothing()
{
    // return
}

void BRAKE_LIGHT_ON()
{
	//use to turn on brake light, no CAN
}

void BRAKE_LIGHT_OFF()
{
	//use to turn off brake light, no CAN
}
