/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2018 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx_hal.h"
#include "can.h"
#include "gpio.h"
#include "machine.h"
#include "sm_functions.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  *
  * @retval None
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_CAN1_Init();
  /* USER CODE BEGIN 2 */

  // to-do: init statemachine

    statemachine sm;
    make_state_machine(&sm, NUM_STATES, NUM_EVENTS, WAIT);

    // machine, state, event, next_state, function

    //STATE: WAIT
    add_tuple(&sm, WAIT, E_START,                WAIT,         &do_nothing);
    add_tuple(&sm, WAIT, E_PEDAL_ACEL,           WAIT,         &do_nothing);
    add_tuple(&sm, WAIT, E_PEDAL_BRAKE_RELEASED, WAIT,         &PEDAL_BRAKE_RELEASED);
    add_tuple(&sm, WAIT, E_PEDAL_BRAKE_PUSHED,   START_BRAKE,  &PEDAL_BRAKE_PUSHED);
    add_tuple(&sm, WAIT, E_PWR_80,               WAIT,         &PWR_80);
    add_tuple(&sm, WAIT, E_BPPC_FLT,             RST_FAULT,    &BPPC_RST);
    add_tuple(&sm, WAIT, E_IMD_FLT,              NO_RST_FAULT, &IMD_NO_RST);
    add_tuple(&sm, WAIT, E_BSPD_FLT,             NO_RST_FAULT, &BSPD_NO_RST);
    add_tuple(&sm, WAIT, E_APPS_FLT,             NO_RST_FAULT, &APPS_NO_RST);
    add_tuple(&sm, WAIT, E_BSE_FLT,              NO_RST_FAULT, &BSE_NO_RST);
    add_tuple(&sm, WAIT, E_BMS_FLT,              NO_RST_FAULT, &BMS_NO_RST);

    //STATE: DRIVE
    add_tuple(&sm, DRIVE, E_START,                DRIVE         &do_nothing);
    add_tuple(&sm, DRIVE, E_PEDAL_ACEL,           DRIVE,        &PEDAL_ACEL);
    add_tuple(&sm, DRIVE, E_PEDAL_BRAKE_RELEASED, DRIVE,        &PEDAL_BRAKE_RELEASED);
    add_tuple(&sm, DRIVE, E_PEDAL_BRAKE_PUSHED,   DRIVE,        &PEDAL_BRAKE_PUSHED);
    add_tuple(&sm, DRIVE, E_PWR_80,               DRIVE,        &PWR_80);
    add_tuple(&sm, DRIVE, E_BPPC_FLT,             RST_FAULT,    &BPPC_RST);
    add_tuple(&sm, DRIVE, E_IMD_FLT,              NO_RST_FAULT, &IMD_NO_RST);
    add_tuple(&sm, DRIVE, E_BSPD_FLT,             NO_RST_FAULT, &BSPD_NO_RST);
    add_tuple(&sm, DRIVE, E_APPS_FLT,             NO_RST_FAULT, &APPS_NO_RST);
    add_tuple(&sm, DRIVE, E_BSE_FLT,              NO_RST_FAULT, &BSE_NO_RST);
    add_tuple(&sm, DRIVE, E_BMS_FLT,              NO_RST_FAULT, &BMS_NO_RST);

    //STATE: START_BRAKE
    add_tuple(&sm, START_BRAKE, E_START,                DRIVE         &RTDS);
    add_tuple(&sm, START_BRAKE, E_PEDAL_ACEL,           WAIT,         &send_CAN);
    add_tuple(&sm, START_BRAKE, E_PEDAL_BRAKE_RELEASED, WAIT,         &send_CAN);
    add_tuple(&sm, START_BRAKE, E_PEDAL_BRAKE_PUSHED,   START_BRAKE,  &do_nothing);
    add_tuple(&sm, START_BRAKE, E_PWR_80,               START_BRAKE,  &PWR_80);
    add_tuple(&sm, START_BRAKE, E_BPPC_FLT,             RST_FAULT,    &BPPC_RST);
    add_tuple(&sm, START_BRAKE, E_IMD_FLT,              NO_RST_FAULT, &IMD_NO_RST);
    add_tuple(&sm, START_BRAKE, E_BSPD_FLT,             NO_RST_FAULT, &BSPD_NO_RST);
    add_tuple(&sm, START_BRAKE, E_APPS_FLT,             NO_RST_FAULT, &APPS_NO_RST);
    add_tuple(&sm, START_BRAKE, E_BSE_FLT,              NO_RST_FAULT, &BSE_NO_RST);
    add_tuple(&sm, START_BRAKE, E_BMS_FLT,              NO_RST_FAULT, &BMS_NO_RST);

    //STATE: RST_FAULT
    add_tuple(&sm, RST_FAULT, E_START,                RST_FAULT     &do_nothing);
    add_tuple(&sm, RST_FAULT, E_PEDAL_ACEL,           RST_FAULT,    &do_nothing);
    add_tuple(&sm, RST_FAULT, E_PEDAL_BRAKE_RELEASED, RST_FAULT,    &do_nothing);
    add_tuple(&sm, RST_FAULT, E_PEDAL_BRAKE_PUSHED,   START_BRAKE,  &PEDAL_BRAKE_PUSHED);
    add_tuple(&sm, RST_FAULT, E_PWR_80,               RST_FAULT,    &PWR_80);
    add_tuple(&sm, RST_FAULT, E_BPPC_FLT,             RST_FAULT,    &do_nothing);
    add_tuple(&sm, RST_FAULT, E_IMD_FLT,              NO_RST_FAULT, &IMD_NO_RST);
    add_tuple(&sm, RST_FAULT, E_BSPD_FLT,             NO_RST_FAULT, &BSPD_NO_RST);
    add_tuple(&sm, RST_FAULT, E_APPS_FLT,             NO_RST_FAULT, &APPS_NO_RST);
    add_tuple(&sm, RST_FAULT, E_BSE_FLT,              NO_RST_FAULT, &BSE_NO_RST);
    add_tuple(&sm, RST_FAULT, E_BMS_FLT,              NO_RST_FAULT, &BMS_NO_RST);


    //STATE: NO_RST_FAULT
    add_tuple(&sm, NO_RST_FAULT, E_START,                NO_RST_FAULT     &do_nothing);
    add_tuple(&sm, NO_RST_FAULT, E_PEDAL_ACEL,           NO_RST_FAULT,    &do_nothing);
    add_tuple(&sm, NO_RST_FAULT, E_PEDAL_BRAKE_RELEASED, NO_RST_FAULT,    &BRAKE_LIGHT_OFF);
    add_tuple(&sm, NO_RST_FAULT, E_PEDAL_BRAKE_PUSHED,   NO_RST_FAULT,    &BRAKE_LIGHT_ON);
    add_tuple(&sm, NO_RST_FAULT, E_PWR_80,               NO_RST_FAULT,    &PWR_80);
    add_tuple(&sm, NO_RST_FAULT, E_BPPC_FLT,             NO_RST_FAULT,    &do_nothing);
    add_tuple(&sm, NO_RST_FAULT, E_IMD_FLT,              NO_RST_FAULT,    &IMD_NO_RST);
    add_tuple(&sm, NO_RST_FAULT, E_BSPD_FLT,             NO_RST_FAULT,    &BSPD_NO_RST);
    add_tuple(&sm, NO_RST_FAULT, E_APPS_FLT,             NO_RST_FAULT,    &APPS_NO_RST);
    add_tuple(&sm, NO_RST_FAULT, E_BSE_FLT,              NO_RST_FAULT,    &BSE_NO_RST);
    add_tuple(&sm, NO_RST_FAULT, E_BMS_FLT,              NO_RST_FAULT,    &BMS_NO_RST);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

  /* USER CODE END WHILE */

	  // transmit CAN message idiom
	  /*
	  msg = 0x00000 // update with appropriate msg, construct from common.h/c
	  CAN_short_msg(&can_msg, create_ID(0,0), msg);
	  CAN_queue_transmit(&can_msg);
	  */
  	  // receive CAN message idiom
 	  /*
  	  if (CAN_dequeue_msg(&can_msg))
  	  {
  	      msg = CAN_decode_short(&can_msg);
  	  }
  	  // process msg using common.h/c
  	  */


  /* USER CODE BEGIN 3 */

  }
  /* USER CODE END 3 */

}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

    /**Configure the main internal regulator output voltage 
    */
  __HAL_RCC_PWR_CLK_ENABLE();

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  file: The file name as string.
  * @param  line: The line in file as a number.
  * @retval None
  */
void _Error_Handler(char *file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
