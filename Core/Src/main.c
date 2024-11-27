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
#include <math.h>
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "spi.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "u8g2/u8g2.h"
#include "key.h"
#include "stdio.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define OLED_DC_Clr() HAL_GPIO_WritePin(OLED_DC_GPIO_Port, OLED_DC_Pin, GPIO_PIN_RESET)
#define OLED_DC_Set() HAL_GPIO_WritePin(OLED_DC_GPIO_Port, OLED_DC_Pin, GPIO_PIN_SET)
#define OLED_RST_Clr() HAL_GPIO_WritePin(OLED_RST_GPIO_Port, OLED_RST_Pin, GPIO_PIN_RESET)
#define OLED_RST_Set() HAL_GPIO_WritePin(OLED_RST_GPIO_Port, OLED_RST_Pin, GPIO_PIN_SET)
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
uint8_t u8x8_byte_4wire_hw_spi(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr) {
  uint8_t *p = (uint8_t *) arg_ptr;
  switch (msg) {
    /*通过SPI发�?�arg_int个字节数�?*/
    case U8X8_MSG_BYTE_SEND:
      for (int i = 0; i < arg_int; i++)
        HAL_SPI_Transmit(&hspi2,
                         (const uint8_t *) (p + i),
                         1,
                         1000);
      break;

      /*设置DC引脚，DC引脚控制发�?�的是数据还是命�?*/
    case U8X8_MSG_BYTE_SET_DC:
      if (arg_int) OLED_DC_Set();
      else
        OLED_DC_Clr();
      break;

      /* 下面功能无需定义 */

      /*�?始传输前会进行的操作，如果使用软件片选可以在这里进行控制*/
    case U8X8_MSG_BYTE_START_TRANSFER:break;

      /*传输后进行的操作，如果使用软件片选可以在这里进行控制*/
    case U8X8_MSG_BYTE_END_TRANSFER:break;
    default:return 0;
  }
  return 1;
}

uint8_t u8x8_stm32_gpio_and_delay(U8X8_UNUSED u8x8_t *u8x8,
                                  U8X8_UNUSED uint8_t msg, U8X8_UNUSED uint8_t arg_int,
                                  U8X8_UNUSED void *arg_ptr) {
  switch (msg) {
    case U8X8_MSG_GPIO_AND_DELAY_INIT: /*delay和GPIO的初始化，在main中已经初始化完成�????*/
      break;
    case U8X8_MSG_DELAY_MILLI: /*延时函数*/
      HAL_Delay(arg_int);
      break;
    case U8X8_MSG_GPIO_CS: /*片�?�信�????*/
      //HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, static_cast<GPIO_PinState>(_argInt));
      break;
    case U8X8_MSG_GPIO_DC:
    case U8X8_MSG_GPIO_RESET:
    default: break;
  }
  return 1;
}

void u8g2Init(u8g2_t *u8g2) {
  u8g2_Setup_ssd1306_128x64_noname_f(u8g2,
                                     U8G2_R0,
                                     u8x8_byte_4wire_hw_spi,
                                     u8x8_stm32_gpio_and_delay);  // 初始�???? u8g2 结构�????
  u8g2_InitDisplay(u8g2); // 根据�????选的芯片进行初始化工作，初始化完成后，显示器处于关闭状�??
  u8g2_SetPowerSave(u8g2, 0); // 打开显示�????
  u8g2_ClearBuffer(u8g2);

  //delay(100);

  u8g2_SetFontMode(u8g2, 1); /*字体模式选择*/
  u8g2_SetFontDirection(u8g2, 0); /*字体方向选择*/
  u8g2_SetFont(u8g2, u8g2_font_myfont); /*字库选择*/
}

void _ssd1306_transmit_cmd(unsigned char _cmd) { //NOLINT
  //HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(OLED_DC_GPIO_Port, OLED_DC_Pin, GPIO_PIN_RESET);
  HAL_SPI_Transmit_DMA(&hspi2, &_cmd, 1);
  //while (HAL_SPI_GetState(&hspi2) != HAL_SPI_STATE_READY);
  //HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_SET);
}

void _ssd1306_transmit_data(unsigned char _data, unsigned char _mode) { //NOLINT
  if (!_mode) _data = ~_data;
  //HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(OLED_DC_GPIO_Port, OLED_DC_Pin, GPIO_PIN_SET);
  HAL_SPI_Transmit_DMA(&hspi2, &_data, 1);
  //while (HAL_SPI_GetState(&hspi2) != HAL_SPI_STATE_READY);
  //HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_SET);
}

void _ssd1306_set_cursor(unsigned char _x, unsigned char _y) {
  _ssd1306_transmit_cmd(0xB0 | _y);
  _ssd1306_transmit_cmd(0x10 | ((_x & 0xF0) >> 4));
  _ssd1306_transmit_cmd(0x00 | (_x & 0x0F));
}

void _ssd1306_fill(unsigned char _data) {
  uint8_t k, n;
  for (k = 0; k < 8; k++) {
    _ssd1306_transmit_cmd(0xb0 + k);
    _ssd1306_transmit_cmd(0x00);
    _ssd1306_transmit_cmd(0x10);
    for (n = 0; n < 128; n++)
      _ssd1306_transmit_data(_data, 1);
  }
}

void oledInit() {
  uint32_t i, j;

  OLED_RST_Clr();
  HAL_Delay(200);
  OLED_RST_Set();

  _ssd1306_transmit_cmd(0xAE);                        /* display off */
  _ssd1306_transmit_cmd(0x00); /*set lower column address*/
  _ssd1306_transmit_cmd(0x10);/*set higher column address*/
  _ssd1306_transmit_cmd(0xB0); /*set page address*/
  _ssd1306_transmit_cmd(0x40); /*set display start lines*/
  _ssd1306_transmit_cmd(0x81); /*contract control*/
  _ssd1306_transmit_cmd(0x88); /*4d*/
  _ssd1306_transmit_cmd(0x82); /* iref resistor set and adjust ISEG*/
  _ssd1306_transmit_cmd(0x00);
  _ssd1306_transmit_cmd(0xA1); /*set segment remap 0xA0*/
  _ssd1306_transmit_cmd(0xA2); /*set seg pads hardware configuration*/
  _ssd1306_transmit_cmd(0xA4); /*Disable Entire Display On (0xA4/0xA5)*/
  _ssd1306_transmit_cmd(0xA6); /*normal / reverse*/
  _ssd1306_transmit_cmd(0xA8); /*multiplex ratio*/
  _ssd1306_transmit_cmd(0x3F); /*duty = 1/64*/
  _ssd1306_transmit_cmd(0xC8); /*Com scan direction 0XC0*/
  _ssd1306_transmit_cmd(0xD3); /*set display offset*/
  _ssd1306_transmit_cmd(0x00); /* */
  _ssd1306_transmit_cmd(0xD5); /*set osc division*/
  _ssd1306_transmit_cmd(0xa0);
  _ssd1306_transmit_cmd(0xD9); /*set pre-charge period*/
  _ssd1306_transmit_cmd(0x22);
  _ssd1306_transmit_cmd(0xdb); /*set vcomh*/
  _ssd1306_transmit_cmd(0x40);
  _ssd1306_transmit_cmd(0x31); /* Set pump 7.4v */
  _ssd1306_transmit_cmd(0xad); /*set charge pump enable*/
  _ssd1306_transmit_cmd(0x8b); /*Set DC-DC enable (0x8a=disable; 0x8b=enable) */

  _ssd1306_fill(0);

  _ssd1306_transmit_cmd(0xAF);                        /* display on */
}

u8g2_t u8g2;

uint8_t key1Cnt = 0;
uint8_t key2Cnt = 0;

void key1Clicked() { key1Cnt++; }

void key2Clicked() { key2Cnt++; }

void key1Pressed() { key1Cnt = 0; }

void key2Pressed() { key2Cnt = 0; }

uint16_t voltageADC = 0;
uint16_t currentADC = 0;

//卡尔曼滤波
double kalmanFilter(double inData)
{
  static double prevData = 0;                                 //先前数值
  static double p = 10;
  static double q = 0.0005;
  static double r = 0.01;
  static double kGain = 0;      // q控制误差  r控制响应速度

  p = p + q;
  kGain = p / ( p + r );                                     //计算卡尔曼增益
  inData = prevData + ( kGain * ( inData - prevData ) );     //计算本次滤波估计值
  p = ( 1 - kGain ) * p;                                     //更新测量方差
  prevData = inData;
  return inData;                                             //返回滤波值
}

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

  oledInit();
  u8g2Init(&u8g2);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1) {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    HAL_ADC_Start(&hadc1);
    HAL_ADC_PollForConversion(&hadc1, 50);   //等待转换完成，50为最大等待时间，单位为ms
    HAL_ADC_Start(&hadc2);
    HAL_ADC_PollForConversion(&hadc2, 50);   //等待转换完成，50为最大等待时间，单位为ms

    u8g2_ClearBuffer(&u8g2);
    u8g2_SetFont(&u8g2, u8g2_font_Cascadia);
    u8g2_DrawUTF8(&u8g2, 0, 20, "Hello, ");
    u8g2_DrawUTF8(&u8g2, 0, 40, "World!");
    u8g2_SetFont(&u8g2, u8g2_font_myfont);
    u8g2_DrawUTF8(&u8g2, 102, 60, "测试");

    char voltageChar[10];
    sprintf(voltageChar, "%.3f", (voltageADC * 3.3f / 4095.0) * 6 - 0.15);
    u8g2_DrawStr(&u8g2, 88, 10, voltageChar);
    u8g2_DrawStr(&u8g2, 119, 10, "V");

    char currentChar[10];
    sprintf(currentChar, "%.3f", (currentADC * 3.3f / 4095.0) / 5 - 0.01);
    u8g2_DrawStr(&u8g2, 88, 20, currentChar);
    u8g2_DrawStr(&u8g2, 119, 20, "A");

    char pwrChar[10];
    sprintf(pwrChar, "%.3f", ((voltageADC * 3.3f / 4095.0) * 6 - 0.15) * (((currentADC * 3.3f / 4095.0) / 5) - 0.01));
    u8g2_DrawStr(&u8g2, 88, 30, pwrChar);
    u8g2_DrawStr(&u8g2, 118, 30, "W");

    keyCallBack(2, key1Clicked, key2Clicked, key1Pressed, key2Pressed);
    char key1CntChar[10];
    char key2CntChar[10];
    sprintf(key1CntChar, "%d", key1Cnt);
    sprintf(key2CntChar, "%d", key2Cnt);
    u8g2_DrawStr(&u8g2, 0, 60, "key1: ");
    u8g2_DrawStr(&u8g2, 28, 60, key1CntChar);
    u8g2_DrawStr(&u8g2, 40, 60, " key2: ");
    u8g2_DrawStr(&u8g2, 74, 60, key2CntChar);

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
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV2;
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

