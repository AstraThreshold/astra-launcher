//
// Created by Fir on 24-11-30.
//

#include "astra_ui_item.h"

void astra_draw_pop_window(uint16_t _y, const char *_content) {
  oled_set_font(u8g2_font_wqy12_t_chinese1);
  static uint8_t _width = 0;
  _width = oled_get_UTF8_width(_content) + 8;

  oled_set_draw_color(0); //黑遮罩打底
  oled_draw_R_box(OLED_WIDTH/2 - (_width + 4)/2, _y - 2, _width + 4, POP_HEIGHT + 4, 3);

  oled_set_draw_color(1);
  oled_draw_R_frame(OLED_WIDTH/2 - _width/2, _y, _width, POP_HEIGHT, 3);

  oled_draw_UTF8(OLED_WIDTH/2 - _width/2 + 4, _y + oled_get_str_height() - 1, _content);
}
