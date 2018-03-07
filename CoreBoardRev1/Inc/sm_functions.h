/*
 * sm_functions.h
 *
 *  Created on: Mar 6, 2018
 *      Author: shwet
 */

#ifndef SM_FUNCTIONS_H_
#define SM_FUNCTIONS_H_

// Bit 5: generic fault (YES/NO)
// Bit 4: IMD     fault (YES/NO)
// Bit 3: BSPD    fault (YES/NO)
// Bit 2: APPS    fault (YES/NO)
// Bit 1: BSE     fault (YES/NO)
// Bit 0: BMS     fault (YES/NO)

static const uint16_t GENERIC_NO_RST  = 0b100000;
static const uint16_t IMD_FLT         = 0b10000;
static const uint16_t BSPD_FLT        = 0b1000;
static const uint16_t APPS_FLT        = 0b100;
static const uint16_t BSE_FLT         = 0b10;
static const uint16_t BMS_FLT         = 0b1;

// Bit 1: generic fault (YES/NO)
// Bit 0: BPPC    fault (YES/NO)
static const uint16_t GENERIC_RST     = 0b10;
static const uint16_t BPPC_FLT        = 0b1;

void RTDS();
void PEDAL_ACEL();
void PEDAL_BRAKE_PUSHED();
void PEDAL_BRAKE_RELEASED();
void PWR_80();
void BPPC_RST();
void IMD_NO_RST();
void BSPD_NO_RST();
void APPS_NO_RST();
void BSE_NO_RST();
void BMS_NO_RST();
void RST();
void NO_RST();

//above functions will use send_CAN
void send_CAN(uint16_t MID, uint16_t message);
void send_FLT_CAN(uint16_t MID, uint16_t message);
//below functions will not use send_CAN

void do_nothing();
void BRAKE_LIGHT_ON();
void BRAKE_LIGHT_OFF();

#endif /* SM_FUNCTIONS_H_ */
