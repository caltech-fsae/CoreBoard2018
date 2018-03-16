/*
 * init_sm.c
 *
 *  Created on: Mar 11, 2018
 *      Author: shwet
 */

#include "init_sm.h"

void initialize_state_machine(statemachine sm)
{
	    make_state_machine(&sm, NUM_STATES, NUM_EVENTS, WAIT_HEARTBEATS);

	    // machine, state, event, next_state, function

	    //STATE: WAIT_HEARTBEATS
	    add_tuple(&sm, WAIT_HEARTBEATS, E_START,                WAIT_HEARTBEATS,  &do_nothing);
	    add_tuple(&sm, WAIT_HEARTBEATS, E_PEDAL_ACEL,           WAIT_HEARTBEATS,  &do_nothing);
	    add_tuple(&sm, WAIT_HEARTBEATS, E_PEDAL_BRAKE_RELEASED, WAIT_HEARTBEATS,  &do_nothing);
	    add_tuple(&sm, WAIT_HEARTBEATS, E_PEDAL_BRAKE_PUSHED,   WAIT_HEARTBEATS,  &do_nothing);
	    add_tuple(&sm, WAIT_HEARTBEATS, E_PWR_80,               WAIT_HEARTBEATS,  &do_nothing);
	    add_tuple(&sm, WAIT_HEARTBEATS, E_RST_FLT,              RST_FAULT,    &do_nothing);
	    add_tuple(&sm, WAIT_HEARTBEATS, E_BPPC_FLT,             RST_FAULT,    &do_nothing);
	    add_tuple(&sm, WAIT_HEARTBEATS, E_NO_RST_FLT,           NO_RST_FAULT, &do_nothing);
	    add_tuple(&sm, WAIT_HEARTBEATS, E_IMD_FLT,              NO_RST_FAULT, &do_nothing);
	    add_tuple(&sm, WAIT_HEARTBEATS, E_BSPD_FLT,             NO_RST_FAULT, &do_nothing);
	    add_tuple(&sm, WAIT_HEARTBEATS, E_APPS_FLT,             NO_RST_FAULT, &do_nothing);
	    add_tuple(&sm, WAIT_HEARTBEATS, E_BSE_FLT,              NO_RST_FAULT, &do_nothing);
	    add_tuple(&sm, WAIT_HEARTBEATS, E_AMS_FLT,              NO_RST_FAULT, &do_nothing);
	    add_tuple(&sm, WAIT_HEARTBEATS, E_BOARDS_LIVE,          WAIT_DRIVER,  &RESET_FAULTS);

	    //STATE: WAIT_DRIVER
	    add_tuple(&sm, WAIT_DRIVER, E_START,                WAIT_DRIVER,  &do_nothing);
	    add_tuple(&sm, WAIT_DRIVER, E_PEDAL_ACEL,           WAIT_DRIVER,  &do_nothing);
	    add_tuple(&sm, WAIT_DRIVER, E_PEDAL_BRAKE_RELEASED, WAIT_DRIVER,  &PEDAL_BRAKE_RELEASED);
	    add_tuple(&sm, WAIT_DRIVER, E_PEDAL_BRAKE_PUSHED,   START_BRAKE,  &PEDAL_BRAKE_PUSHED);
	    add_tuple(&sm, WAIT_DRIVER, E_PWR_80,               WAIT_DRIVER,  &PWR_80);
	    add_tuple(&sm, WAIT_DRIVER, E_RST_FLT,              RST_FAULT,    &RST);
	    add_tuple(&sm, WAIT_DRIVER, E_BPPC_FLT,             RST_FAULT,    &BPPC_RST);
	    add_tuple(&sm, WAIT_DRIVER, E_NO_RST_FLT,           NO_RST_FAULT, &NO_RST);
	    add_tuple(&sm, WAIT_DRIVER, E_IMD_FLT,              NO_RST_FAULT, &IMD_NO_RST);
	    add_tuple(&sm, WAIT_DRIVER, E_BSPD_FLT,             NO_RST_FAULT, &BSPD_NO_RST);
	    add_tuple(&sm, WAIT_DRIVER, E_APPS_FLT,             NO_RST_FAULT, &APPS_NO_RST);
	    add_tuple(&sm, WAIT_DRIVER, E_BSE_FLT,              NO_RST_FAULT, &BSE_NO_RST);
	    add_tuple(&sm, WAIT_DRIVER, E_AMS_FLT,              NO_RST_FAULT, &BMS_NO_RST);
	    add_tuple(&sm, WAIT_DRIVER, E_BOARDS_LIVE,          WAIT_DRIVER,  &do_nothing);

	    //STATE: DRIVE
	    add_tuple(&sm, DRIVE, E_START,                WAIT_DRIVER,  &END_DRIVE);
	    add_tuple(&sm, DRIVE, E_PEDAL_ACEL,           DRIVE,        &PEDAL_ACEL);
	    add_tuple(&sm, DRIVE, E_PEDAL_BRAKE_RELEASED, DRIVE,        &PEDAL_BRAKE_RELEASED);
	    add_tuple(&sm, DRIVE, E_PEDAL_BRAKE_PUSHED,   DRIVE,        &PEDAL_BRAKE_PUSHED);
	    add_tuple(&sm, DRIVE, E_PWR_80,               DRIVE,        &PWR_80);
	    add_tuple(&sm, DRIVE, E_RST_FLT,              RST_FAULT,    &RST);
	    add_tuple(&sm, DRIVE, E_BPPC_FLT,             RST_FAULT,    &BPPC_RST);
	    add_tuple(&sm, DRIVE, E_NO_RST_FLT,           NO_RST_FAULT, &NO_RST);
	    add_tuple(&sm, DRIVE, E_IMD_FLT,              NO_RST_FAULT, &IMD_NO_RST);
	    add_tuple(&sm, DRIVE, E_BSPD_FLT,             NO_RST_FAULT, &BSPD_NO_RST);
	    add_tuple(&sm, DRIVE, E_APPS_FLT,             NO_RST_FAULT, &APPS_NO_RST);
	    add_tuple(&sm, DRIVE, E_BSE_FLT,              NO_RST_FAULT, &BSE_NO_RST);
	    add_tuple(&sm, DRIVE, E_AMS_FLT,              NO_RST_FAULT, &BMS_NO_RST);
	    add_tuple(&sm, DRIVE, E_BOARDS_LIVE,          DRIVE,  &do_nothing);

	    //STATE: START_BRAKE
	    add_tuple(&sm, START_BRAKE, E_START,                DRIVE,        &RTDS);
	    add_tuple(&sm, START_BRAKE, E_PEDAL_ACEL,           WAIT_DRIVER,  &do_nothing);
	    add_tuple(&sm, START_BRAKE, E_PEDAL_BRAKE_RELEASED, WAIT_DRIVER,  &do_nothing);
	    add_tuple(&sm, START_BRAKE, E_PEDAL_BRAKE_PUSHED,   START_BRAKE,  &do_nothing);
	    add_tuple(&sm, START_BRAKE, E_PWR_80,               START_BRAKE,  &PWR_80);
	    add_tuple(&sm, START_BRAKE, E_RST_FLT,              RST_FAULT,    &RST);
	    add_tuple(&sm, START_BRAKE, E_BPPC_FLT,             RST_FAULT,    &BPPC_RST);
	    add_tuple(&sm, START_BRAKE, E_NO_RST_FLT,           NO_RST_FAULT, &NO_RST);
	    add_tuple(&sm, START_BRAKE, E_IMD_FLT,              NO_RST_FAULT, &IMD_NO_RST);
	    add_tuple(&sm, START_BRAKE, E_BSPD_FLT,             NO_RST_FAULT, &BSPD_NO_RST);
	    add_tuple(&sm, START_BRAKE, E_APPS_FLT,             NO_RST_FAULT, &APPS_NO_RST);
	    add_tuple(&sm, START_BRAKE, E_BSE_FLT,              NO_RST_FAULT, &BSE_NO_RST);
	    add_tuple(&sm, START_BRAKE, E_AMS_FLT,              NO_RST_FAULT, &BMS_NO_RST);
	    add_tuple(&sm, START_BRAKE, E_BOARDS_LIVE,          START_BRAKE,  &do_nothing);

	    //STATE: RST_FAULT
	    add_tuple(&sm, RST_FAULT, E_START,                RST_FAULT,    &do_nothing);
	    add_tuple(&sm, RST_FAULT, E_PEDAL_ACEL,           RST_FAULT,    &do_nothing);
	    add_tuple(&sm, RST_FAULT, E_PEDAL_BRAKE_RELEASED, RST_FAULT,    &PEDAL_BRAKE_RELEASED);
	    add_tuple(&sm, RST_FAULT, E_PEDAL_BRAKE_PUSHED,   WAIT_HEARTBEATS,  &PEDAL_BRAKE_PUSHED);
	    add_tuple(&sm, RST_FAULT, E_PWR_80,               RST_FAULT,    &PWR_80);
	    add_tuple(&sm, RST_FAULT, E_RST_FLT,              RST_FAULT,    &RST);
	    add_tuple(&sm, RST_FAULT, E_BPPC_FLT,             RST_FAULT,    &do_nothing);
	    add_tuple(&sm, RST_FAULT, E_NO_RST_FLT,           NO_RST_FAULT, &NO_RST);
	    add_tuple(&sm, RST_FAULT, E_IMD_FLT,              NO_RST_FAULT, &IMD_NO_RST);
	    add_tuple(&sm, RST_FAULT, E_BSPD_FLT,             NO_RST_FAULT, &BSPD_NO_RST);
	    add_tuple(&sm, RST_FAULT, E_APPS_FLT,             NO_RST_FAULT, &APPS_NO_RST);
	    add_tuple(&sm, RST_FAULT, E_BSE_FLT,              NO_RST_FAULT, &BSE_NO_RST);
	    add_tuple(&sm, RST_FAULT, E_AMS_FLT,              NO_RST_FAULT, &BMS_NO_RST);
	    add_tuple(&sm, RST_FAULT, E_BOARDS_LIVE,          RST_FAULT,    &do_nothing);


	    //STATE: NO_RST_FAULT
	    add_tuple(&sm, NO_RST_FAULT, E_START,                NO_RST_FAULT,  &do_nothing);
	    add_tuple(&sm, NO_RST_FAULT, E_PEDAL_ACEL,           NO_RST_FAULT,  &do_nothing);
	    add_tuple(&sm, NO_RST_FAULT, E_PEDAL_BRAKE_RELEASED, NO_RST_FAULT,  &PEDAL_BRAKE_RELEASED);
	    add_tuple(&sm, NO_RST_FAULT, E_PEDAL_BRAKE_PUSHED,   NO_RST_FAULT,  &PEDAL_BRAKE_PUSHED);
	    add_tuple(&sm, NO_RST_FAULT, E_PWR_80,               NO_RST_FAULT,  &PWR_80);
	    add_tuple(&sm, NO_RST_FAULT, E_RST_FLT,              NO_RST_FAULT,  &RST);
	    add_tuple(&sm, NO_RST_FAULT, E_BPPC_FLT,             NO_RST_FAULT,  &do_nothing);
	    add_tuple(&sm, NO_RST_FAULT, E_NO_RST_FLT,           NO_RST_FAULT,  &NO_RST);
	    add_tuple(&sm, NO_RST_FAULT, E_IMD_FLT,              NO_RST_FAULT,  &IMD_NO_RST);
	    add_tuple(&sm, NO_RST_FAULT, E_BSPD_FLT,             NO_RST_FAULT,  &BSPD_NO_RST);
	    add_tuple(&sm, NO_RST_FAULT, E_APPS_FLT,             NO_RST_FAULT,  &APPS_NO_RST);
	    add_tuple(&sm, NO_RST_FAULT, E_BSE_FLT,              NO_RST_FAULT,  &BSE_NO_RST);
	    add_tuple(&sm, NO_RST_FAULT, E_AMS_FLT,              NO_RST_FAULT,  &BMS_NO_RST);
	    add_tuple(&sm, NO_RST_FAULT, E_BOARDS_LIVE,          NO_RST_FAULT,  &do_nothing);

}
