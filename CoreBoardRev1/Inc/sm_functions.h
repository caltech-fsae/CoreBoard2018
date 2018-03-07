/*
 * sm_functions.h
 *
 *  Created on: Mar 6, 2018
 *      Author: shwet
 */

#ifndef SM_FUNCTIONS_H_
#define SM_FUNCTIONS_H_


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
void send_CAN();
//below functions will not use send_CAN

void do_nothing();
void BRAKE_LIGHT_ON();
void BRAKE_LIGHT_OFF();

#endif /* SM_FUNCTIONS_H_ */
