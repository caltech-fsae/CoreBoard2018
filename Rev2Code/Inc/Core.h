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

#define RESET_HEARTBEAT 1000
#define MAX_CURRENT 80
#define PRESSED 1000 // pressed threshold
#define MAX_THROTTLE_VAL 0xFFFF // ask David + Tyler
#define RESET_START 1000;
#define RESET_FLT_CNT 100;

#define START_GPIO_Port			GPIOE
#define START_Pin				GPIO_PIN_3

#define RTDS_GPIO_Port			GPIOE
#define RTDS_Pin				GPIO_PIN_2

#define BRAKELIGHT_GPIO_Port	GPIOE
#define BRAKELIGHT_Pin			GPIO_PIN_1

#define FLT_NR_GPIO_Port		GPIOD
#define FLT_NR_Pin				GPIO_PIN_0

#define FLT_GPIO_Port			GPIOC
#define FLT_Pin					GPIO_PIN_11

#define FLT_NR_CTRL_GPIO_Port	GPIOC
#define FLT_NR_CTRL_Pin			GPIO_PIN_12

#define FLT_CTRL_GPIO_Port		GPIOC
#define FLT_CTRL_Pin			GPIO_PIN_10

#define AUX_LED_0_GROUP			GPIOB
#define AUX_LED_0_PIN			GPIO_PIN_12

#define AUX_LED_1_GROUP			GPIOB
#define AUX_LED_1_PIN			GPIO_PIN_13

#define AUX_LED_2_GROUP			GPIOB
#define AUX_LED_2_PIN			GPIO_PIN_14

#define AUX_LED_3_GROUP			GPIOB
#define AUX_LED_3_PIN			GPIO_PIN_15

// Prototypes ------------------------------------------------------------------------------

void mainloop();
void send_heartbeat();
void send_state();
void get_CAN();
void assert_FLT_lines();

void WriteAUXLED(uint8_t led, uint8_t state);
void ToggleAUXLED(uint8_t led);

#endif /* CORE_H_ */
