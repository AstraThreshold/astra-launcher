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

extern u8g2_t u8g2;

/* 此处修改oled绘制函数 */
#define OLED_HEIGHT 64
#define OLED_WIDTH 128

#define delay(ms) HAL_Delay(ms)
#define oled_set_font(font) u8g2_SetFont(&u8g2, font)
#define oled_draw_str(x, y, str) u8g2_DrawStr(&u8g2, x, y, str)
#define oled_draw_UTF8(x, y, str) u8g2_DrawUTF8(&u8g2, x, y, str)
#define oled_get_str_width(str) u8g2_GetStrWidth(&u8g2, str)
#define oled_get_str_height() u8g2_GetMaxCharHeight(&u8g2)
#define oled_draw_pixel(x, y) u8g2_DrawPixel(&u8g2, x, y)
#define oled_draw_R_box(x, y, w, h, r) u8g2_DrawRBox(&u8g2, x, y, w, h, r)
#define oled_draw_box(x, y, w, h) u8g2_DrawBox(&u8g2, x, y, w, h)
#define oled_draw_frame(x, y, w, h) u8g2_DrawFrame(&u8g2, x, y, w, h)
#define oled_draw_R_frame(x, y, w, h, r) u8g2_DrawRFrame(&u8g2, x, y, w, h, r)
#define oled_draw_H_line(x, y, l) u8g2_DrawHLine(&u8g2, x, y, l)
#define oled_draw_V_line(x, y, h) u8g2_DrawVLine(&u8g2, x, y, h)
#define oled_draw_line(x1, y1, x2, y2) u8g2_DrawLine(&u8g2, x1, y1, x2, y2)
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

extern void astra_ui_driver_init();

#endif //FUCKCLION_CORE_SRC_ASTRA_UI_LITE_DRAW_DRIVER_H_
