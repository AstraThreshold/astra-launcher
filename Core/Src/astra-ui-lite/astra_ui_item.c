//
// Created by Fir on 24-11-30.
//

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include "astra_ui_item.h"

int16_t terminal_x_min = 0;
int16_t terminal_y_min = 0;
int16_t terminal_x_max = 0;
int16_t terminal_y_max = 0;

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
  } else if (_status == Stop) {
    oled_draw_R_box(_x, _y, oled_get_str_width("-Stop-") + 3, 10, 1);
    oled_set_draw_color(2);
    oled_draw_str(_x + 2, _y + 8, "-Stop-");
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

void astra_draw_terminal_prompter(uint8_t _x, uint8_t _y, astra_terminal_prompter_t _type) {
  static char _str[5] = {'\0'};
  if (_type == info) strcpy(_str, "info");
  else if (_type == uart) strcpy(_str, "uart");
  else if (_type == f411) strcpy(_str, "f411");
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
}

void animation(int16_t *_pos, int16_t _posTrg, int16_t _speed) {
  if (*_pos != _posTrg) {
    if (abs(*_pos - _posTrg) <= 1) *_pos = _posTrg;
    else *_pos += round(_posTrg - *_pos) / (100 - _speed);
  }
}

//buffer链表的各种函数不需要传入头指针 头指针只有一个
terminal_buffer_t *terminal_buffer_head = NULL;

//测试ok
void push_str_to_terminal(astra_terminal_prompter_t _type, char *_str) {
  static uint16_t _terminal_buffer_size = 0;

  //最高容纳一百条记录
  if (_terminal_buffer_size > 100) {
    terminal_buffer_pop_front();
    _terminal_buffer_size--;
  }

  if (terminal_buffer_head == NULL) {
    terminal_buffer_head = (terminal_buffer_t *) malloc(sizeof(terminal_buffer_t));
    terminal_buffer_head->type = _type;
    terminal_buffer_head->str = _str;
    terminal_buffer_head->next = NULL;
    _terminal_buffer_size = 1;
    return;
  }

  terminal_buffer_t *_node = (terminal_buffer_t *) malloc(sizeof(terminal_buffer_t));
  terminal_buffer_t *_p = terminal_buffer_head;
  while (_p->next != NULL) _p = _p->next; //找到末端
  _p->next = _node;
  _node->type = _type;
  _node->str = _str;
  _node->next = NULL;
  _terminal_buffer_size++;
}

void terminal_buffer_pop_front() {
  if (terminal_buffer_head == NULL) return;
  terminal_buffer_t *_old = terminal_buffer_head;
  terminal_buffer_head = terminal_buffer_head->next;
  free(_old);
  _old = NULL;
}

/*
 * todo 除了camera其他都ok了
 * 每个节点存储一句话
 * 链表的话就有两个坐标的概念 一个是每句话的起始坐标 一个是每句话里面每个字的坐标
 * 第二个坐标以第一个坐标作为初值开始变化
 * 遍历新节点的时候 文字坐标换行要换到指示器后面的位置
 * 但是绘制每句话的时候 换行逻辑不变
 * 链表既然没有“末端”这一概念 也就不会卡行 每次执行绘制函数时将当前的第一段字的起始坐标赋给最小坐标
 */
void terminal_print_test() {
  terminal_buffer_t *_node = terminal_buffer_head;
  static int16_t _x_node, _y_node = 0; //每句话的起始坐标
  static int16_t _x_str, _y_str = 0;  //每句话里面每个字的坐标
  static int16_t _y_camera = 0; //一直是负数或者0 这样y+camera才是正确的
  static int16_t _y_camera_trg = 0; //一直是负数或者0 这样y+camera才是正确的
  static int16_t _node_cnt = 0; //代表当前节点数 从0开始
  static int16_t _line_cnt = 0; //代表当前行数 从0开始
  static int16_t _line_offset_cnt = 0;
  static char _str_to_print[3] = {'\0'};

  _node_cnt = 0;
  _line_cnt = 0;
  _x_node = terminal_x_min + 26; //加上指示器宽度
  _y_node = terminal_y_min;
  _x_str = _x_node;
  _y_str = _y_node;

  while (_node != NULL) {
    astra_draw_terminal_prompter(4, (terminal_y_min - 1) + LINE_HEIGHT * _line_cnt + _y_camera, _node->type);
    _x_node = terminal_x_min + 26;
    _y_node = terminal_y_min + (_line_cnt) * LINE_HEIGHT;
    _x_str = _x_node;
    _y_str = _y_node;
    for (int i = 0; _node->str[i] != '\0'; i++) {
      oled_set_font(u8g2_font_wqy12_t_chinese1);
      if (_node->str[i] == '\n') {
        _line_cnt++;
        _x_str = terminal_x_min;
        _y_str += LINE_HEIGHT;
        continue;
      }

      if (_node->str[i] == '\r') {
        _x_str += 4;
        continue;
      }

      if ((_node->str[i] & 0xF0) == 0xE0) { //如果当前字符是中文字符
        _str_to_print[0] = _node->str[i];
        _str_to_print[1] = _node->str[i + 1];
        _str_to_print[2] = _node->str[i + 2];

        if (_x_str >= terminal_x_max - 22) {
          _x_str = terminal_x_min, _y_str += LINE_HEIGHT, _line_cnt++;
          if (_y_str > terminal_y_max + (_line_offset_cnt * LINE_HEIGHT)) _line_offset_cnt++;
        }

        oled_draw_UTF8(_x_str, _y_str + _y_camera, _str_to_print);
        _x_str += oled_get_UTF8_width(_str_to_print);
        i += 2;
      } else {
        _str_to_print[0] = _node->str[i];
        _str_to_print[1] = '\0';
        _str_to_print[2] = '\0';

        if (_x_str >= terminal_x_max - 10) {
          _x_str = terminal_x_min, _y_str += LINE_HEIGHT, _line_cnt++;
          if (_y_str > terminal_y_max + (_line_offset_cnt * LINE_HEIGHT)) _line_offset_cnt++;
        }

        oled_draw_str(_x_str, _y_str + _y_camera, _str_to_print);
        _x_str += oled_get_str_width(_str_to_print) + 1;
      }
    }

    _y_node += (_node_cnt) * LINE_HEIGHT;
    if (_y_node > terminal_y_max + (_line_offset_cnt * LINE_HEIGHT)) _line_offset_cnt++;
    _x_node = terminal_x_min + 26;

    _node_cnt++;
    _line_cnt++; //换了新节点必定要换行
    _node = _node->next;
  }


//  //在开始绘制所有字之前 先清零坐标
//  _x = terminal_x_min + 26; //加上指示器宽度
//  _y = terminal_y_min;
//  _line_offset_cnt = 0;
//  while (_p != NULL) {
//    astra_draw_terminal_prompter(4, (terminal_y_min - 1), _p->type);
//    oled_set_font(u8g2_font_wqy12_t_chinese1);
//    for (int i = 0; _p->str[i] != '\0'; i++) {
//      if (_p->str[i] == '\n') {
//        _x = terminal_x_min;
//        _y += LINE_HEIGHT;
//        if (_y > terminal_y_max + (_line_offset_cnt * LINE_HEIGHT)) _line_offset_cnt++;
//        continue;
//      }
//
//      if (_p->str[i] == '\r') {
//        _x += 4;
//        continue;
//      }
//
//      if ((_p->str[i] & 0xF0) == 0xE0) { //如果当前字符是中文字符
//        _str_to_print[0] = _p->str[i];
//        _str_to_print[1] = _p->str[i + 1];
//        _str_to_print[2] = _p->str[i + 2];
//        oled_draw_UTF8(_x, _y + _camera_y, _str_to_print);
//        if (_x >= terminal_x_max - 22) {
//          _x = terminal_x_min, _y += LINE_HEIGHT;
//          if (_y > terminal_y_max + (_line_offset_cnt * LINE_HEIGHT)) _line_offset_cnt++;
//        }
//        else _x += oled_get_UTF8_width(_str_to_print);
//        i += 2;
//      } else {
//        _str_to_print[0] = _p->str[i];
//        _str_to_print[1] = '\0';
//        _str_to_print[2] = '\0';
//        oled_draw_str(_x, _y + _camera_y, _str_to_print);
//        if (_x >= terminal_x_max - 10) {
//          _x = terminal_x_min, _y += LINE_HEIGHT;
//          if (_y > terminal_y_max + (_line_offset_cnt * LINE_HEIGHT)) _line_offset_cnt++;
//        }
//        else _x += oled_get_str_width(_str_to_print) + 1;
//      }
//    }
//
//    _p = _p->next;
//  }
}
