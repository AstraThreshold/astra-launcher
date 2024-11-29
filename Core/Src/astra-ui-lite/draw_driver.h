//
// Created by Fir on 24-11-29.
//

#ifndef FUCKCLION_CORE_SRC_ASTRA_UI_LITE_DRAW_DRIVER_H_
#define FUCKCLION_CORE_SRC_ASTRA_UI_LITE_DRAW_DRIVER_H_

/* 此处自行添加头文件 */
#include "../u8g2/u8g2.h"
#include "main.h"
#include "spi.h"
/* 此处自行添加头文件 */

u8g2_t u8g2;

/* 此处修改oled绘制函数 */
#define delay(ms) HAL_Delay(ms)
#define oled_set_font(font) u8g2_SetFont(&u8g2, font)
#define oled_draw_str(x, y, str) u8g2_DrawStr(&u8g2, x, y, str)
#define oled_draw_UTF8(x, y, str) u8g2_DrawUTF8(&u8g2, x, y, str)
#define oled_draw_pixel(x, y) u8g2_DrawPixel(&u8g2, x, y)
#define oled_draw_R_box(x, y, w, h, r) u8g2_DrawRBox(&u8g2, x, y, w, h, r)
#define oled_draw_box(x, y, w, h) u8g2_DrawBox(&u8g2, x, y, w, h)
#define oled_draw_frame(x, y, w, h) u8g2_DrawFrame(&u8g2, x, y, w, h)
#define oled_draw_R_frame(x, y, w, h, r) u8g2_DrawRFrame(&u8g2, x, y, w, h, r)
#define oled_draw_H_line(x, y, l) u8g2_DrawHLine(&u8g2, x, y, l)
#define oled_draw_V_line(x, y, h) u8g2_DrawVLine(&u8g2, x, y, h)
#define oled_draw_H_dotted_line(x, y, l) u8g2_DrawHDottedLine(&u8g2, x, y, l)
#define oled_draw_V_dotted_line(x, y, h) u8g2_DrawVDottedLine(&u8g2, x, y, h)
#define oled_draw_bMP(x, y, w, h, bitMap) u8g2_DrawBMP(&u8g2, x, y, w, h, bitMap)
#define oled_set_draw_color(color) u8g2_SetDrawColor(&u8g2, color)
#define oled_set_font_mode(mode) u8g2_SetFontMode(&u8g2, mode)
#define oled_set_font_direction(dir) u8g2_SetFontDirection(&u8g2, dir)
#define oled_draw_pixel(x, y) u8g2_DrawPixel(&u8g2, x, y)
#define oled_clear_buffer() u8g2_ClearBuffer(&u8g2)
#define oled_send_buffer() u8g2_SendBuffer(&u8g2)
/* 此处修改oled绘制函数 */

/* 此处自行编写oled及图形库初始化函数所需的函数 */
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
    case U8X8_MSG_BYTE_SET_DC:
      if (arg_int) HAL_GPIO_WritePin(OLED_DC_GPIO_Port, OLED_DC_Pin, GPIO_PIN_SET);
      else
        HAL_GPIO_WritePin(OLED_DC_GPIO_Port, OLED_DC_Pin, GPIO_PIN_RESET);
      break;
    case U8X8_MSG_BYTE_START_TRANSFER:break;
    case U8X8_MSG_BYTE_END_TRANSFER:break;
    default:return 0;
  }
  return 1;
}

uint8_t u8x8_stm32_gpio_and_delay(U8X8_UNUSED u8x8_t *u8x8,
                                  U8X8_UNUSED uint8_t msg, U8X8_UNUSED uint8_t arg_int,
                                  U8X8_UNUSED void *arg_ptr) {
  switch (msg) {
    case U8X8_MSG_GPIO_AND_DELAY_INIT:
      break;
    case U8X8_MSG_DELAY_MILLI:
      HAL_Delay(arg_int);
      break;
    case U8X8_MSG_GPIO_CS:
      break;
    case U8X8_MSG_GPIO_DC:
    case U8X8_MSG_GPIO_RESET:
    default: break;
  }
  return 1;
}

void _ssd1306_transmit_cmd(unsigned char _cmd) { //NOLINT
  HAL_GPIO_WritePin(OLED_DC_GPIO_Port, OLED_DC_Pin, GPIO_PIN_RESET);
  HAL_SPI_Transmit_DMA(&hspi2, &_cmd, 1);
}

void _ssd1306_transmit_data(unsigned char _data, unsigned char _mode) { //NOLINT
  if (!_mode) _data = ~_data;
  HAL_GPIO_WritePin(OLED_DC_GPIO_Port, OLED_DC_Pin, GPIO_PIN_SET);
  HAL_SPI_Transmit_DMA(&hspi2, &_data, 1);
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

void oled_init() {
  uint32_t i, j;
  HAL_GPIO_WritePin(OLED_RST_GPIO_Port, OLED_RST_Pin, GPIO_PIN_RESET);
  HAL_Delay(200);
  HAL_GPIO_WritePin(OLED_RST_GPIO_Port, OLED_RST_Pin, GPIO_PIN_SET);
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

void u8g2_init(u8g2_t *u8g2) {
  u8g2_Setup_ssd1306_128x64_noname_f(u8g2,
                                     U8G2_R0,
                                     u8x8_byte_4wire_hw_spi,
                                     u8x8_stm32_gpio_and_delay);
  u8g2_InitDisplay(u8g2);
  u8g2_SetPowerSave(u8g2, 0);
  u8g2_ClearBuffer(u8g2);
  u8g2_SetFontMode(u8g2, 1);
  u8g2_SetFontDirection(u8g2, 0);
  u8g2_SetFont(u8g2, u8g2_font_myfont);
}
/* 此处自行编写oled及图形库初始化函数所需的函数 */

/* 此处自行修改内部函数名 */
void astra_ui_driver_init() {
  oled_init();
  u8g2_init(&u8g2);
}
/* 此处自行修改内部函数名 */










#endif //FUCKCLION_CORE_SRC_ASTRA_UI_LITE_DRAW_DRIVER_H_
