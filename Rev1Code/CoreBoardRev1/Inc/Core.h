/*
 * Core.h
 *
 *  Created on: Mar 11, 2018
 *      Author: shwet
 */

#ifndef CORE_H_
#define CORE_H_

#include "gpio.h"
#include "machine.h"
#include "sm_functions.h"
#include "can.h"
#include "mycan.h"
#include "scheduler.h"
#include "identifiers.h"
#include "init_sm.h"

#define CHECK_BIT(var, pos) ((var) & (1<<(pos)))

#define RESET_HEARTBEAT 100
#define MAX_CURRENT 80
#define PRESSED 1000 // pressed threshold
#define MAX_THROTTLE_VAL 0xFFFF // ask David + Tyler
#define RESET_START 1000;

void mainloop();
void send_heartbeat();
void send_state();
void get_CAN();

#endif /* CORE_H_ */
