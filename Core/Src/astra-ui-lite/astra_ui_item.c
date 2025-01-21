//
// Created by Fir on 24-11-30.
//

#include "astra_ui_item.h"

#include <string.h>

#include "../astra-launcher/launcher_delay.h"

astra_info_bar_t astra_info_bar = {0, 1, 0 - 5 * POP_HEIGHT, 0, 10, 10, false, 0, 1};

// void astra_draw_pop_window(const char *_content) {
//   oled_set_font(u8g2_font_wqy12_t_chinese1);
//   static uint8_t _width = 0;
//   _width = oled_get_UTF8_width(_content) + 8;
//
//   oled_set_draw_color(0); //黑遮罩打底
//   oled_draw_R_box(OLED_WIDTH/2 - (_width + 4)/2, _y - 2, _width + 4, POP_HEIGHT + 4, 3);
//
//   oled_set_draw_color(1);
//   oled_draw_R_frame(OLED_WIDTH/2 - _width/2, _y, _width, POP_HEIGHT, 3);
//
//   oled_draw_UTF8(OLED_WIDTH/2 - _width/2 + 4, _y + oled_get_str_height() - 1, _content);
// }

void astra_push_info_bar(char *_content, const uint16_t _span)
{
  //设定显示时间的概念，超过了显示时间，就将ytrg设为初始位置，如果在显示时间之内，有新的消息涌入，则y和ytrg都不变，继续显示，且显示时间清零
  //只有显示时间到了的时候，才会复位

  astra_info_bar.content = _content;
  astra_info_bar.span = _span;
  astra_info_bar.is_running = false; //每次进入该函数都代表有新的消息涌入，所以需要重置is_running

  //展开弹窗 收回弹窗和同步时间戳需要在循环中进行 所以移到了drawer中
  if (!astra_info_bar.is_running)
  {
    astra_info_bar.time_start = launcher_get_tick_ms();
    astra_info_bar.y_info_bar_trg = 0;
    astra_info_bar.is_running = true;
  }

  oled_set_font(u8g2_font_wqy12_t_chinese1);
  astra_info_bar.w_info_bar_trg = oled_get_UTF8_width(astra_info_bar.content) + 10;
}
