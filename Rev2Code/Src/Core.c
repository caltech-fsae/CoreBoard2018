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
int precharge_complete = 0;
int precharging = 0;
int throttle_pressed = 1;
int heartbeat_counter[4] = {RESET_HEARTBEAT, RESET_HEARTBEAT, RESET_HEARTBEAT, RESET_HEARTBEAT}; // bms shutdown mc io
int start_button_counter = RESET_START;
int flt_r_counter = RESET_FLT_CNT;
int flt_nr_counter = RESET_FLT_CNT;
int button_pressed = 0;
int current = 0;
int first_run = 1;
int send_torque = 0;
int charge_finish_time = 0;
int init_heartbeat[4] = {0, 0, 0, 0}; // bms shutdown mc io
int ignore_nr_line = 0;
extern int ignore_nr_start_time;
int is_brake_pressed = 0;

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
		    	} else if (type == MID_PRECHARGE_STATUS) {
		    			precharge_start |= message;
			    		precharging = message;
			    		//ToggleAUXLED(1);
			    		//WriteAUXLED(2, precharging);
		    	}
		    	break;
		    case (int) BID_SHUTDOWN:
		    	if (type == MID_HEARTBEAT)
		    	{
		    		init_heartbeat[SHUTDOWN_HEARTBEAT] = 1;
		    		heartbeat_counter[SHUTDOWN_HEARTBEAT] = RESET_HEARTBEAT;
		    	} else if (type == MID_REQUEST_NR_RESET_CONSENT) {
		    		if(sm.current_state_ == ATTEMPT_RST) {
		    			can_msg_t can_msg;
		    			CAN_short_msg(&can_msg, create_ID(BID_CORE, MID_PROVIDE_NR_RESET_CONSENT), 0);
		    			CAN_queue_transmit(&can_msg);
		    		}
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
		    	} else if (type == MID_FLT_CLEAR) {
		    		can_flt_clear = 1;
		    			can_msg_t can_msg;
		    			CAN_short_msg(&can_msg, create_ID(BID_CORE, MID_FLT_CLEAR_ACK), 0);
		    			CAN_queue_transmit(&can_msg);
		    	} else if (type == MID_BPPC_BSPD)
		    	{
		    		// message 0b00: 0b0x = BSPD, NR
		    		//               0bx0 = BPPC, R
		    		if (CHECK_BIT(message, 0))
		    		{
		    		    RunEvent(&sm, E_BSPD_FLT); // NR AIRS FAULT THAT NEEDS TO BE "resettable"
		    		}
		    		else if (CHECK_BIT(message, 1))
		    		{
		    			RunEvent(&sm, E_BPPC_FLT); // R FAULT
		    		}
		    	}
		    	else if (type == MID_THROTTLE)
		    	{
		    		if (throttle_pressed && !brake_pressed && send_torque) {
		    			throttle_val = (uint16_t) (((float) message)/(0xFFF)*MAX_THROTTLE_VAL);
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
		    		   is_brake_pressed = 1;
		    		   RunEvent(&sm, E_PEDAL_BRAKE_PUSHED);
		    		}
		    		else {
		    		   is_brake_pressed = 0;
		    		   RunEvent(&sm, E_PEDAL_BRAKE_RELEASED);
		    		}
		    	}
		    	break;
		    case (int) BID_CORE:
		    	break;
		}

	}
}

void CheckHeartbeats() {
    if (heartbeat_counter[BMS_HEARTBEAT] <= 0 || heartbeat_counter[SHUTDOWN_HEARTBEAT] <= 0 || /* heartbeat_counter[MC_HEARTBEAT] <= 0 || */ heartbeat_counter[IO_HEARTBEAT] <= 0)
    {
    	RunEvent(&sm, E_HEARTBEATS_FLT);
    	// create new state which is transitioned to on E_HEARTBEATS_FLT which still keeps flt nr line low
    	//
    }
}

void DecrementHeartbeats() {
	// decrement all elements of heart beat array
		heartbeat_counter[BMS_HEARTBEAT]--;
		heartbeat_counter[SHUTDOWN_HEARTBEAT]--;
		//heartbeat_counter[MC_HEARTBEAT]--;
		heartbeat_counter[IO_HEARTBEAT]--;
}

void CheckFaultNR() {
		if(!HAL_GPIO_ReadPin(FLT_NR_GPIO_Port, FLT_NR_Pin))
		{
			if (flt_nr_counter > 0) {
				flt_nr_counter--;
			}
			else {
				flt_nr_counter = RESET_FLT_CNT;
				RunEvent(&sm, E_NO_RST_FLT);
			}
		} else {
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

	current = HAL_GPIO_ReadPin(START_GPIO_Port, START_Pin);

    if (current != button_pressed) // if the current button state does not match the stored state, start debounce
    {
    	if (start_button_counter > 0) {
    	   start_button_counter--; // debouncing
    	}
    	else { // state was different for at least RESET_START calls to this function
    		if (current == 1 && is_brake_pressed == 1) { // button was pressed, need to run start event
    		   RunEvent(&sm, E_START);
    		   button_pressed = 1; // button was pressed, must be released for at least the debounce time
    		} else {
    			button_pressed = 0; // button was released for the debounce time, a new button press can now be registered
    		}
    		start_button_counter = RESET_START;
    	}
    }
	else
	{
		start_button_counter = RESET_START; // so that random noise doesn't eventually decrement the counter down
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

void send_stop_drive() {
	precharge_start = 0;
	//
	if(precharging) {
		can_msg_t can_msg;
		CAN_short_msg(&can_msg, create_ID(BID_CORE, MID_END_DRIVE), 0);
		CAN_queue_transmit(&can_msg);
	} else {
		//WriteAUXLED(3,0);
	}
}

void CheckPrecharging() {
	if(precharge_start && !precharging && !charge_finish_time) {
		charge_finish_time = HAL_GetTick();
	}
	if(precharge_start && !precharging && charge_finish_time && HAL_GetTick() - charge_finish_time > 2000) {
		RunEvent(&sm, E_PRECHARGE_FINISHED);
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
	ToggleAUXLED(3);
	send_torque = 0;
	if(HAL_GetTick() > 1000) {
		ToggleAUXLED(1);
		CheckFaultNR();
	}
	if(HAL_GetTick() > 10000) {
		WriteAUXLED(2, 1);
		RunEvent(&sm, E_NO_RST_FLT);
	}

	init_heartbeat[BMS_HEARTBEAT] ? heartbeat_counter[BMS_HEARTBEAT]-- : 0;
	init_heartbeat[SHUTDOWN_HEARTBEAT] ? heartbeat_counter[SHUTDOWN_HEARTBEAT]--  : 0;
	//init_heartbeat[MC_HEARTBEAT] ? heartbeat_counter[MC_HEARTBEAT]-- : 0;
	init_heartbeat[IO_HEARTBEAT] ? heartbeat_counter[IO_HEARTBEAT]-- : 0;
	//TODO(@bgberr): Should check for heartbeats of live boards even during this startup
	if  (init_heartbeat[BMS_HEARTBEAT] == 0 || init_heartbeat[SHUTDOWN_HEARTBEAT] == 0 || /*init_heartbeat[MC_HEARTBEAT] == 0 ||*/ init_heartbeat[IO_HEARTBEAT] == 0)
	{
	}
	else
	{
		//WriteAUXLED(3, 1);
		RunEvent(&sm, E_BOARDS_LIVE);
	}
	CheckHeartbeats();
	CheckFaultResettable();
}

void WaitDriverFunc() {
	charge_finish_time = 0;
	send_torque = 0;
	CheckFaultNR();
	DecrementHeartbeats();
	CheckHeartbeats();
	CheckFaultResettable();
	send_stop_drive();
	PEDAL_ACEL();
}
void StartBrakeFunc() {
	send_torque = 0;
	CheckFaultNR();
	DecrementHeartbeats();
	CheckHeartbeats();
	CheckFaultResettable();
	CheckStartButton();
	send_stop_drive();
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
void RstFaultFunc() {
	charge_finish_time = 0;
	send_torque = 0;
	CheckFaultNR();
	DecrementHeartbeats();
	CheckHeartbeats();
	!PulledFaultResettable() && can_flt_clear ? RunEvent(&sm, E_CLR_RST_FLT) : 0;
	PEDAL_ACEL();
	send_stop_drive();
}

void NoRstFaultFunc(){
	WriteAUXLED(0, 1);
	CheckStartButton();
	send_torque = 0;
	HAL_GPIO_WritePin(FLT_NR_CTRL_GPIO_Port, FLT_NR_CTRL_Pin, GPIO_PIN_SET); //Pull Fault NR
    //send_CAN(MID_FAULT_NR, 0);
	DecrementHeartbeats();
    CheckHeartbeats();
	send_stop_drive();
	CheckFaultResettable();
	PEDAL_ACEL(); // TODO UMMMMMMM?

	can_msg_t can_msg;
	CAN_short_msg(&can_msg, create_ID(BID_CORE, MID_FAULT_NR), 0);
	CAN_queue_transmit(&can_msg);

}

void PrechargeFunc() {
	//precharge_complete ? RunEvent(&sm, E_PRECHARGE_FINISHED) : 0;
	send_torque = 0;
	StartPrecharge();
	CheckFaultNR();
	DecrementHeartbeats();
	CheckHeartbeats();
	CheckFaultResettable();
	CheckStartButton();
	CheckPrecharging();
}

void PrechargeReadyFunc() {
	send_torque = 0;
	CheckFaultNR();
	DecrementHeartbeats();
	CheckHeartbeats();
	CheckFaultResettable();
	CheckStartButton();
}

void HeartbeatsNoRstFunc() {
	send_torque = 0;
	HAL_GPIO_WritePin(FLT_NR_CTRL_GPIO_Port, FLT_NR_CTRL_Pin, GPIO_PIN_SET); //Pull Fault NR
	//send_CAN(MID_FAULT_NR, 0);
	init_heartbeat[BMS_HEARTBEAT] ? heartbeat_counter[BMS_HEARTBEAT]-- : 0;
	init_heartbeat[SHUTDOWN_HEARTBEAT] ? heartbeat_counter[SHUTDOWN_HEARTBEAT]--  : 0;
	//init_heartbeat[MC_HEARTBEAT] ? heartbeat_counter[MC_HEARTBEAT]-- : 0;
	init_heartbeat[IO_HEARTBEAT] ? heartbeat_counter[IO_HEARTBEAT]-- : 0;
	if  (init_heartbeat[BMS_HEARTBEAT] == 0 || init_heartbeat[SHUTDOWN_HEARTBEAT] == 0 || /*init_heartbeat[MC_HEARTBEAT] == 0 ||*/ init_heartbeat[IO_HEARTBEAT] == 0)
	{
	}
	else
	{
		RunEvent(&sm, E_BOARDS_LIVE);
	}

			    			can_msg_t can_msg;
			    			CAN_short_msg(&can_msg, create_ID(BID_CORE, MID_FAULT_NR), 0);
			    			CAN_queue_transmit(&can_msg);

}

void AttemptRstFunc() {
	WriteAUXLED(3, 1);
	send_torque = 0;
	HAL_GPIO_WritePin(FLT_NR_CTRL_GPIO_Port, FLT_NR_CTRL_Pin, GPIO_PIN_RESET);
	if (HAL_GetTick() - ignore_nr_start_time >= STOP_IGNORING_NR_LINE) {
		ignore_nr_line = 0;
	}

	if (!ignore_nr_line) {
		WriteAUXLED(3, 0);
		for(int i = 150; i > 0; i--){
			CheckFaultNR();
	    }
		RunEvent(&sm, E_CLR_NO_RST_FLT);
	}

	attempt_driver_reset();
	DecrementHeartbeats();
    CheckHeartbeats();
	send_stop_drive();
	CheckFaultResettable();
	PEDAL_ACEL();
}
