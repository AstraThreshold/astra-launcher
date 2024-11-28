//
// Created by Fir on 24-11-28.
//

#include "oled_launcher.h"


uint8_t u8x8_byte_4wire_hw_spi(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr) {
  uint8_t *p = (uint8_t *) arg_ptr;
  switch (msg) {
    /*通过SPI发�?�arg_int个字节数�??*/
    case U8X8_MSG_BYTE_SEND:
      for (int i = 0; i < arg_int; i++)
        HAL_SPI_Transmit(&hspi2,
                         (const uint8_t *) (p + i),
                         1,
                         1000);
      break;

      /*设置DC引脚，DC引脚控制发�?�的是数据还是命�??*/
    case U8X8_MSG_BYTE_SET_DC:
      if (arg_int) OLED_DC_Set();
      else
        OLED_DC_Clr();
      break;

      /* 下面功能无需定义 */

      /*�??始传输前会进行的操作，如果使用软件片选可以在这里进行控制*/
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
    case U8X8_MSG_GPIO_AND_DELAY_INIT: /*delay和GPIO的初始化，在main中已经初始化完成�?????*/
      break;
    case U8X8_MSG_DELAY_MILLI: /*延时函数*/
      HAL_Delay(arg_int);
      break;
    case U8X8_MSG_GPIO_CS: /*片�?�信�?????*/
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
                                     u8x8_stm32_gpio_and_delay);  // 初始�????? u8g2 结构�?????
  u8g2_InitDisplay(u8g2); // 根据�?????选的芯片进行初始化工作，初始化完成后，显示器处于关闭状�??
  u8g2_SetPowerSave(u8g2, 0); // 打开显示�?????
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