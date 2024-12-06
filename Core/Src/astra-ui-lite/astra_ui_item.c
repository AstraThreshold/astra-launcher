//
// Created by Fir on 24-11-30.
//

#include <stdio.h>
#include <string.h>
#include "astra_ui_item.h"

int16_t terminal_x_min = 0;
int16_t terminal_y_min = 0;
int16_t terminal_x_max = 0;
int16_t terminal_y_max = 0;
bool terminal_area_is_ok = false;

uint8_t terminal_cnt = 0;

char terminal_buffer[TERMINAL_BUFFER_SIZE + 1] = {'\0'};
size_t buffer_len = 0;
size_t buffer_free_space = TERMINAL_BUFFER_SIZE;

void astra_draw_status_box(uint8_t _x, uint8_t _y, astra_status_t _status) {
  oled_set_font(u8g2_font_spleen5x8_mr);
  if (_status == OK) {
    oled_draw_R_box(_x, _y, oled_get_str_width("-OK-") + 3, 10, 1);
    oled_set_draw_color(2);
    oled_draw_str(_x + 2, _y + 8, "-OK-");
    oled_set_draw_color(1);
  } else if (_status == Ready) {
    oled_draw_R_box(_x, _y, oled_get_str_width("-Ready-") + 3, 10, 1);
    oled_set_draw_color(2);
    oled_draw_str(_x + 2, _y + 8, "-Ready-");
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

void astra_draw_status_bar(uint8_t _y,
                           astra_status_t _status,
                           double _data1,
                           char *_unit1,
                           double _data2,
                           char *_unit2) {
  oled_draw_H_line(2, _y, OLED_WIDTH - 4);
  astra_draw_data(OLED_WIDTH - 72, _y - 3, _data1, _unit1);
  astra_draw_data(OLED_WIDTH - 36, _y - 3, _data2, _unit2);

  astra_draw_status_box(2, _y - 11, _status);
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

//这部分测试没问题 彻底的
void astra_add_str_to_terminal_buffer(astra_terminal_prompter_t _type, char *_str) {
  buffer_len = strlen(terminal_buffer);
  buffer_free_space = TERMINAL_BUFFER_SIZE - buffer_len;
  size_t _str_len = strlen(_str);

  //如果传进来的_str长度大于最大的缓冲区长度 则直接取_str的后TERMINAL_BUFFER_SIZE长度 覆盖掉原来的缓冲区内容
  if (_str_len > TERMINAL_BUFFER_SIZE) {
    memset(terminal_buffer, '\0', TERMINAL_BUFFER_SIZE);
    memcpy(terminal_buffer, _str + (_str_len - TERMINAL_BUFFER_SIZE), TERMINAL_BUFFER_SIZE);
    buffer_len = strlen(terminal_buffer);
    buffer_free_space = TERMINAL_BUFFER_SIZE - buffer_len;
    return;
  }

  //如果buffer剩余空间小于_str长度 则将buffer向前移动sizeof(_str)-剩余空间为_str腾出空间
  if (_str_len > buffer_free_space) {
    memmove(terminal_buffer, terminal_buffer + (_str_len - buffer_free_space), TERMINAL_BUFFER_SIZE - _str_len);
    memset(terminal_buffer + (TERMINAL_BUFFER_SIZE - _str_len), '\0', _str_len);
  }

  // 添加新字符串到缓冲区
  strcat(terminal_buffer, _str);
  buffer_len = strlen(terminal_buffer);
  buffer_free_space = TERMINAL_BUFFER_SIZE - buffer_len;
}

//todo 把buffer的内容分割成行 不断用getUTF8Width和max的大小关系来试探能打印的最多字符 然后一次性打印一整行 遇到换行符就切断改行
//在循环里执行
void astra_refresh_terminal_buffer() {
  static int16_t _x, _y = 0;
  static int16_t _camera_y = 0; //一直是负数或者0 这样y+camera才是正确的
  static uint8_t _line_cnt = 0;
  for (size_t i = 0; i < strlen(terminal_buffer); i++) {
    if (_y + _camera_y > terminal_y_max) {
      _camera_y -= LINE_HEIGHT;
      _line_cnt++;
    }

  }
}
