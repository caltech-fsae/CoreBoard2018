/*
 * Core.c
 *
 *  Created on: Mar 11, 2018
 *      Author: shwet
 */

#include "Core.h"

extern StateMachine sm;
uint16_t throttle_val = 0;
uint16_t brake_val = 0;
int brake_pressed = 0;
int throttle_pressed = 1;
int heartbeat_counter[4] = {RESET_HEARTBEAT, RESET_HEARTBEAT, RESET_HEARTBEAT, RESET_HEARTBEAT}; // bms shutdown mc io
int start_button_counter = RESET_START;
int flt_r_counter = RESET_FLT_CNT;
int flt_nr_counter = RESET_FLT_CNT;
int button_pressed = 0;
int first_run = 1;
int send_torque = 0;
int init_heartbeat[4] = {0, 0, 0, 0}; // bms shutdown mc io
enum Boards {
	BMS_HEARTBEAT = 0,
	SHUTDOWN_HEARTBEAT = 1,
	MC_HEARTBEAT = 2,
	IO_HEARTBEAT = 3
};



void get_CAN() // this is in the scheduler along with mainloop, runs every cycle
{
	can_msg_t msg;
	if(CAN_dequeue_msg(&msg))
	{
		uint16_t board_id = 0b0000000000001111 & msg.identifier;
		uint16_t type     = 0b0000011111110000 & msg.identifier;
		uint16_t message  = CAN_decode_short(&msg);

		switch(board_id) { // bms shutdown mc io
		    case (int) BID_BMS_MASTER:
		    	if (type == MID_HEARTBEAT)
		    	{
		    		init_heartbeat[BMS_HEARTBEAT] = 1;
		    		heartbeat_counter[BMS_HEARTBEAT] = RESET_HEARTBEAT;
		    	}
		    	break;
		    case (int) BID_SHUTDOWN:
		    	if (type == MID_HEARTBEAT)
		    	{
		    		init_heartbeat[SHUTDOWN_HEARTBEAT] = 1;
		    		heartbeat_counter[SHUTDOWN_HEARTBEAT] = RESET_HEARTBEAT;
		    	}
		    	else if (type == MID_FAULT_STATUS)
		    	{
		    		/*if (CHECK_BIT(message, 6)) // battery fault
		    		{
		    			RunEvent(&sm, E_NO_RST_FLT);
		    		}
		    		else if (CHECK_BIT(message, 5)) // interlock fault
		    		{
		    			RunEvent(&sm, E_NO_RST_FLT);
		    		}
		    		else if (CHECK_BIT(message, 3)) // generic nonresettable fault
		    		{
		    			RunEvent(&sm, E_NO_RST_FLT);
		    		}
		    		else if (CHECK_BIT(message, 1)) // AMS fault
		    		{
		    			RunEvent(&sm, E_AMS_FLT);
		    		}
		    		else if (CHECK_BIT(message, 2)) // IMD fault
		    		{
		    			RunEvent(&sm, E_IMD_FLT);
		    		}
		    		else if (CHECK_BIT(message, 0)) // BSPD fault
		    		{
		    			RunEvent(&sm, E_BSPD_FLT);
		    		}
		    		else if (CHECK_BIT(message, 4)) // generic resettable fault
		    		{
		    			RunEvent(&sm, E_RST_FLT);
		    		}*/
		    	}
		    	break;
		    case (int) BID_MOTOR_CONTROLLER:
		    	if (type == MID_HEARTBEAT)
		    	{
		    		init_heartbeat[MC_HEARTBEAT] = 1;
		    		heartbeat_counter[MC_HEARTBEAT] = RESET_HEARTBEAT;
		    	}
		    	else if (type == MID_BATTERY_CURRENT && message >= MAX_CURRENT)
		    	{
		    		RunEvent(&sm, E_PWR_80);
		    	}
		    	break;
		    case (int) BID_IO:
		    	if (type == MID_HEARTBEAT)
		    	{
		    		init_heartbeat[IO_HEARTBEAT] = 1;
		    		heartbeat_counter[IO_HEARTBEAT] = RESET_HEARTBEAT;
		    	}
		    	else if (type == MID_BPPC_BSPD)
		    	{
		    		// message 0b00: 0b0x = BSPD, NR
		    		//               0bx0 = BPPC, R
		    		if (CHECK_BIT(message, 0))
		    		{
		    		    RunEvent(&sm, E_BSPD_FLT); // NR FAULT
		    		}
		    		else if (CHECK_BIT(message, 1))
		    		{
		    			RunEvent(&sm, E_BPPC_FLT); // R FAULT
		    		}
		    	}
		    	else if (type == MID_THROTTLE)
		    	{
		    		if (throttle_pressed && !brake_pressed && send_torque) {
		    			throttle_val = (uint16_t) (((float) message)/(0xFFFF)*MAX_THROTTLE_VAL);
		    		} else
		    		{
		    		    throttle_val = 0;
		    		}
		    	}
		    	else if (type == MID_PEDAL_STATUS)
		    	{
		    		brake_pressed = message & 1;
		    		throttle_pressed = (message & 2) >> 1;
		    		if ((message & 1) == 0 && (message & 4) == 1) {
		    		   RunEvent(&sm, E_PEDAL_ACEL);
		    		}
		    		else if ((message & 1) == 1) {
		    		   throttle_val = 0;
		    		   RunEvent(&sm, E_PEDAL_BRAKE_PUSHED);
		    		}
		    		else {
		    		   RunEvent(&sm, E_PEDAL_BRAKE_RELEASED);
		    		}
		    	}
		    	break;
		    case (int) BID_CORE:
		    	break;
		}

	}
}

int CheckHeartbeats() {
    if (/*heartbeat_counter[BMS_HEARTBEAT] <= 0 ||*/ heartbeat_counter[SHUTDOWN_HEARTBEAT] <= 0 ||  heartbeat_counter[MC_HEARTBEAT] <= 0 || heartbeat_counter[IO_HEARTBEAT] <= 0)
    {
    	RunEvent(&sm, E_NO_RST_FLT);
    }
}

void DecrementHeartbeats() {
	// decrement all elements of heart beat array
		// heartbeat_counter[BMS_HEARTBEAT]--;
		heartbeat_counter[SHUTDOWN_HEARTBEAT]--;
		heartbeat_counter[MC_HEARTBEAT]--;
		heartbeat_counter[IO_HEARTBEAT]--;
}

void CheckFaultNR() {
	if(!HAL_GPIO_ReadPin(FLT_NR_GPIO_Port, FLT_NR_Pin))
	{
		WriteAUXLED(0,1);
		if (flt_nr_counter > 0) {
			flt_nr_counter--;
		}
		else {
		    RunEvent(&sm, E_NO_RST_FLT);
		}
	} else {
		WriteAUXLED(0,0);
		flt_nr_counter = RESET_FLT_CNT;
	}

}

int PulledFaultResettable() {
	if (!HAL_GPIO_ReadPin(FLT_GPIO_Port, FLT_Pin))
	{
		if (flt_r_counter > 0)
	    {
		 	flt_r_counter--;
		 	return 0;
		}
		else
		{
		   	return 1;
		}
	}
	else {
		flt_r_counter = RESET_FLT_CNT;
		return 0;
	}
}

void CheckFaultResettable(){
	if(PulledFaultResettable())
		RunEvent(&sm, E_RST_FLT);
}

int CheckStartButton() {
    if (HAL_GPIO_ReadPin(START_GPIO_Port, START_Pin) && start_button_counter > 0)
    {
    	start_button_counter--;
    }
	else if (HAL_GPIO_ReadPin(START_GPIO_Port, START_Pin) && start_button_counter <= 0 && !button_pressed)
	{
		RunEvent(&sm, E_START);
		button_pressed = 1; // button was pressed, must be released to register next press
		start_button_counter = RESET_START;
	}
	else
	{
		start_button_counter = RESET_START;
		button_pressed = 0;
	}
}


void mainloop() // this is in the scheduler along with get_CAN
{
	RunState(&sm);
}

void send_heartbeat()
{
	can_msg_t can_msg;
	CAN_short_msg(&can_msg, create_ID(BID_CORE, MID_HEARTBEAT), 0);
	CAN_queue_transmit(&can_msg);
	if (first_run)
	{
	    first_run = 0;
	    // RESET_FAULTS();
	}
}

void send_state()
{
	can_msg_t can_msg;
	CAN_short_msg(&can_msg, create_ID(BID_CORE, MID_CAR_STATE), (int) sm.current_state_);
	CAN_queue_transmit(&can_msg);
}

void assert_FLT_lines()
{
	if ((int) sm.current_state_ == NO_RST_FAULT)
	{
		//HAL_GPIO_WritePin(FLT_NR_CTRL_GPIO_Port, FLT_NR_CTRL_Pin, GPIO_PIN_SET);
	}
	else if ((int) sm.current_state_ == RST_FAULT)
	{
		//HAL_GPIO_WritePin(FLT_CTRL_GPIO_Port, FLT_CTRL_Pin, GPIO_PIN_SET);
	}
	else
	{
		//HAL_GPIO_WritePin(FLT_CTRL_GPIO_Port, FLT_CTRL_Pin, GPIO_PIN_RESET);
	}
}

void WriteAUXLED(uint8_t led, uint8_t state)
{
	// Sets auxiliary LED (0, 1, 2, 3) to state (1-on, 0-off)
	switch(led)
	{
	case 0:
		if(state)
		{
			HAL_GPIO_WritePin(AUX_LED_0_GROUP, AUX_LED_0_PIN, GPIO_PIN_SET);
		}
		else
		{
			HAL_GPIO_WritePin(AUX_LED_0_GROUP, AUX_LED_0_PIN, GPIO_PIN_RESET);
		}
		break;
	case 1:
		if(state)
		{
			HAL_GPIO_WritePin(AUX_LED_1_GROUP, AUX_LED_1_PIN, GPIO_PIN_SET);
		}
		else
		{
			HAL_GPIO_WritePin(AUX_LED_1_GROUP, AUX_LED_1_PIN, GPIO_PIN_RESET);
		}
		break;
	case 2:
		if(state)
		{
			HAL_GPIO_WritePin(AUX_LED_2_GROUP, AUX_LED_2_PIN, GPIO_PIN_SET);
		}
		else
		{
			HAL_GPIO_WritePin(AUX_LED_2_GROUP, AUX_LED_2_PIN, GPIO_PIN_RESET);
		}
		break;
	case 3:
		if(state)
		{
			HAL_GPIO_WritePin(AUX_LED_3_GROUP, AUX_LED_3_PIN, GPIO_PIN_SET);
		}
		else
		{
			HAL_GPIO_WritePin(AUX_LED_3_GROUP, AUX_LED_3_PIN, GPIO_PIN_RESET);
		}
		break;
	}
}

void ToggleAUXLED(uint8_t led)
{
	// Toggles auxiliary LED (0, 1, 2, 3)
	switch(led)
	{
	case 0:
		HAL_GPIO_TogglePin(AUX_LED_0_GROUP, AUX_LED_0_PIN);
		break;
	case 1:
		HAL_GPIO_TogglePin(AUX_LED_1_GROUP, AUX_LED_1_PIN);
		break;
	case 2:
		HAL_GPIO_TogglePin(AUX_LED_2_GROUP, AUX_LED_2_PIN);
		break;
	case 3:
		HAL_GPIO_TogglePin(AUX_LED_3_GROUP, AUX_LED_3_PIN);
		break;
	}
}

void WaitHeartbeatsFunc() {
	send_torque = 0;
	if(HAL_GetTick() > 1000) {
		CheckFaultNR();
	}
	init_heartbeat[BMS_HEARTBEAT] ? heartbeat_counter[BMS_HEARTBEAT]-- : 0;

	init_heartbeat[SHUTDOWN_HEARTBEAT] ? heartbeat_counter[SHUTDOWN_HEARTBEAT]--  : 0;
	init_heartbeat[MC_HEARTBEAT] ? heartbeat_counter[MC_HEARTBEAT]-- : 0;
	init_heartbeat[IO_HEARTBEAT] ? heartbeat_counter[IO_HEARTBEAT]-- : 0;
	// DecrementHeartbeats();
	//TODO(@bgberr): Should check for heartbeats of live boards even during this startup
	if  (/*init_heartbeat[BMS_HEARTBEAT] == 0 || */ init_heartbeat[SHUTDOWN_HEARTBEAT] == 0  || init_heartbeat[MC_HEARTBEAT] == 0 || init_heartbeat[IO_HEARTBEAT] == 0)
	{
		return;
	}
	else
	{
		RunEvent(&sm, E_BOARDS_LIVE);
	}
	CheckFaultResettable();
}
void WaitDriverFunc() {
	send_torque = 0;
	CheckFaultNR();
	DecrementHeartbeats();
	CheckHeartbeats();
	CheckFaultResettable();
	PEDAL_ACEL();
}
void StartBrakeFunc() {
	send_torque = 0;
	CheckFaultNR();
	DecrementHeartbeats();
	CheckHeartbeats();
	CheckFaultResettable();
	CheckStartButton();
	PEDAL_ACEL();
}
void DriveFunc(){
	send_torque = 1;
	CheckFaultNR();
	DecrementHeartbeats();
	CheckHeartbeats();
	CheckFaultResettable();
    PEDAL_ACEL(); // sends MC torque commands
    CheckStartButton();
}
void RstFaultFunc(){
	send_torque = 0;
	CheckFaultNR();
	DecrementHeartbeats();
	CheckHeartbeats();
	PulledFaultResettable() ? 0 : RunEvent(&sm, E_CLR_RST_FLT);
	PEDAL_ACEL();
}
void NoRstFaultFunc(){
	send_torque = 0;
    HAL_GPIO_WritePin(FLT_NR_CTRL_GPIO_Port, FLT_NR_CTRL_Pin, GPIO_PIN_SET); //Pull Fault NR
    send_CAN(MID_FAULT_NR, 0);
	DecrementHeartbeats();
    CheckHeartbeats();
	CheckFaultResettable();
	PEDAL_ACEL();
}
