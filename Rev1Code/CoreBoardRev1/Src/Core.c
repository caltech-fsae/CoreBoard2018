/*
 * Core.c
 *
 *  Created on: Mar 11, 2018
 *      Author: shwet
 */

#include "Core.h"

extern statemachine sm;
uint16_t throttle_val = 0;
uint16_t brake_val = 0;
int init_heartbeat[4] = {0, 0, 0, 0}; // bms shutdown mc io
int heartbeat_counter[4] = {RESET_HEARTBEAT, RESET_HEARTBEAT, RESET_HEARTBEAT, RESET_HEARTBEAT}; // bms shutdown mc io
int start_button_counter = RESET_START;
int button_pressed = 0;
int first_run = 1;

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
		    	if (type == MID_HEARTBEAT && ((int) sm.current_state) == WAIT_HEARTBEATS)
		    	{
		    		init_heartbeat[0] = 1;
		    	}
		    	else if (type == MID_HEARTBEAT)
		    	{
		    		heartbeat_counter[0] = RESET_HEARTBEAT;
		    	}
		    	break;
		    case (int) BID_SHUTDOWN:
		    	if (type == MID_HEARTBEAT)
		    	{
		    		init_heartbeat[1] = 1;
		    	}
		    	else if (type == MID_HEARTBEAT)
		    	{
		    		heartbeat_counter[1] = RESET_HEARTBEAT;
		    	}
		    	else if (type == MID_FAULT_STATUS)
		    	{
		    		if (CHECK_BIT(message, 6)) // battery fault
		    		{
		    			run_event(&sm, E_NO_RST_FLT);
		    		}
		    		else if (CHECK_BIT(message, 5)) // interlock fault
		    		{
		    			run_event(&sm, E_NO_RST_FLT);
		    		}
		    		else if (CHECK_BIT(message, 3)) // generic nonresettable fault
		    		{
		    			run_event(&sm, E_NO_RST_FLT);
		    		}
		    		else if (CHECK_BIT(message, 1)) // AMS fault
		    		{
		    			run_event(&sm, E_AMS_FLT);
		    		}
		    		else if (CHECK_BIT(message, 2)) // IMD fault
		    		{
		    			run_event(&sm, E_IMD_FLT);
		    		}
		    		else if (CHECK_BIT(message, 0)) // BSPD fault
		    		{
		    			run_event(&sm, E_BSPD_FLT);
		    		}
		    		else if (CHECK_BIT(message, 4)) // generic resettable fault
		    		{
		    			run_event(&sm, E_RST_FLT);
		    		}
		    	}
		    	break;
		    case (int) BID_MOTOR_CONTROLLER:
		    	if (type == MID_HEARTBEAT)
		    	{
		    		init_heartbeat[2] = 1;
		    	}
		    	else if (type == MID_HEARTBEAT)
		    	{
		    		heartbeat_counter[2] = RESET_HEARTBEAT;
		    	}
		    	else if (type == MID_BATTERY_CURRENT && message >= MAX_CURRENT)
		    	{
		    		run_event(&sm, E_PWR_80);
		    	}
		    	break;
		    case (int) BID_IO:
		    	if (type == MID_HEARTBEAT)
		    	{
		    		init_heartbeat[3] = 1;
		    	}
		    	else if (type == MID_HEARTBEAT)
		    	{
		    		heartbeat_counter[3] = RESET_HEARTBEAT;
		    	}
		    	else if (type == MID_BPPC_BSPD)
		    	{
		    		// message 0b00: 0b0x = BSPD, NR
		    		//               0bx0 = BPPC, R
		    		if (CHECK_BIT(message, 0))
		    		{
		    		    run_event(&sm, E_BSPD_FLT); // NR FAULT
		    		}
		    		else if (CHECK_BIT(message, 1))
		    		{
		    			run_event(&sm, E_BPPC_FLT); // R FAULT
		    		}
		    	}
		    	else if (type == MID_THROTTLE)
		    	{
		    		throttle_val = (uint16_t) (((float) message)/(0xFFFF)*MAX_THROTTLE_VAL);
		    		if (brake_val < PRESSED && throttle_val > PRESSED)
		    		{
		    			run_event(&sm, E_PEDAL_ACEL);
		    		}
		    	}
		    	else if (type == MID_BRAKE)
		    	{
		    		brake_val = (uint16_t) (((float) message)/(0xFFFF)*MAX_THROTTLE_VAL);
		    		if (brake_val > PRESSED)
		    		{
		    			throttle_val = 0;
		    			run_event(&sm, E_PEDAL_BRAKE_PUSHED);
		    		}
		    		else
		    		{
		    			run_event(&sm, E_PEDAL_BRAKE_RELEASED);
		    		}
		    	}
		    	else if ((type == MID_BRAKE_STATUS) && ((message & 1) == 1))
		    	{
		    		run_event(&sm, E_PEDAL_BRAKE_PUSHED);
		    	}
		    	break;
		    case (int) BID_CORE:
		    	break;
		}

	}
}

void mainloop() // this is in the scheduler along with get_CAN, runs every 100 cycles
{


	if  (/*init_heartbeat[0] == 0 ||*/ init_heartbeat[1] == 0 || /* init_heartbeat[2] == 0 || */ init_heartbeat[3] == 0)
	{
		return;
	}
	else
	{
		run_event(&sm, E_BOARDS_LIVE);
	}
	// decrement all elements of heart beat array
	// heartbeat_counter[0]--;
	heartbeat_counter[1]--;
	//heartbeat_counter[2]--;
	heartbeat_counter[3]--;

	if ((int) sm.current_state == NO_RST_FAULT)
	{
         send_CAN(MID_FAULT_NR, 0);
	}
	else if ((int) sm.current_state == RST_FAULT)
	{
		 send_CAN(MID_FAULT, 0);
	}


    if (/*heartbeat_counter[0] <= 0 ||*/ heartbeat_counter[1] <= 0 || /* heartbeat_counter[2] <= 0 || */ heartbeat_counter[3] <= 0)
    {
    	run_event(&sm, E_NO_RST_FLT);
    }
    // comment back in once running on new board with hardware support for reading these pins
   /* else if (HAL_GPIO_ReadPin(FLT_NR_GPIO_Port, FLT_NR_Pin))
	{
		run_event(&sm, E_NO_RST_FLT);
		return;
	}
	else if (HAL_GPIO_ReadPin(FLT_GPIO_Port, FLT_Pin))
	{
        run_event(&sm, E_RST_FLT);
        return;
	} */
    if (HAL_GPIO_ReadPin(START_GPIO_Port, START_Pin) && start_button_counter > 0)
    {
    	start_button_counter--;
    }
	else if (HAL_GPIO_ReadPin(START_GPIO_Port, START_Pin) && start_button_counter <= 0 && !button_pressed)
	{
		run_event(&sm, E_START);
		button_pressed = 1; // button was pressed, must be released to register next press
		start_button_counter = RESET_START;
	}
	else
	{
		start_button_counter = RESET_START;
		button_pressed = 0;
	}

    PEDAL_ACEL(); // sends MC torque commands
}

void send_heartbeat()
{
	can_msg_t can_msg;
	CAN_short_msg(&can_msg, create_ID(BID_CORE, MID_HEARTBEAT), 0);
	CAN_queue_transmit(&can_msg);
	if (first_run)
	{
	    first_run = 0;
	    RESET_FAULTS();
	}
}

void send_state()
{
	can_msg_t can_msg;
	CAN_short_msg(&can_msg, create_ID(BID_CORE, MID_CAR_STATE), (int) sm.current_state);
	CAN_queue_transmit(&can_msg);
}
