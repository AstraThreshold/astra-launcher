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
#include "u8g2/u8g2.h"
#include "key_launcher.h"
#include "adc_launcher.h"
#include "stdio.h"
#include "astra-ui-lite/draw_driver.h"
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




/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void) {
  /* USER CODE BEGIN 1 */

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

//  oledInit();
//  u8g2Init(&u8g2);
  astra_ui_driver_init();

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1) {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    u8g2_ClearBuffer(&u8g2);

//    oled_set_draw_color(1);
//    char voltageChar[10];
//    sprintf(voltageChar, "%05.2f", (volADC1 + volADC2) / 2);
//    oled_set_font(u8g2_font_boutique_bitmap_9x9_tn);
//    oled_set_draw_color(1);
//    oled_draw_str(87, 9, voltageChar);
//    oled_draw_R_box(118, 2, 8, 8, 1);
//    oled_set_font(u8g2_font_spleen5x8_mr);
//    oled_set_draw_color(2);
//    oled_draw_str(120, 9, "V");
//    oled_set_draw_color(1);
//    oled_set_font(u8g2_font_spleen5x8_mr);
//    oled_draw_str(75, 9, "u<");

//    char currentChar[10];
//    sprintf(currentChar, "%.3f", (iBoardADC1 + iBoardADC2) / 2);
//    oled_set_font(u8g2_font_boutique_bitmap_9x9_tn);
//    oled_set_draw_color(1);
//    oled_draw_str(87, 18, currentChar);
//    oled_draw_R_box(118, 11, 8, 8, 1);
//    oled_set_font(u8g2_font_spleen5x8_mr);
//    oled_set_draw_color(2);
//    oled_draw_str(120, 18, "A");
//    oled_set_draw_color(1);
//    oled_set_font(u8g2_font_spleen5x8_mr);
//    oled_draw_str(75, 18, "i<");
//
//    char pwrChar[10];
//    sprintf(pwrChar, "%05.2f", (volADC1 + volADC2) * (iBoardADC1 + iBoardADC2) / 4);
//    oled_set_font(u8g2_font_boutique_bitmap_9x9_tn);
//    oled_set_draw_color(1);
//    oled_draw_str(87, 27, pwrChar);
//    oled_draw_R_box(118, 20, 8, 8, 1);
//    oled_set_font(u8g2_font_spleen5x8_mr);
//    oled_set_draw_color(2);
//    oled_draw_str(120, 27, "W");
//    oled_set_draw_color(1);
//    oled_set_font(u8g2_font_spleen5x8_mr);
//    oled_draw_str(75, 27, "p<");
//
//    char currentBaseChar[10];
//    sprintf(currentBaseChar, "%05.2f", iBaseADC1);
//    oled_set_font(u8g2_font_boutique_bitmap_9x9_tn);
//    oled_set_draw_color(1);
//    oled_draw_str(87, 36, currentBaseChar);
//    oled_draw_R_box(118, 29, 8, 8, 1);
//    oled_set_font(u8g2_font_spleen5x8_mr);
//    oled_set_draw_color(2);
//    oled_draw_str(120, 36, "A");
//    oled_set_draw_color(1);
//    oled_set_font(u8g2_font_spleen5x8_mr);
//    oled_draw_str(75, 36, "i>");
//
//    char tempChar[10];
//    sprintf(tempChar, "%05.1f", innerTempADC1);
//    oled_set_font(u8g2_font_boutique_bitmap_9x9_tn);
//    oled_set_draw_color(1);
//    oled_draw_str(87, 45, tempChar);
//    oled_draw_R_box(118, 38, 8, 8, 1);
//    oled_set_font(u8g2_font_spleen5x8_mr);
//    oled_set_draw_color(2);
//    oled_draw_str(120, 45, "W");
//    oled_set_draw_color(1);
//    oled_set_font(u8g2_font_spleen5x8_mr);
//    oled_draw_str(75, 45, "p>");

//    keyCallBack(2, key1Clicked, key2Clicked, key1Pressed, key2Pressed);
//    char key1CntChar[10];
//    char key2CntChar[10];
//    sprintf(key1CntChar, "%d", key1Cnt);
//    sprintf(key2CntChar, "%d", key2Cnt);
//    oled_draw_str(&u8g2, 0, 60, "key1: ");
//    oled_draw_str(&u8g2, 28, 60, key1CntChar);
//    oled_draw_str(&u8g2, 40, 60, " key2: ");
//    oled_draw_str(&u8g2, 74, 60, key2CntChar);

//    oled_draw_R_frame(0, 0, 70, 64, 3);
//    oled_draw_R_frame(1, 1, 68, 62, 3);
//    oled_draw_pixel(70, 51);
//    oled_draw_line(70, 47, 72, 49);
//    oled_draw_line(70, 51, 72, 49);
//    oled_draw_pixel(71, 49);
//    oled_draw_pixel(70, 48);
//    oled_draw_pixel(70, 49);
//    oled_draw_pixel(70, 50);
//    oled_draw_R_frame(72, 0, 56, 48, 3);
//    oled_draw_R_frame(72, 50, 56, 14, 3);
//
//    oled_draw_V_line(126, 51, 12);
//    oled_draw_V_line(125, 50, 14);

    oled_draw_R_box(0, 14, 128, 60, 3);
    oled_set_draw_color(0);
    oled_draw_R_box(2, 15, 124, 47, 2);
    oled_set_draw_color(1);
    oled_draw_R_box(122, 18, 2, 20, 1);

    oled_set_draw_color(1);
    char voltageChar[10];
    sprintf(voltageChar, "%05.2f", (volADC1 + volADC2) / 2);
    oled_set_font(u8g2_font_spleen5x8_mr);
    oled_set_draw_color(1);
    oled_draw_str(56, 9, voltageChar);
    oled_draw_R_box(82, 2, 8, 8, 1);
    oled_set_draw_color(2);
    oled_draw_str(82 + 2, 9, "V");
    oled_set_draw_color(1);

    oled_set_draw_color(1);
    char currentBaseChar[10];
    sprintf(currentBaseChar, "%05.2f", iBaseADC1);
    oled_set_font(u8g2_font_spleen5x8_mr);
    oled_set_draw_color(1);
    oled_draw_str(92, 9, currentBaseChar);
    oled_draw_R_box(118, 2, 8, 8, 1);
    oled_set_draw_color(2);
    oled_draw_str(118 + 2, 9, "A");
    oled_set_draw_color(1);

    oled_draw_R_box(2, 1, 23, 10, 1);
    oled_set_font(u8g2_font_spleen5x8_mr);
    oled_set_draw_color(2);
    oled_draw_str(4, 9, "-OK-");
    oled_set_draw_color(1);

    oled_draw_H_line(2, 12, 124);

    //串口打印思路 一个字一个字打印 检测到超过边界就换行 空格的间距因为一个字一个字打印 所以可以缩小

    oled_set_font(u8g2_font_spleen5x8_mr);
    oled_draw_R_box(4, 17, 20, 10, 1);
    oled_set_draw_color(2);
    oled_draw_str(4, 25, "info");
    oled_set_draw_color(1);
    oled_draw_V_line(24, 19, 6);
    oled_draw_V_line(25, 20, 4);
    oled_draw_V_line(26, 21, 2);

    oled_set_font(u8g2_font_spleen5x8_mr);
    oled_draw_R_box(4, 28, 20, 10, 1);
    oled_set_draw_color(2);
    oled_draw_str(4, 36, "info");
    oled_set_draw_color(1);
    oled_draw_V_line(24, 30, 6);
    oled_draw_V_line(25, 31, 4);
    oled_draw_V_line(26, 32, 2);

    oled_set_font(u8g2_font_spleen5x8_mr);
    oled_draw_R_box(4, 39, 20, 10, 1);
    oled_set_draw_color(2);
    oled_draw_str(5, 47, "f411");
    oled_set_draw_color(1);
    oled_draw_V_line(24, 41, 6);
    oled_draw_V_line(25, 42, 4);
    oled_draw_V_line(26, 43, 2);

    oled_set_font(u8g2_font_spleen5x8_mr);
    oled_draw_R_box(4, 50, 20, 10, 1);
    oled_set_draw_color(2);
    oled_draw_str(5, 58, "uart");
    oled_set_draw_color(1);
    oled_draw_V_line(24, 52, 6);
    oled_draw_V_line(25, 53, 4);
    oled_draw_V_line(26, 54, 2);

    oled_set_font(u8g2_font_wqy12_t_chinese1);
    oled_draw_str(30, 25, "20000727.");
    oled_draw_UTF8(30, 37, "你好.");
    oled_draw_str(30, 48, "forpaindream!");
    oled_draw_str(30, 59, "hello world!");
    oled_draw_box(91, 57, 6, 2);

    u8g2_SendBuffer(&u8g2);
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void) {
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
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
                                | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK) {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK) {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void) {
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

