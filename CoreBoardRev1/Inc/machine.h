/*
 * machine.h
 *
 *  Created on: Mar 6, 2018
 *      Author: shwet
 */

#ifndef MACHINE_H_
#define MACHINE_H_

#include <stdlib.h>

#define NUM_STATES 5
#define NUM_EVENTS 13

typedef enum
{
    WAIT,           // waiting for driver start (brake + start button)
	DRIVE,          // normal driving state
    START_BRAKE,    // waiting for driver start (start button)
    RST_FAULT,      // entered resettable fault state
    NO_RST_FAULT    // entered non-resettable fault state
}StateType;

typedef enum
{
    E_START,                // start button gpio active
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
	E_NO_RST_FLT
}EventType;


typedef struct transition
{
    void (*function)(void);
    StateType next_state;
} Transition;

typedef struct  {
  StateType current_state;
  int num_states;
  int num_events;
  Transition *table;
} statemachine;

void make_state_machine(statemachine * machine, int num_states, int num_events, StateType current_state);
void add_tuple(statemachine * machine, StateType state, EventType event, StateType next_state, void (*function)(void));
void run_event(statemachine * machine, EventType event);


#endif /* MACHINE_H_ */
