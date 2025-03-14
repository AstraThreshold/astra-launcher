/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "spi.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <sys/types.h>

#include "astra-launcher/launcher_key.h"
#include "astra-launcher/launcher_adc.h"
#include "stdio.h"
#include "astra-launcher/launcher_delay.h"
#include "astra-ui-lite/astra_ui_item.h"
#include "astra-launcher/launcher_home_page.h"
#include "astra-ui-lite/astra_ui_core.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void test_user_item_init_function()
{
  HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, 1);
}

void test_user_item_loop_function()
{
  oled_set_draw_color(1);
  char test_str[10] = {};
  sprintf(test_str, "%d", launcher_get_tick_ms());
  // oled_draw_str(20, 50, "test_str");
  oled_set_draw_color(2);
  oled_draw_box(0,0,128,64);
  oled_draw_str(3, 30, "this is a test user item");
}

void test_user_item_exit_function()
{
  HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, 0);
}

// astra_list_item_t astra_list_item_test1 = {list_item, "返回主页面"};
// astra_list_item_t astra_list_item_test2 = {button_item, "玩得开心"};
// astra_list_item_t astra_list_item_test3 = {switch_item, "你好世界", 0};
// astra_list_item_t astra_list_item_test4 = {slider_item, "返回主页返回主页"};
// astra_list_item_t astra_list_item_test5 = {list_item, "你好世界"};
// astra_list_item_t astra_list_item_test6 = {user_item, "Just Test1"};
// astra_list_item_t astra_list_item_test7 = {user_item, "Just Test11"};
// astra_list_item_t astra_list_item_test8 = {user_item, "Just Test111"};
// astra_list_item_t astra_list_item_test9 = {user_item, "test user item"};
// astra_list_item_t astra_list_item_test10 = {user_item, "Just Test00"};
// astra_list_item_t astra_list_item_test11 = {user_item, "Just Test000"};

uint8_t button_test_value = 0;
uint8_t switch_test_value = 1;
uint8_t slider_test_value = 100;

void null_function() {}

void astra_ui_entry_prompt_1()
{
  astra_push_pop_up("按键1被按下.",2000);
  key1Cnt++;
}

void astra_ui_entry_prompt_2()
{
  astra_push_pop_up("key 2 pressed.",2000);
  key2Cnt++;
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
  HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, 0);

  // astra_bind_init_function_to_user_item(&astra_list_item_test9, test_user_item_init_function);
  // astra_bind_loop_function_to_user_item(&astra_list_item_test9, test_user_item_loop_function);
  //
  // astra_push_item_to_list(&astra_list_item_root, &astra_list_item_test1);
  // astra_push_item_to_list(&astra_list_item_root, &astra_list_item_test2);
  // astra_push_item_to_list(&astra_list_item_root, &astra_list_item_test3);
  // astra_push_item_to_list(&astra_list_item_root, &astra_list_item_test9);
  // astra_push_item_to_list(&astra_list_item_root, &astra_list_item_test10);
  // // astra_push_item_to_list(&astra_list_item_root, &astra_list_item_test11);
  // astra_push_item_to_list(&astra_list_item_test3, &astra_list_item_test4);
  // astra_push_item_to_list(&astra_list_item_test3, &astra_list_item_test5);
  // astra_push_item_to_list(&astra_list_item_test3, &astra_list_item_test6);
  // astra_push_item_to_list(&astra_list_item_test3, &astra_list_item_test7);
  // astra_push_item_to_list(&astra_list_item_test3, &astra_list_item_test8);
  //
  // astra_bind_value_to_list_item(&astra_list_item_test2, &button_test_value);
  // astra_bind_value_to_list_item(&astra_list_item_test3, &switch_test_value);
  // astra_bind_value_to_list_item(&astra_list_item_test4, &slider_test_value);

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

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
  MX_SPI2_Init();
  MX_DMA_Init();
  MX_ADC1_Init();
  MX_ADC2_Init();
  /* USER CODE BEGIN 2 */
  HAL_ADCEx_Calibration_Start(&hadc1);
  HAL_ADCEx_Calibration_Start(&hadc2);

  astra_ui_driver_init();
  astra_init_core();

  astra_list_item_t* test_sec_list = astra_new_list_item(list_item, "second page", NULL);
  astra_push_item_to_list(astra_get_root_list(), astra_new_list_item(list_item, "你好世界!", NULL));
  astra_push_item_to_list(astra_get_root_list(), astra_new_list_item(list_item, "hello", NULL));
  astra_push_item_to_list(astra_get_root_list(), astra_new_list_item(switch_item, "world!", NULL));
  astra_push_item_to_list(astra_get_root_list(), astra_new_list_item(switch_item, "astra ui", NULL));
  astra_push_item_to_list(astra_get_root_list(), astra_new_list_item(slider_item, "lite version!", NULL));
  astra_push_item_to_list(astra_get_root_list(), test_sec_list);
  astra_push_item_to_list(astra_get_root_list(), astra_new_user_item("user item test!", test_user_item_init_function, test_user_item_loop_function, test_user_item_exit_function));
  astra_push_item_to_list(test_sec_list, astra_new_list_item(list_item, "你好世界!", NULL));
  astra_push_item_to_list(test_sec_list, astra_new_list_item(list_item, "你好世界!", NULL));
  astra_push_item_to_list(test_sec_list, astra_new_list_item(list_item, "你好世界!", NULL));
  astra_push_item_to_list(test_sec_list, astra_new_list_item(list_item, "你好世界!", NULL));
  astra_push_item_to_list(test_sec_list, astra_new_list_item(list_item, "你好世界!", NULL));
  astra_push_item_to_list(test_sec_list, astra_new_list_item(list_item, "你好世界!", NULL));
  astra_push_item_to_list(test_sec_list, astra_new_list_item(list_item, "你好世界!", NULL));
  astra_push_item_to_list(test_sec_list, astra_new_list_item(list_item, "你好世界!", NULL));


  launcher_set_terminal_area(4, 26, 124, 62);
  static uint32_t _tick = 0;

  launcher_push_str_to_terminal(info, "launcher Rev1.0\nmade by forpaindream");

  char msg[100] = {};
  sprintf(msg, "启动时间: %dms.", launcher_get_tick_ms());
  astra_push_pop_up(msg,1500);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1) {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    oled_clear_buffer();

    // if (_tick % 150 == 1) launcher_push_str_to_terminal(info, "你好,\rworld!\r1");
    // if (_tick % 301 == 1) launcher_push_str_to_terminal(uart, "hello,\r你好\r2");
    // if (_tick % 400 == 1) launcher_push_str_to_terminal(info, "你好,\nworld!\r3");
    // if (_tick % 500 == 1) launcher_push_str_to_terminal(f411, "hello,\r你好\r4");
    // if (_tick % 600 == 1) launcher_push_str_to_terminal(info, "你好,\nworld!\r5");
    // if (_tick % 708 == 1) launcher_push_str_to_terminal(info, "你好,\nworld!\r5");
    // if (_tick % 808 == 1) launcher_push_str_to_terminal(info, "你好,\nworld!\r5");

    ad_astra();
    launcher_key_call_back(1, astra_selector_go_prev_item, astra_selector_go_next_item, astra_selector_jump_to_prev_layer, astra_selector_jump_to_next_layer);

    if (!in_astra) launcher_draw_home_page();

    astra_ui_main_core();
    astra_ui_widget_core();

    oled_send_buffer();
    _tick++;
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV2;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
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
  while (1) {
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

