/*
 * init_sm.c
 *
 *  Created on: Mar 11, 2018
 *      Author: shwet
 */

#include "init_sm.h"

void initialize_state_machine(StateMachine *sm)
{
    MakeStateMachine(sm, NUM_STATES, NUM_EVENTS);

	WAIT_HEARTBEATS = MakeState(sm, &WaitHeartbeatsFunc);
	WAIT_DRIVER = MakeState(sm, &WaitDriverFunc);
	START_BRAKE = MakeState(sm, &StartBrakeFunc);
	DRIVE = MakeState(sm, &DriveFunc);
	RST_FAULT = MakeState(sm, &RstFaultFunc);
	NO_RST_FAULT = MakeState(sm, &NoRstFaultFunc);
	PRECHARGE = MakeState(sm, &PrechargeFunc);
	PRECHARGE_READY = MakeState(sm, &PrechargeReadyFunc);
	HEARTBEATS_NO_RST = MakeState(sm, &HeartbeatsNoRstFunc);
	ATTEMPT_RST = MakeState(sm, &AttemptRstFunc);


	E_START = MakeEvent(sm);                // start button gpio active
	E_PEDAL_ACEL = MakeEvent(sm);          // acel pedal value changed enough for us to care about
	E_PEDAL_BRAKE_PUSHED = MakeEvent(sm);   // brake pedal pushed beyond threshold to turn on brake light
	E_PEDAL_BRAKE_RELEASED = MakeEvent(sm); // brake pedal not beyond threshold to turn on brake light
	E_PWR_80 = MakeEvent(sm);               // power more than 80 kW
	E_RST_FLT = MakeEvent(sm);
	E_BPPC_FLT = MakeEvent(sm);             // BPPC asserted resettable fault
	E_IMD_FLT = MakeEvent(sm);           // IMD asserted non-resettable fault
	E_BSPD_FLT = MakeEvent(sm);          // BSPD asserted non-resettable fault
	E_APPS_FLT = MakeEvent(sm);          // APPS asserted non-resettable fault
	E_BSE_FLT = MakeEvent(sm);           // BSE asserted non-resettable fault
	E_AMS_FLT = MakeEvent(sm);            // BMS asserted non-resettable fault
	E_NO_RST_FLT = MakeEvent(sm);
	E_BOARDS_LIVE = MakeEvent(sm);
	E_CLR_RST_FLT = MakeEvent(sm);
	E_PRECHARGE_FINISHED = MakeEvent(sm);
	E_HEARTBEATS_FLT = MakeEvent(sm);
	E_CLR_NO_RST_FLT = MakeEvent(sm);

	InitStateMachine(sm, WAIT_HEARTBEATS);
	    // machine, state, event, next_state, function

	    //STATE: WAIT_HEARTBEATS
	    AddEvent(sm, WAIT_HEARTBEATS, E_START,                WAIT_HEARTBEATS,  &do_nothing);
	    AddEvent(sm, WAIT_HEARTBEATS, E_PEDAL_ACEL,           WAIT_HEARTBEATS,  &do_nothing);
	    AddEvent(sm, WAIT_HEARTBEATS, E_PEDAL_BRAKE_RELEASED, WAIT_HEARTBEATS,  &PEDAL_BRAKE_RELEASED);
	    AddEvent(sm, WAIT_HEARTBEATS, E_PEDAL_BRAKE_PUSHED,   WAIT_HEARTBEATS,  &PEDAL_BRAKE_PUSHED);
	    AddEvent(sm, WAIT_HEARTBEATS, E_PWR_80,               WAIT_HEARTBEATS,  &do_nothing);
	    AddEvent(sm, WAIT_HEARTBEATS, E_RST_FLT,              WAIT_HEARTBEATS,  &do_nothing);
	    AddEvent(sm, WAIT_HEARTBEATS, E_BPPC_FLT,             WAIT_HEARTBEATS,  &do_nothing);
	    AddEvent(sm, WAIT_HEARTBEATS, E_NO_RST_FLT,           NO_RST_FAULT,  &do_nothing);
	    AddEvent(sm, WAIT_HEARTBEATS, E_IMD_FLT,              WAIT_HEARTBEATS,  &do_nothing);
	    AddEvent(sm, WAIT_HEARTBEATS, E_BSPD_FLT,             WAIT_HEARTBEATS,  &do_nothing);
	    AddEvent(sm, WAIT_HEARTBEATS, E_APPS_FLT,             WAIT_HEARTBEATS,  &do_nothing);
	    AddEvent(sm, WAIT_HEARTBEATS, E_BSE_FLT,              WAIT_HEARTBEATS,  &do_nothing);
	    AddEvent(sm, WAIT_HEARTBEATS, E_AMS_FLT,              WAIT_HEARTBEATS,  &do_nothing);
	    AddEvent(sm, WAIT_HEARTBEATS, E_BOARDS_LIVE,          WAIT_DRIVER,      &do_nothing);
	    AddEvent(sm, WAIT_HEARTBEATS, E_CLR_RST_FLT,          WAIT_HEARTBEATS,  &do_nothing);
	    AddEvent(sm, WAIT_HEARTBEATS, E_PRECHARGE_FINISHED,   WAIT_HEARTBEATS,  &do_nothing);
	    AddEvent(sm, WAIT_HEARTBEATS, E_HEARTBEATS_FLT,       WAIT_HEARTBEATS,  &do_nothing);
	    AddEvent(sm, WAIT_HEARTBEATS, E_CLR_NO_RST_FLT,       WAIT_HEARTBEATS,  &do_nothing);


	    //STATE: WAIT_DRIVER
	    AddEvent(sm, WAIT_DRIVER, E_START,                WAIT_DRIVER,  &do_nothing);
	    AddEvent(sm, WAIT_DRIVER, E_PEDAL_ACEL,           WAIT_DRIVER,  &do_nothing);
	    AddEvent(sm, WAIT_DRIVER, E_PEDAL_BRAKE_RELEASED, WAIT_DRIVER,  &PEDAL_BRAKE_RELEASED);
	    AddEvent(sm, WAIT_DRIVER, E_PEDAL_BRAKE_PUSHED,   START_BRAKE,  &PEDAL_BRAKE_PUSHED);
	    AddEvent(sm, WAIT_DRIVER, E_PWR_80,               WAIT_DRIVER,  &PWR_80);
	    AddEvent(sm, WAIT_DRIVER, E_RST_FLT,              RST_FAULT,    &send_FLT_CAN);
	    AddEvent(sm, WAIT_DRIVER, E_BPPC_FLT,             RST_FAULT,    &send_FLT_CAN);
	    AddEvent(sm, WAIT_DRIVER, E_NO_RST_FLT,           NO_RST_FAULT, &send_FLT_CAN);
	    AddEvent(sm, WAIT_DRIVER, E_IMD_FLT,              NO_RST_FAULT, &send_FLT_CAN);
	    AddEvent(sm, WAIT_DRIVER, E_BSPD_FLT,             NO_RST_FAULT, &send_FLT_CAN);
	    AddEvent(sm, WAIT_DRIVER, E_APPS_FLT,             NO_RST_FAULT, &send_FLT_CAN);
	    AddEvent(sm, WAIT_DRIVER, E_BSE_FLT,              NO_RST_FAULT, &send_FLT_CAN);
	    AddEvent(sm, WAIT_DRIVER, E_AMS_FLT,              NO_RST_FAULT, &send_FLT_CAN);
	    AddEvent(sm, WAIT_DRIVER, E_BOARDS_LIVE,          WAIT_DRIVER,  &do_nothing);
	    AddEvent(sm, WAIT_DRIVER, E_CLR_RST_FLT,          WAIT_DRIVER,  &do_nothing);
	    AddEvent(sm, WAIT_DRIVER, E_PRECHARGE_FINISHED,          WAIT_DRIVER,  &do_nothing);
	    AddEvent(sm, WAIT_DRIVER, E_HEARTBEATS_FLT,       HEARTBEATS_NO_RST,  &reset_init_heartbeats);
	    AddEvent(sm, WAIT_DRIVER, E_CLR_NO_RST_FLT,       WAIT_DRIVER,  &do_nothing);

	    //STATE: DRIVE
	    AddEvent(sm, DRIVE, E_START,                WAIT_DRIVER,  &END_DRIVE);
	    AddEvent(sm, DRIVE, E_PEDAL_ACEL,           DRIVE,        &PEDAL_ACEL);
	    AddEvent(sm, DRIVE, E_PEDAL_BRAKE_RELEASED, DRIVE,        &PEDAL_BRAKE_RELEASED);
	    AddEvent(sm, DRIVE, E_PEDAL_BRAKE_PUSHED,   DRIVE,        &PEDAL_BRAKE_PUSHED);
	    AddEvent(sm, DRIVE, E_PWR_80,               DRIVE,        &PWR_80);
	    AddEvent(sm, DRIVE, E_RST_FLT,              RST_FAULT,    &send_FLT_CAN);
	    AddEvent(sm, DRIVE, E_BPPC_FLT,             RST_FAULT,    &send_FLT_CAN);
	    AddEvent(sm, DRIVE, E_NO_RST_FLT,           NO_RST_FAULT, &send_FLT_CAN);
	    AddEvent(sm, DRIVE, E_IMD_FLT,              NO_RST_FAULT, &send_FLT_CAN);
	    AddEvent(sm, DRIVE, E_BSPD_FLT,             NO_RST_FAULT, &send_FLT_CAN);
	    AddEvent(sm, DRIVE, E_APPS_FLT,             NO_RST_FAULT, &send_FLT_CAN);
	    AddEvent(sm, DRIVE, E_BSE_FLT,              NO_RST_FAULT, &send_FLT_CAN);
	    AddEvent(sm, DRIVE, E_AMS_FLT,              NO_RST_FAULT, &send_FLT_CAN);
	    AddEvent(sm, DRIVE, E_BOARDS_LIVE,          DRIVE,  &do_nothing);
	    AddEvent(sm, DRIVE, E_CLR_RST_FLT,          DRIVE,  &do_nothing);
	    AddEvent(sm, DRIVE, E_PRECHARGE_FINISHED,          DRIVE,  &do_nothing);
	    AddEvent(sm, DRIVE, E_HEARTBEATS_FLT,       HEARTBEATS_NO_RST,  &reset_init_heartbeats);
	    AddEvent(sm, DRIVE, E_CLR_NO_RST_FLT,       DRIVE,  &do_nothing);


	    //STATE: START_BRAKE
	    AddEvent(sm, START_BRAKE, E_START,                PRECHARGE,        &StartPrecharge);
	    AddEvent(sm, START_BRAKE, E_PEDAL_ACEL,           WAIT_DRIVER,  &do_nothing);
	    AddEvent(sm, START_BRAKE, E_PEDAL_BRAKE_RELEASED, WAIT_DRIVER,  &do_nothing);
	    AddEvent(sm, START_BRAKE, E_PEDAL_BRAKE_PUSHED,   START_BRAKE,  &do_nothing);
	    AddEvent(sm, START_BRAKE, E_PWR_80,               START_BRAKE,  &PWR_80);
	    AddEvent(sm, START_BRAKE, E_RST_FLT,              RST_FAULT,    &send_FLT_CAN);
	    AddEvent(sm, START_BRAKE, E_BPPC_FLT,             RST_FAULT,    &send_FLT_CAN);
	    AddEvent(sm, START_BRAKE, E_NO_RST_FLT,           NO_RST_FAULT, &send_FLT_CAN);
	    AddEvent(sm, START_BRAKE, E_IMD_FLT,              NO_RST_FAULT, &send_FLT_CAN);
	    AddEvent(sm, START_BRAKE, E_BSPD_FLT,             NO_RST_FAULT, &send_FLT_CAN);
	    AddEvent(sm, START_BRAKE, E_APPS_FLT,             NO_RST_FAULT, &send_FLT_CAN);
	    AddEvent(sm, START_BRAKE, E_BSE_FLT,              NO_RST_FAULT, &send_FLT_CAN);
	    AddEvent(sm, START_BRAKE, E_AMS_FLT,              NO_RST_FAULT, &send_FLT_CAN);
	    AddEvent(sm, START_BRAKE, E_BOARDS_LIVE,          START_BRAKE,  &do_nothing);
	    AddEvent(sm, START_BRAKE, E_CLR_RST_FLT,          START_BRAKE,  &do_nothing);
	    AddEvent(sm, START_BRAKE, E_PRECHARGE_FINISHED,   START_BRAKE,  &do_nothing);
	    AddEvent(sm, START_BRAKE, E_HEARTBEATS_FLT,       HEARTBEATS_NO_RST,  &reset_init_heartbeats);
	    AddEvent(sm, START_BRAKE, E_CLR_NO_RST_FLT,       START_BRAKE,  &do_nothing);


	    //STATE: PRECHARGE
	    AddEvent(sm, PRECHARGE, E_START,                WAIT_DRIVER,  &do_nothing);
	    AddEvent(sm, PRECHARGE, E_PEDAL_ACEL,           PRECHARGE,  &do_nothing);
	    AddEvent(sm, PRECHARGE, E_PEDAL_BRAKE_RELEASED, PRECHARGE,  &do_nothing);
	    AddEvent(sm, PRECHARGE, E_PEDAL_BRAKE_PUSHED,   PRECHARGE,  &do_nothing);
	    AddEvent(sm, PRECHARGE, E_PWR_80,               PRECHARGE,  &do_nothing);
	    AddEvent(sm, PRECHARGE, E_RST_FLT,              RST_FAULT,    &reset_precharge_timer);  //SEND STOP DRIVE
	    AddEvent(sm, PRECHARGE, E_BPPC_FLT,             PRECHARGE,    &do_nothing);
	    AddEvent(sm, PRECHARGE, E_NO_RST_FLT,           NO_RST_FAULT, &reset_precharge_timer); //SEND STOP DRIVE
	    AddEvent(sm, PRECHARGE, E_IMD_FLT,              PRECHARGE, &do_nothing);
	    AddEvent(sm, PRECHARGE, E_BSPD_FLT,             PRECHARGE, &do_nothing);
	    AddEvent(sm, PRECHARGE, E_APPS_FLT,             NO_RST_FAULT, &reset_precharge_timer); //SEND STOP DRIVE
	    AddEvent(sm, PRECHARGE, E_BSE_FLT,              NO_RST_FAULT, &reset_precharge_timer); //SEND STOP DRIVE
	    AddEvent(sm, PRECHARGE, E_AMS_FLT,              NO_RST_FAULT, &reset_precharge_timer); //SEND STOP DRIVE
	    AddEvent(sm, PRECHARGE, E_BOARDS_LIVE,          PRECHARGE,  &do_nothing);
	    AddEvent(sm, PRECHARGE, E_CLR_RST_FLT,          PRECHARGE,  &do_nothing);
	    AddEvent(sm, PRECHARGE, E_PRECHARGE_FINISHED,   PRECHARGE_READY,  &SetFinishPrecharge);
	    AddEvent(sm, PRECHARGE, E_HEARTBEATS_FLT,       HEARTBEATS_NO_RST,  &reset_precharge_timer);
	    AddEvent(sm, PRECHARGE, E_CLR_NO_RST_FLT,       PRECHARGE,  &do_nothing);

	    //STATE: PRECHARGE_READY
	    	    AddEvent(sm, PRECHARGE_READY, E_START,                DRIVE,  &RTDS);
	    	    AddEvent(sm, PRECHARGE_READY, E_PEDAL_ACEL,           PRECHARGE_READY,  &do_nothing);
	    	    AddEvent(sm, PRECHARGE_READY, E_PEDAL_BRAKE_RELEASED, PRECHARGE_READY,  &do_nothing);
	    	    AddEvent(sm, PRECHARGE_READY, E_PEDAL_BRAKE_PUSHED,   PRECHARGE_READY,  &do_nothing);
	    	    AddEvent(sm, PRECHARGE_READY, E_PWR_80,               PRECHARGE_READY,  &do_nothing);
	    	    AddEvent(sm, PRECHARGE_READY, E_RST_FLT,              RST_FAULT,    &do_nothing);  //SEND STOP DRIVE
	    	    AddEvent(sm, PRECHARGE_READY, E_BPPC_FLT,             PRECHARGE_READY,    &do_nothing);
	    	    AddEvent(sm, PRECHARGE_READY, E_NO_RST_FLT,           NO_RST_FAULT, &do_nothing); //SEND STOP DRIVE
	    	    AddEvent(sm, PRECHARGE_READY, E_IMD_FLT,              PRECHARGE_READY, &do_nothing);
	    	    AddEvent(sm, PRECHARGE_READY, E_BSPD_FLT,             PRECHARGE_READY, &do_nothing);
	    	    AddEvent(sm, PRECHARGE_READY, E_APPS_FLT,             NO_RST_FAULT, &do_nothing); //SEND STOP DRIVE
	    	    AddEvent(sm, PRECHARGE_READY, E_BSE_FLT,              NO_RST_FAULT, &do_nothing); //SEND STOP DRIVE
	    	    AddEvent(sm, PRECHARGE_READY, E_AMS_FLT,              NO_RST_FAULT, &do_nothing); //SEND STOP DRIVE
	    	    AddEvent(sm, PRECHARGE_READY, E_BOARDS_LIVE,          PRECHARGE_READY,  &do_nothing);
	    	    AddEvent(sm, PRECHARGE_READY, E_CLR_RST_FLT,          PRECHARGE_READY,  &do_nothing);
	    	    AddEvent(sm, PRECHARGE_READY, E_PRECHARGE_FINISHED,   PRECHARGE_READY,  &do_nothing);
	    	    AddEvent(sm, PRECHARGE_READY, E_HEARTBEATS_FLT,       HEARTBEATS_NO_RST,  &reset_init_heartbeats);
	    	    AddEvent(sm, PRECHARGE_READY, E_CLR_NO_RST_FLT,       PRECHARGE_READY,  &do_nothing);


	    //STATE: RST_FAULT
	    AddEvent(sm, RST_FAULT, E_START,                RST_FAULT,    &do_nothing);
	    AddEvent(sm, RST_FAULT, E_PEDAL_ACEL,           RST_FAULT,    &do_nothing);
	    AddEvent(sm, RST_FAULT, E_PEDAL_BRAKE_RELEASED, RST_FAULT,    &PEDAL_BRAKE_RELEASED);
	    AddEvent(sm, RST_FAULT, E_PEDAL_BRAKE_PUSHED,   RST_FAULT,    &PEDAL_BRAKE_PUSHED);
	    AddEvent(sm, RST_FAULT, E_PWR_80,               RST_FAULT,    &PWR_80);
	    AddEvent(sm, RST_FAULT, E_RST_FLT,              RST_FAULT,    &send_FLT_CAN);
	    AddEvent(sm, RST_FAULT, E_BPPC_FLT,             RST_FAULT,    &do_nothing);
	    AddEvent(sm, RST_FAULT, E_NO_RST_FLT,           NO_RST_FAULT, &send_FLT_CAN);
	    AddEvent(sm, RST_FAULT, E_IMD_FLT,              NO_RST_FAULT, &send_FLT_CAN);
	    AddEvent(sm, RST_FAULT, E_BSPD_FLT,             NO_RST_FAULT, &send_FLT_CAN);
	    AddEvent(sm, RST_FAULT, E_APPS_FLT,             NO_RST_FAULT, &send_FLT_CAN);
	    AddEvent(sm, RST_FAULT, E_BSE_FLT,              NO_RST_FAULT, &send_FLT_CAN);
	    AddEvent(sm, RST_FAULT, E_AMS_FLT,              NO_RST_FAULT, &send_FLT_CAN);
	    AddEvent(sm, RST_FAULT, E_BOARDS_LIVE,          RST_FAULT,    &do_nothing);
	    AddEvent(sm, RST_FAULT, E_CLR_RST_FLT,          WAIT_DRIVER,  &ExitFault);
	    AddEvent(sm, RST_FAULT, E_PRECHARGE_FINISHED,   RST_FAULT,    &do_nothing);
	    AddEvent(sm, RST_FAULT, E_HEARTBEATS_FLT,       HEARTBEATS_NO_RST,  &reset_init_heartbeats);
	    AddEvent(sm, RST_FAULT, E_CLR_NO_RST_FLT,       RST_FAULT,  &do_nothing);


	    //STATE: NO_RST_FAULT
	    AddEvent(sm, NO_RST_FAULT, E_START,                ATTEMPT_RST,   &set_ignore_nr_line);
	    AddEvent(sm, NO_RST_FAULT, E_PEDAL_ACEL,           NO_RST_FAULT,  &do_nothing);
	    AddEvent(sm, NO_RST_FAULT, E_PEDAL_BRAKE_RELEASED, NO_RST_FAULT,  &PEDAL_BRAKE_RELEASED);
	    AddEvent(sm, NO_RST_FAULT, E_PEDAL_BRAKE_PUSHED,   NO_RST_FAULT,  &PEDAL_BRAKE_PUSHED);
	    AddEvent(sm, NO_RST_FAULT, E_PWR_80,               NO_RST_FAULT,  &PWR_80);
	    AddEvent(sm, NO_RST_FAULT, E_RST_FLT,              NO_RST_FAULT,  &send_FLT_CAN);
	    AddEvent(sm, NO_RST_FAULT, E_BPPC_FLT,             NO_RST_FAULT,  &do_nothing);
	    AddEvent(sm, NO_RST_FAULT, E_NO_RST_FLT,           NO_RST_FAULT,  &send_FLT_CAN);
	    AddEvent(sm, NO_RST_FAULT, E_IMD_FLT,              NO_RST_FAULT,  &send_FLT_CAN);
	    AddEvent(sm, NO_RST_FAULT, E_BSPD_FLT,             NO_RST_FAULT,  &send_FLT_CAN);
	    AddEvent(sm, NO_RST_FAULT, E_APPS_FLT,             NO_RST_FAULT,  &send_FLT_CAN);
	    AddEvent(sm, NO_RST_FAULT, E_BSE_FLT,              NO_RST_FAULT,  &send_FLT_CAN);
	    AddEvent(sm, NO_RST_FAULT, E_AMS_FLT,              NO_RST_FAULT,  &send_FLT_CAN);
	    AddEvent(sm, NO_RST_FAULT, E_BOARDS_LIVE,          NO_RST_FAULT,  &do_nothing);
	    AddEvent(sm, NO_RST_FAULT, E_CLR_RST_FLT,          NO_RST_FAULT,  &do_nothing);
	    AddEvent(sm, NO_RST_FAULT, E_PRECHARGE_FINISHED,   NO_RST_FAULT,  &do_nothing);
	    AddEvent(sm, NO_RST_FAULT, E_HEARTBEATS_FLT,       HEARTBEATS_NO_RST,  &reset_init_heartbeats);
	    AddEvent(sm, NO_RST_FAULT, E_CLR_NO_RST_FLT,       NO_RST_FAULT,  &do_nothing);

	    //STATE: HEARTBEATS_NO_RST - is resettable, but needs to pull NR line bc needs to open airs
	    AddEvent(sm, HEARTBEATS_NO_RST, E_START,                HEARTBEATS_NO_RST,  &do_nothing);
	    AddEvent(sm, HEARTBEATS_NO_RST, E_PEDAL_ACEL,           HEARTBEATS_NO_RST,  &do_nothing);
	    AddEvent(sm, HEARTBEATS_NO_RST, E_PEDAL_BRAKE_RELEASED, HEARTBEATS_NO_RST,  &PEDAL_BRAKE_RELEASED);
	    AddEvent(sm, HEARTBEATS_NO_RST, E_PEDAL_BRAKE_PUSHED,   HEARTBEATS_NO_RST,  &PEDAL_BRAKE_PUSHED);
	    AddEvent(sm, HEARTBEATS_NO_RST, E_PWR_80,               HEARTBEATS_NO_RST,  &PWR_80);
	    AddEvent(sm, HEARTBEATS_NO_RST, E_RST_FLT,              HEARTBEATS_NO_RST,  &send_FLT_CAN);
	    AddEvent(sm, HEARTBEATS_NO_RST, E_BPPC_FLT,             HEARTBEATS_NO_RST,  &do_nothing);
	    AddEvent(sm, HEARTBEATS_NO_RST, E_NO_RST_FLT,           HEARTBEATS_NO_RST,  &send_FLT_CAN);
	    AddEvent(sm, HEARTBEATS_NO_RST, E_IMD_FLT,              HEARTBEATS_NO_RST,  &send_FLT_CAN);
	    AddEvent(sm, HEARTBEATS_NO_RST, E_BSPD_FLT,             HEARTBEATS_NO_RST,  &send_FLT_CAN);
	    AddEvent(sm, HEARTBEATS_NO_RST, E_APPS_FLT,             HEARTBEATS_NO_RST,  &send_FLT_CAN);
	    AddEvent(sm, HEARTBEATS_NO_RST, E_BSE_FLT,              HEARTBEATS_NO_RST,  &send_FLT_CAN);
	    AddEvent(sm, HEARTBEATS_NO_RST, E_AMS_FLT,              HEARTBEATS_NO_RST,  &send_FLT_CAN);
	    AddEvent(sm, HEARTBEATS_NO_RST, E_BOARDS_LIVE,          PRECHARGE_READY,    &do_nothing); // ????
	    AddEvent(sm, HEARTBEATS_NO_RST, E_CLR_RST_FLT,          HEARTBEATS_NO_RST,  &do_nothing);
	    AddEvent(sm, HEARTBEATS_NO_RST, E_PRECHARGE_FINISHED,   HEARTBEATS_NO_RST,  &do_nothing);
	    AddEvent(sm, HEARTBEATS_NO_RST, E_HEARTBEATS_FLT,       HEARTBEATS_NO_RST,  &do_nothing);
	    AddEvent(sm, HEARTBEATS_NO_RST, E_CLR_NO_RST_FLT,       HEARTBEATS_NO_RST,  &do_nothing);

	    //STATE: ATTEMPT_RST - driver presses start button in fault nr state, so attempts reset for 3 s
	    AddEvent(sm, ATTEMPT_RST, E_START,                ATTEMPT_RST,  &do_nothing);
	    AddEvent(sm, ATTEMPT_RST, E_PEDAL_ACEL,           ATTEMPT_RST,  &do_nothing);
	    AddEvent(sm, ATTEMPT_RST, E_PEDAL_BRAKE_RELEASED, ATTEMPT_RST,  &PEDAL_BRAKE_RELEASED);
	    AddEvent(sm, ATTEMPT_RST, E_PEDAL_BRAKE_PUSHED,   ATTEMPT_RST,  &PEDAL_BRAKE_PUSHED);
	    AddEvent(sm, ATTEMPT_RST, E_PWR_80,               ATTEMPT_RST,  &PWR_80);
	    AddEvent(sm, ATTEMPT_RST, E_RST_FLT,              ATTEMPT_RST,  &send_FLT_CAN);
	    AddEvent(sm, ATTEMPT_RST, E_BPPC_FLT,             ATTEMPT_RST,  &do_nothing);
	    AddEvent(sm, ATTEMPT_RST, E_NO_RST_FLT,           NO_RST_FAULT, &send_FLT_CAN);
	    AddEvent(sm, ATTEMPT_RST, E_IMD_FLT,              ATTEMPT_RST,  &send_FLT_CAN);
	    AddEvent(sm, ATTEMPT_RST, E_BSPD_FLT,             ATTEMPT_RST,  &send_FLT_CAN);
	    AddEvent(sm, ATTEMPT_RST, E_APPS_FLT,             ATTEMPT_RST,  &send_FLT_CAN);
	    AddEvent(sm, ATTEMPT_RST, E_BSE_FLT,              ATTEMPT_RST,  &send_FLT_CAN);
	    AddEvent(sm, ATTEMPT_RST, E_AMS_FLT,              ATTEMPT_RST,  &send_FLT_CAN);
	    AddEvent(sm, ATTEMPT_RST, E_BOARDS_LIVE,          ATTEMPT_RST,  &do_nothing); // ????
	    AddEvent(sm, ATTEMPT_RST, E_CLR_RST_FLT,          ATTEMPT_RST,  &do_nothing);
	    AddEvent(sm, ATTEMPT_RST, E_PRECHARGE_FINISHED,   ATTEMPT_RST,  &do_nothing);
	    AddEvent(sm, ATTEMPT_RST, E_HEARTBEATS_FLT,       ATTEMPT_RST,  &do_nothing);
	    AddEvent(sm, ATTEMPT_RST, E_CLR_NO_RST_FLT,       PRECHARGE_READY,  &reset_ignore_nr_line);
}
