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
#include "usart.h"
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
#include "tgmath.h"
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

uint8_t dma_rx_buffer[DMA_RX_BUFFER_SIZE];  // DMA接收缓冲区
volatile uint16_t dma_rx_len = 0;           // 接收到的数据长度
volatile uint8_t dma_rx_flag = 0;           // 接收完成标志

uint32_t time_start = 0;
static int16_t y_logo = 200;
static int16_t y_version = 200;
static int16_t y_name = -200;
static int16_t y_astra = -200;
static int16_t y_box = 200;
static int16_t x_board = -200;
static int16_t y_wire_1 = 200;
static int16_t y_wire_2 = 200;

void test_user_item_init_function()
{
  time_start = get_ticks();
}

void test_user_item_loop_function()
{
  // uint32_t _time = get_ticks();
  //
  // oled_set_draw_color(1);
  // oled_draw_R_box(2, y_box - 1, oled_get_UTF8_width("「astraLauncher」") + 4, oled_get_str_height() + 2, 1);
  // oled_set_draw_color(2);
  // oled_draw_UTF8(4, y_logo - 2, "「astraLauncher」");
  //
  // oled_set_draw_color(1);
  // oled_draw_str(106, y_version, "v1.0");
  // oled_draw_UTF8(2, y_name, "by 无理造物.");
  // oled_draw_UTF8(2, y_astra, "由「astra UI Lite」v1.1");
  // oled_draw_UTF8(2, y_astra + 14, "驱动.");
  // oled_draw_frame(x_board, 38, 28, 20);
  // oled_draw_frame(x_board + 2, 40, 24, 10);
  // oled_draw_box(x_board + 2, 40, 2, 10);
  // oled_draw_pixel(x_board + 25, 51);
  // oled_draw_pixel(x_board + 25, 53);
  // oled_draw_pixel(x_board + 25, 55);
  // oled_draw_box(x_board + 21, 51, 3, 2);
  // oled_draw_box(x_board + 21, 54, 3, 2);
  // oled_draw_box(x_board + 17, 53, 3, 3);
  //
  // oled_draw_box(x_board + 12, 53, 4, 3);
  // oled_draw_box(x_board + 7, 53, 4, 3);
  // oled_draw_box(x_board + 2, 53, 4, 3);
  //
  // oled_draw_box(x_board + 7, y_wire_1, 4, 3);
  // oled_draw_V_line(x_board + 9, y_wire_1 + 3, 3);
  // oled_draw_V_line(x_board + 8, y_wire_1 + 6, 2);
  //
  // oled_draw_box(x_board + 12, y_wire_2, 4, 3);
  // oled_draw_V_line(x_board + 14, y_wire_2 + 3, 3);
  // oled_draw_V_line(x_board + 15, y_wire_2 + 6, 2);
  //
  // if (_time - time_start > 300) animation(&y_logo, 15, 94);
  // if (_time - time_start > 350) animation(&y_version, 14, 88);
  // if (_time - time_start > 400) animation(&y_box, 2, 92);
  // if (_time - time_start > 450) animation(&y_astra, 36, 91);
  // if (_time - time_start > 500) animation(&y_name, 62, 94);
  // if (_time - time_start > 550) animation(&x_board, 102, 92);
  // if (_time - time_start > 620) animation(&y_wire_1, 56, 86);
  // if (_time - time_start > 1400 && _time - time_start < 1600) oled_draw_box(x_board + 5, 42, 19, 6);
  // if (_time - time_start > 1800 && _time - time_start < 1900) oled_draw_box(x_board + 5, 42, 19, 6);
  // if (_time - time_start > 2200) oled_draw_box(x_board + 5, 42, 19, 6);
  // if (_time - time_start > 2400) animation(&y_wire_2, 56, 86);
}

void test_user_item_exit_function()
{
  time_start = 0;
  y_logo = 200;
  y_version = 200;
  y_name = -200;
  y_astra = -200;
  y_box = 200;
  x_board = -200;
  y_wire_1 = 200;
  y_wire_2 = 200;
}

bool pulse_light = true;
bool key_flip = false;
bool dark_mode = false;
int16_t p_mode = 1;
bool mcu_serial_channel = false;
bool external_serial_channel = false;

bool screen_switch = false;
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
  HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, 0);
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
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
  HAL_ADCEx_Calibration_Start(&hadc1);
  HAL_ADCEx_Calibration_Start(&hadc2);

  // 启动DMA接收（循环模式，持续接收数据）
  HAL_UART_Receive_DMA(&huart2, dma_rx_buffer, DMA_RX_BUFFER_SIZE);

  astra_ui_driver_init();
  astra_init_core();

  astra_list_item_t* launcher_setting_list_item = astra_new_list_item("开发板设置");

  astra_push_item_to_list(astra_get_root_list(), launcher_setting_list_item);
  astra_push_item_to_list(astra_get_root_list(), astra_new_switch_item("切换屏幕", &screen_switch));
  astra_push_item_to_list(astra_get_root_list(), astra_new_user_item("接线图...", test_user_item_init_function, test_user_item_loop_function, test_user_item_exit_function));
  astra_push_item_to_list(astra_get_root_list(), astra_new_user_item("关于开发板...", test_user_item_init_function, test_user_item_loop_function, test_user_item_exit_function));

  astra_push_item_to_list(launcher_setting_list_item, astra_new_switch_item("心跳灯开关", &pulse_light));
  astra_push_item_to_list(launcher_setting_list_item, astra_new_switch_item("反转按键", &key_flip));
  astra_push_item_to_list(launcher_setting_list_item, astra_new_slider_item("数据显示样式", &p_mode, 1, 1, 3));
  astra_push_item_to_list(launcher_setting_list_item, astra_new_switch_item("反转显示", &dark_mode));
  astra_push_item_to_list(launcher_setting_list_item, astra_new_switch_item("MCU串口通道", &mcu_serial_channel));
  astra_push_item_to_list(launcher_setting_list_item, astra_new_switch_item("外部串口通道", &external_serial_channel));

  launcher_set_terminal_area(4, 26, 124, 62);

  launcher_push_str_to_terminal(info, "astraLauncher \nRev1.0 made by \nforpaindream.");

  char msg[100] = {};
  sprintf(msg, "启动时间: %dms.", launcher_get_tick_ms());
  astra_push_pop_up(msg,1500);

  uint16_t _tick = 0;

  // 使能IDLE中断
  __HAL_UART_ENABLE_IT(&huart2, UART_IT_IDLE);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1) {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    oled_clear_buffer();

    if (dma_rx_flag) {
      // 处理接收到的数据（长度为dma_rx_len）
      // 例如回显数据：

      //push进去的地址，指向的是原来的地址，所以这样写的话，dma_rx_buffer有变化，所有指向它的内容都会变化
      launcher_push_str_to_terminal(f411, dma_rx_buffer);
      if (!in_astra) launcher_draw_home_page();
      //给dma_rx_buffer清零
      memset(dma_rx_buffer, '\0', DMA_RX_BUFFER_SIZE);

      // 重置标志位
      dma_rx_flag = 0;
      dma_rx_len = 0;
    }

    // if (_tick % 150 == 1) launcher_push_str_to_terminal(info, "hello,\rworld!\r1");
    // if (_tick % 201 == 1) launcher_push_str_to_terminal(uart, "hello,\rworld\r2");
    // if (_tick % 251 == 1) launcher_push_str_to_terminal(info, "hello,\nworld!\r3");

    ad_astra();
    launcher_key_call_back(1, astra_selector_go_prev_item, astra_selector_go_next_item, astra_selector_exit_current_item, astra_selector_jump_to_selected_item);

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

