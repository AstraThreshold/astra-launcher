//
// Created by forpaindream on 25-1-20.
//

#include "astra_ui_drawer.h"

void astra_draw_info_bar() {
  //弹窗到位后才开始计算时间
  if (astra_info_bar.y_info_bar == astra_info_bar.y_info_bar_trg) astra_info_bar.time = launcher_get_tick_ms();

  if (astra_info_bar.time - astra_info_bar.time_start >= astra_info_bar.span && astra_info_bar.is_running)
  {
    astra_info_bar.y_info_bar_trg = 0 - 2 * POP_HEIGHT; //收回
    astra_info_bar.is_running = false;
  }

  oled_set_font(u8g2_font_wqy12_t_chinese1);
  oled_set_draw_color(0); //黑遮罩打底
  oled_draw_R_box(OLED_WIDTH/2 - (astra_info_bar.w_info_bar + 4)/2, astra_info_bar.y_info_bar - 2, astra_info_bar.w_info_bar + 4, POP_HEIGHT + 4, 3);
  oled_draw_box(OLED_WIDTH/2 - (astra_info_bar.w_info_bar + 4)/2, astra_info_bar.y_info_bar - 6, astra_info_bar.w_info_bar + 4, 4); //上半部分直角矩形

  oled_set_draw_color(1);
  oled_draw_R_frame(OLED_WIDTH/2 - astra_info_bar.w_info_bar/2, astra_info_bar.y_info_bar - 4, astra_info_bar.w_info_bar, POP_HEIGHT + 4, 3);
  //向上移动四个像素 同时向下多画四个像素 只用下半部分圆角

  oled_draw_UTF8(OLED_WIDTH/2 - astra_info_bar.w_info_bar/2 + 4, astra_info_bar.y_info_bar + oled_get_str_height() - 1, astra_info_bar.content);
}

void astra_draw_canvas() {
  //需要调用所有的draw函数
  astra_draw_info_bar();
}