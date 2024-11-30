//
// Created by Fir on 24-11-30.
//

#include <stdio.h>
#include "astra_ui_item.h"

uint8_t terminal_x_min = 0;
uint8_t terminal_y_min = 0;
uint8_t terminal_x_max = 0;
uint8_t terminal_y_max = 0;
bool terminal_area_is_ok = false;

void astra_draw_status_box(uint8_t _x, uint8_t _y, astra_status_t _status) {
  oled_set_font(u8g2_font_spleen5x8_mr);
  if (_status == OK) {
    oled_draw_R_box(_x, _y, oled_get_str_width("-OK-") + 3, 10, 1);
    oled_set_draw_color(2);
    oled_draw_str(_x + 2, _y + 8, "-OK-");
    oled_set_draw_color(1);
  } else if (_status == Wait) {
    oled_draw_R_box(_x, _y, oled_get_str_width("-Wait-") + 3, 10, 1);
    oled_set_draw_color(2);
    oled_draw_str(_x + 2, _y + 8, "-Wait-");
    oled_set_draw_color(1);
  } else if (_status == Loading) {
    oled_draw_R_box(_x, _y, oled_get_str_width("-Loading-") + 3, 10, 1);
    oled_set_draw_color(2);
    oled_draw_str(_x + 2, _y + 8, "-Loading-");
    oled_set_draw_color(1);
  } else if (_status == Err) {
    oled_draw_R_box(_x, _y, oled_get_str_width("-Err-") + 3, 10, 1);
    oled_set_draw_color(2);
    oled_draw_str(_x + 2, _y + 8, "-Err-");
    oled_set_draw_color(1);
  }
}

void astra_draw_data(uint8_t _x, uint8_t _y, double _data, char *_unit) {
  oled_set_draw_color(1);
  char _data_char[10];
  sprintf(_data_char, "%05.2f", _data);
  oled_set_font(u8g2_font_spleen5x8_mr);
  oled_set_draw_color(1);
  oled_draw_str(_x, _y, _data_char);
  oled_draw_R_box(_x + 26, _y - 7, 8, 8, 1);
  oled_set_draw_color(2);
  oled_draw_str(_x + 28, _y, _unit);
  oled_set_draw_color(1);
}

void astra_draw_status_bar(uint8_t _y, astra_status_t _status, double _data1, char *_unit1, double _data2, char *_unit2) {
  oled_draw_H_line(2, _y, OLED_WIDTH - 4);
  astra_draw_data(OLED_WIDTH - 72, _y - 3, _data1, _unit1);
  astra_draw_data(OLED_WIDTH - 36, _y - 3, _data2, _unit2);

  astra_draw_status_box(2, _y - 11, Loading);
}

void astra_draw_terminal_prompter(uint8_t _x, uint8_t _y, char *_str) {
  oled_set_font(u8g2_font_spleen5x8_mr);
  oled_draw_R_box(_x, _y - 8, oled_get_str_width(_str), 10, 1);
  oled_set_draw_color(2);
  oled_draw_str(_x + 1, _y, _str);
  oled_set_draw_color(1);
  oled_draw_V_line(_x + oled_get_str_width(_str), _y - 6, 6);
  oled_draw_V_line(_x + oled_get_str_width(_str) + 1, _y - 5, 4);
  oled_draw_V_line(_x + oled_get_str_width(_str) + 2, _y - 4, 2);
}

void astra_draw_cursor(uint8_t _x, uint8_t _y, uint32_t _tick, uint8_t _blink_freq) {

}

//该边界为终端边框最内侧的边界所在坐标
void astra_set_terminal_area(uint8_t _x_min, uint8_t _y_min, uint8_t _x_max, uint8_t _y_max) {
  terminal_x_min = _x_min;
  terminal_y_min = _y_min;
  terminal_x_max = _x_max;
  terminal_y_max = _y_max;
  terminal_area_is_ok = true;
}

bool astra_print_on_terminal(char *_type, char *_str) {
  if (!terminal_area_is_ok) return false;
  static uint8_t _current_x, _current_y;
  static uint8_t _current_time = 0;

  if (_current_time == 0) {
    _current_x = terminal_x_min + 2;
    _current_y = terminal_y_min + 2;
    oled_set_font(u8g2_font_wqy12_t_chinese1);
  }


  return true;
}