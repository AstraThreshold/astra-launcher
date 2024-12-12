//
// Created by Fir on 24-12-7.
//

#include "launcher_home_page.h"

/**
 * @warning 该函数要在终端字体绘制之后调用
 */
void launcher_draw_home_page() {
  oled_set_font(u8g2_font_wqy12_t_chinese1);

  /*后景文字部分*/
  oled_set_draw_color(1);
  terminal_print_test();
  /*后景文字部分*/

  /*前景遮罩部分*/
  oled_set_draw_color(0);
  oled_draw_box(0, 0, 128, 16); //上遮罩
  oled_draw_box(0, 61, 128, 3); //下遮罩
  oled_draw_box(0, 0, 3, 64); //左遮罩
  oled_draw_box(125, 0, 3, 64); //右遮罩
  oled_draw_pixel(3, 16); //左上圆角像素
  oled_draw_pixel(3, 60); //左下圆角像素
  oled_draw_pixel(124, 16); //右上圆角像素
  oled_draw_pixel(124, 60); //右下圆角像素
  /*前景遮罩部分*/

  /*前前景status box + ui部分*/
  oled_set_draw_color(1);
  astra_draw_status_bar(12, Stop, (volADC1 + volADC2) / 2, "V", iBaseADC1, "A");
  oled_draw_H_line(2, 14, 124); //上横线
  oled_draw_H_line(1, 15, 2); //左上圆角
  oled_draw_box(0, 16, 2, 48); //左边线
  oled_draw_pixel(2, 61); //左下圆角像素
  oled_draw_H_line(125, 15, 2); //右上圆角
  oled_draw_box(126, 16, 2, 48); //右边线
  oled_draw_pixel(125, 61); //右下圆角像素
  oled_draw_box(0, 62, 128, 2); //下横线

  oled_set_draw_color(2);
  oled_draw_H_line(56, 63, 66); //下方logo同步条
  oled_draw_V_line(124, 18, 20);  //进度条
  /*前前景status box + ui部分*/
}