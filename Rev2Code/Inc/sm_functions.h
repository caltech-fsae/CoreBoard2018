/*
 * sm_functions.h
 *
 *  Created on: Mar 6, 2018
 *      Author: shwet
 */

#ifndef SM_FUNCTIONS_H_
#define SM_FUNCTIONS_H_

#include <stdint.h>
#include "identifiers.h"

// Messages for MID_FAULT_CAUSE // NOT USING THIS

// Resettable Faults (bits 8 - 15)
// Bit 8: BPPC      fault (YES/NO)

// Nonresettable Fault (bits 0 - 7)
// Bit 5: HEARTBEAT fault (YES/NO)
// Bit 4: IMD       fault (YES/NO)
// Bit 3: BSPD      fault (YES/NO)
// Bit 2: APPS      fault (YES/NO)
// Bit 1: BSE       fault (YES/NO)
// Bit 0: BMS       fault (YES/NO)
// Generic NR fault if no bits set, Generic R fault if all bits set


// NOT USING THESE, left in to allow for compilation.
static const uint16_t GENERIC_NO_RST  = 0b0000000000000000;
static const uint16_t IMD_FLT         = 0b0000000000010000;
static const uint16_t BSPD_FLT        = 0b0000000000001000;
static const uint16_t APPS_FLT        = 0b0000000000000100;
static const uint16_t BSE_FLT         = 0b0000000000000010;
static const uint16_t BMS_FLT         = 0b0000000000000001;

static const uint16_t GENERIC_RST     = 0b1111111111111111;
static const uint16_t BPPC_FLT        = 0b0000000100000000;



void send_CAN(uint16_t, uint16_t);
void send_FLT_CAN();

void RESET_FAULTS();
void END_DRIVE();
void RTDS();
void PEDAL_ACEL();
void PWR_80();
void send_stop_drive();
//above functions will use send_CAN
//below functions will not use send_CAN

void StartPrecharge();
void do_nothing();
void ExitFault();
void SetFinishPrecharge();
void reset_precharge_timer();
void reset_init_heartbeats();
void attempt_driver_reset();
void set_ignore_nr_line();
void reset_ignore_nr_line();

void PEDAL_BRAKE_PUSHED();
void PEDAL_BRAKE_RELEASED();

int WAIT_HEARTBEATS;
int WAIT_DRIVER;
int START_BRAKE;
int DRIVE;
int RST_FAULT;
int NO_RST_FAULT;




#endif /* SM_FUNCTIONS_H_ */
