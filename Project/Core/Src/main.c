//The main.h library is included, which is where we can define all the pins of our board or core.
#include "main.h"

// A private variable is named huart2
UART_HandleTypeDef huart2;

// The global variables that are going to be used in the code are defined
uint32_t left_toggles = 0;
uint32_t right_toggles = 0;
uint32_t left_last_press_tick = 0;
uint32_t right_last_press_tick = 0;
uint16_t probar = 0;
uint32_t stop_toggles = 0;
uint32_t stop_last_press_tick = 0;
uint32_t state = 0;
uint32_t state2 = 0;
uint32_t state3 = 0;


// Private variables are initialized
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);

void heartbeat(void);

/*
The following function assists us in determining the frequency of alternation for each of the LEDs in the right, left, and stop functions. Additionally, each button press sends the corresponding indication to the serial monitor.

This function has the following input data conditions:

A 16-bit data.
Indication of the pressed button.
There is NO way out.
*/

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if (GPIO_Pin == S1_Pin && HAL_GetTick() > (left_last_press_tick + 200)) {
		if(right_toggles > 0){
			state = 1;
		}
		left_toggles = 0;
		right_toggles = 0;
		stop_toggles = 0;
		HAL_GPIO_WritePin(D3_GPIO_Port, D3_Pin, 1);
		HAL_GPIO_WritePin(D4_GPIO_Port, D4_Pin, 1);
		HAL_UART_Transmit(&huart2, "S1\r\n", 4, 10);
		if (HAL_GetTick() < (left_last_press_tick + 300)) {
			// If the previous press occurred within the last 300 milliseconds.
			left_toggles = 0xFFFFFF; // a long time toggling (infinite)
		} else {
			left_toggles = 6;
		}
		left_last_press_tick = HAL_GetTick();

	} else if (GPIO_Pin == S2_Pin && HAL_GetTick() > (right_last_press_tick + 200)) {
		if(left_toggles > 0){
			state2 = 1;
		}
		right_toggles = 0;
		left_toggles = 0;
		stop_toggles = 0;
		HAL_GPIO_WritePin(D3_GPIO_Port, D3_Pin, 1);
		HAL_GPIO_WritePin(D4_GPIO_Port, D4_Pin, 1);
		HAL_UART_Transmit(&huart2, "S2\r\n", 4, 10);
		if (HAL_GetTick() < (right_last_press_tick + 300)) {
			// if the most recent press occurred within the past 300 milliseconds.
			right_toggles = 0xFFFFFF; // a long time toggling (infinite)
		} else {
			right_toggles = 6;
				}
		right_last_press_tick = HAL_GetTick();

	} else if (GPIO_Pin == S3_Pin) {
		if(stop_toggles > 0){
			state3 = 1;
		}
		stop_toggles = 0;
		right_toggles = 0;
		left_toggles = 0;
		HAL_GPIO_WritePin(D3_GPIO_Port, D3_Pin, 1);
		HAL_GPIO_WritePin(D4_GPIO_Port, D4_Pin, 1);
		HAL_UART_Transmit(&huart2, "S3\r\n", 4, 10);
		stop_toggles = 0xFFFFFF; // a long time toggling (infinite)
			}
}

/*
The function depicted below represents the Heartbeat feature as requested in the specifications.
It is utilized to maintain a flashing LED while the system is operational.

This function does not require any input data, and there is no exit pathway.
*/

void heartbeat(void)
{
	static uint32_t heartbeat_tick = 0;
	if (heartbeat_tick < HAL_GetTick()) {
		heartbeat_tick = HAL_GetTick() + 500;
		HAL_GPIO_TogglePin(D1_GPIO_Port, D1_Pin);
	}
}

/*
The function presented below facilitates the execution of the specified alternations indicated at the time of interruption.
If it's a button interrupt indicating the left side, it will execute the alternations as required.
If not, it turns off the left flashing LED.

It does NOT have any type of entrance.
There is NO way out.
*/

void turn_signal_left(void)
{
	static uint32_t turn_toggle_tick = 0;
	if (turn_toggle_tick < HAL_GetTick()) {
		if (left_toggles > 0 && state != 1) {
			turn_toggle_tick = HAL_GetTick() + 500;
			HAL_GPIO_TogglePin(D3_GPIO_Port, D3_Pin);
			left_toggles--;
		} else if(stop_toggles <= 0) {
			HAL_GPIO_WritePin(D3_GPIO_Port, D3_Pin, 1);
			left_toggles = 0;
			state = 0;
	}
	}
}

/*
The function provided below aids in executing the indicated alternations upon interruption.
If the interruption indicates the right side button, it will execute the alternations as many times as needed;
otherwise, it turns off the right flashing LED.

It does NOT have any type of entrance.
There is NO way out.
*/

void turn_signal_right(void)
{
	static uint32_t turn_toggle_tick = 0;
	if (turn_toggle_tick < HAL_GetTick()){
		if (right_toggles > 0 && state2 != 1) {
			turn_toggle_tick = HAL_GetTick() + 500;
			HAL_GPIO_TogglePin(D4_GPIO_Port, D4_Pin);
			right_toggles--;
		} else if(stop_toggles <= 0){
			HAL_GPIO_WritePin(D4_GPIO_Port, D4_Pin, 1);
			right_toggles = 0;
			state2 = 0;
	}
	}
}

/*
The function shown below helps us to execute the alternations shown below.
  indicated at the time of the interruption. If it is a button interruption indicating
  The stop will then execute the alternations as many times as necessary, if it is not this one,
  Turn off the stop flashing LED.

It does NOT have any type of entrance.
There is NO way out.
 */

void stationary(void)
{
	static uint32_t turn_toggle_tick = 0;
	if (turn_toggle_tick < HAL_GetTick()){
		if (stop_toggles > 0 && state3 != 1) {
			turn_toggle_tick = HAL_GetTick() + 500;
			HAL_GPIO_TogglePin(D3_GPIO_Port, D3_Pin);
			HAL_GPIO_TogglePin(D4_GPIO_Port, D4_Pin);
			stop_toggles--;
		}  else if (right_toggles <= 0 && left_toggles <= 0){
			HAL_GPIO_WritePin(D3_GPIO_Port, D3_Pin, 1);
			HAL_GPIO_WritePin(D4_GPIO_Port, D4_Pin, 1);
			stop_toggles = 0;
			state3 = 0;
		}
	}
}



int main(void)
{

//Each of the necessary functions is executed.
  HAL_Init();

  SystemClock_Config();

  MX_GPIO_Init();
  MX_USART2_UART_Init();

/* To ensure that all four functions for the heartbeat, left side, right side, and top are continuously executed,
   we enclose them within a while loop for constant validation.
*/
  while (1)
  {
	  heartbeat();
	  turn_signal_left();
	  turn_signal_right();
	  stationary();

  }

}

void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** The output voltage of the main internal regulator must be configured.
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /*The RCC oscillators are initialized according to the specified parameters.
    Within the RCC_OscInitTypeDef structure.
  */

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** The clocks of the CPU, AHB and APB buses are initialized.
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, D1_Pin|D3_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(D4_GPIO_Port, D4_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : S1_Pin S2_Pin */
  GPIO_InitStruct.Pin = S1_Pin|S2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : D1_Pin D3_Pin */
  GPIO_InitStruct.Pin = D1_Pin|D3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : S3_Pin */
  GPIO_InitStruct.Pin = S3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(S3_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : D4_Pin */
  GPIO_InitStruct.Pin = D4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(D4_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI0_IRQn);

  HAL_NVIC_SetPriority(EXTI1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI1_IRQn);

  HAL_NVIC_SetPriority(EXTI4_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI4_IRQn);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
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
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
