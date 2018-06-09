/*
 * init_sm.h
 *
 *  Created on: Mar 11, 2018
 *      Author: shwet
 */

#ifndef INIT_SM_H_
#define INIT_SM_H_

#include "sm_functions.h"
#include "Core.h"
#include "sm2.h"

#define NUM_EVENTS 18
#define NUM_STATES 9

int WAIT_HEARTBEATS;
int WAIT_DRIVER;
int START_BRAKE;
int DRIVE;
int RST_FAULT;
int NO_RST_FAULT;
int PRECHARGE;
int PRECHARGE_READY;
int HEARTBEATS_NO_RST;
int ATTEMPT_RST;

int E_START,                // start button gpio active
E_PEDAL_ACEL,           // acel pedal value changed enough for us to care about
E_PEDAL_BRAKE_PUSHED,   // brake pedal pushed beyond threshold to turn on brake light
E_PEDAL_BRAKE_RELEASED, // brake pedal not beyond threshold to turn on brake light
E_PWR_80,               // power more than 80 kW
E_RST_FLT,
E_BPPC_FLT,             // BPPC asserted resettable fault
E_IMD_FLT,           // IMD asserted non-resettable fault
E_BSPD_FLT,          // BSPD asserted non-resettable fault
E_APPS_FLT,          // APPS asserted non-resettable fault
E_BSE_FLT,           // BSE asserted non-resettable fault
E_AMS_FLT,            // BMS asserted non-resettable fault
E_NO_RST_FLT,
E_BOARDS_LIVE,        // all boards sent a heartbeat
E_CLR_RST_FLT,       // resettable fault cleared, can go back to previous state
E_PRECHARGE_FINISHED,	//precharge finished go to drive
E_HEARTBEATS_FLT, // heartbeats timed out, need to pull NR line in a resettable fashion lol why do we not have three types of faults :(
E_CLR_NO_RST_FLT;     // shutdown saying all NR faults have cleared, leave NR state if true

void initialize_state_machine(StateMachine *sm);

#endif /* INIT_SM_H_ */
