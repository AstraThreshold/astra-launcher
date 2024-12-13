//
// Created by Fir on 24-12-7.
//

#include "launcher_home_page.h"

int16_t terminal_x_min = 0;
int16_t terminal_y_min = 0;
int16_t terminal_x_max = 0;
int16_t terminal_y_max = 0;

int32_t terminal_buffer_size = 0;

void launcher_draw_status_box(uint8_t _x, uint8_t _y, launcher_status_t _status) {
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

void launcher_draw_data(uint8_t _x, uint8_t _y, double _data, char *_unit) {
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

void launcher_draw_status_bar(uint8_t _y,
                              launcher_status_t _status,
                              double _data1,
                              char *_unit1,
                              double _data2,
                              char *_unit2) {
  oled_draw_H_line(2, _y, OLED_WIDTH - 4);
  launcher_draw_data(OLED_WIDTH - 72, _y - 3, _data1, _unit1);
  launcher_draw_data(OLED_WIDTH - 36, _y - 3, _data2, _unit2);

  launcher_draw_status_box(2, _y - 11, _status);
}

void launcher_draw_terminal_prompter(uint8_t _x, uint8_t _y, launcher_terminal_prompter_t _type) {
  static char _str[5] = {'\0'};
  if (_type == info) strcpy(_str, "info");
  else if (_type == uart) strcpy(_str, "uart");
  else if (_type == f411) strcpy(_str, "f411");
  oled_set_font(u8g2_font_spleen5x8_mr);
  oled_set_draw_color(1);
  oled_draw_R_box(_x, _y - 8, oled_get_str_width(_str), 10, 1);
  oled_set_draw_color(2);
  oled_draw_str(_x + 1, _y, _str);
  oled_set_draw_color(1);
  oled_draw_V_line(_x + oled_get_str_width(_str), _y - 6, 6);
  oled_draw_V_line(_x + oled_get_str_width(_str) + 1, _y - 5, 4);
  oled_draw_V_line(_x + oled_get_str_width(_str) + 2, _y - 4, 2);
}

void launcher_draw_cursor(uint8_t _x, uint8_t _y, uint32_t _tick, uint8_t _blink_freq) {

}

//该边界为终端边框最内侧的边界所在坐标
void launcher_set_terminal_area(uint8_t _x_min, uint8_t _y_min, uint8_t _x_max, uint8_t _y_max) {
  terminal_x_min = _x_min;
  terminal_y_min = _y_min;
  terminal_x_max = _x_max;
  terminal_y_max = _y_max;
}

void animation(int16_t *_pos, int16_t _posTrg, int16_t _speed) {
  if (*_pos != _posTrg) {
    if (fabs(*_pos - _posTrg) <= 1.0f) *_pos = _posTrg;
    else *_pos += (_posTrg - *_pos) / ((100 - _speed) / 1.0f);
  }
}

//buffer链表的各种函数不需要传入头指针 头指针只有一个
terminal_buffer_t *terminal_buffer_head = NULL;

//测试ok
void launcher_push_str_to_terminal(launcher_terminal_prompter_t _type, char *_str) {
  //最高容纳一百条记录 todo 坐标错乱来自于内存溢出

  //todo 绘制部分要在这里调用才行 需要重新整合函数

  if (terminal_buffer_head == NULL) {
    terminal_buffer_head = (terminal_buffer_t *) malloc(sizeof(terminal_buffer_t));
    terminal_buffer_head->type = _type;
    terminal_buffer_head->str = _str;
    terminal_buffer_head->next = NULL;
    terminal_buffer_size++;
    return;
  }

  terminal_buffer_t *_node = (terminal_buffer_t *) malloc(sizeof(terminal_buffer_t));
  terminal_buffer_t *_p = terminal_buffer_head;
  while (_p->next != NULL) _p = _p->next; //找到末端
  _p->next = _node;
  _node->type = _type;
  _node->str = _str;
  _node->next = NULL;
  terminal_buffer_size++;
}

void launcher_terminal_buffer_pop_front() {
  if (terminal_buffer_head == NULL) return;
  terminal_buffer_t *_old = terminal_buffer_head;
  terminal_buffer_head = terminal_buffer_head->next;
  free(_old);
  _old = NULL;
}

int16_t _line_offset_cnt = 0;
int16_t _y_camera, _y_camera_trg = 0;

void launcher_refresh_terminal() {
  _y_camera_trg = 0 - (_line_offset_cnt * LINE_HEIGHT);
  animation(&_y_camera, _y_camera_trg, 98);
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
void launcher_terminal_print_test() {
  terminal_buffer_t *_node = terminal_buffer_head;
  static int32_t _x_node, _y_node = 0; //每句话的起始坐标
  static int32_t _x_str, _y_str = 0;  //每句话里面每个字的坐标
  static int16_t _node_cnt = 0; //代表当前节点数 从0开始
  static int16_t _line_cnt = 0; //代表当前行数 从0开始
//  static int16_t _line_offset_cnt = 0;
  static char _str_to_print[3] = {'\0'};

  //todo 现在第一步应该尝试的就是把所有字的坐标都统一成一个坐标加算式的形式 坐标合并
  //todo 同时还需要考虑 for循环绘制到一半的时候来了新的信息 其他数据就更新了的问题
  _node_cnt = 0;
  _line_cnt = 0;
  _line_offset_cnt = 0;
  _x_node = terminal_x_min + 26; //加上指示器宽度
  _y_node = terminal_y_min;
  _x_str = _x_node;
  _y_str = _y_node;
  memset(_str_to_print, '\0', 3);

  //todo 超过buffer阈值之后 就会出现反复 这边刚加进去 那边还没来得及做动画渲染 这边就弹出来了 就又得计算新的绘制位置了 所以会闪

  while (_node != NULL) {
    _x_node = terminal_x_min + 26;
    _y_node = terminal_y_min + (_line_cnt) * LINE_HEIGHT;
    if (_y_node > terminal_y_max + (_line_offset_cnt * LINE_HEIGHT)) _line_offset_cnt++; //由于绘制下一句话导致的滚动
    _x_str = _x_node;
    _y_str = _y_node;

    if (terminal_y_min <= _y_node - 1 <= terminal_y_max)
      launcher_draw_terminal_prompter(4, _y_node + _y_camera - 1, _node->type);

    //todo 其实可以考虑加上如果耦合camera之后 超过了显示范围 就不进行显示

    //绘制一句话中的每个字
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

        if (_x_str >= terminal_x_max - 22) _x_str = terminal_x_min, _y_str += LINE_HEIGHT, _line_cnt++;
        if (_y_str > terminal_y_max + (_line_offset_cnt * LINE_HEIGHT)) _line_offset_cnt++; //由于句子太长导致的滚动

        if (terminal_y_min <= _y_str <= terminal_y_max)
          oled_draw_UTF8(_x_str, _y_str + _y_camera, _str_to_print);
        _x_str += oled_get_UTF8_width(_str_to_print);
        i += 2;
      } else {
        _str_to_print[0] = _node->str[i];
        _str_to_print[1] = '\0';
        _str_to_print[2] = '\0';

        if (_x_str >= terminal_x_max - 10) _x_str = terminal_x_min, _y_str += LINE_HEIGHT, _line_cnt++;
        if (_y_str > terminal_y_max + (_line_offset_cnt * LINE_HEIGHT)) _line_offset_cnt++; //由于句子太长导致的滚动

        if (terminal_y_min <= _y_str <= terminal_y_max)
          oled_draw_str(_x_str, _y_str + _y_camera, _str_to_print);
        _x_str += oled_get_str_width(_str_to_print) + 1;
      }
    }

    _node_cnt++;
    _line_cnt++; //换了新节点必定要换行
    _node = _node->next;
  }
}

/**
 * @warning 该函数要在终端字体绘制之后调用
 */
void launcher_draw_home_page() {
  oled_set_font(u8g2_font_wqy12_t_chinese1);

  /*后景文字部分*/
  oled_set_draw_color(1);

  //设置坐标
  //todo 滚动的时候上面下面各缺失一半 但是如果有溢出pop之后就正常了
  launcher_refresh_terminal();

  //绘制
  launcher_terminal_print_test();

  if (terminal_buffer_size > 10) //溢出处理
    for (int i = 0; i < terminal_buffer_size - 10; i++) launcher_terminal_buffer_pop_front(), terminal_buffer_size--;

  /*后景文字部分*/

  /*前景遮罩部分*/
  oled_set_draw_color(0);
  oled_draw_box(0, 0, 128, 16); //上遮罩
  oled_draw_box(0, 60, 128, 4); //下遮罩
  oled_draw_box(0, 0, 3, 64); //左遮罩
  oled_draw_box(125, 0, 3, 64); //右遮罩
  oled_draw_pixel(3, 16); //左上圆角像素
  oled_draw_pixel(3, 60); //左下圆角像素
  oled_draw_pixel(124, 16); //右上圆角像素
  oled_draw_pixel(124, 60); //右下圆角像素
  /*前景遮罩部分*/

  /*前前景status box + ui部分*/
  oled_set_draw_color(1);
  launcher_draw_status_bar(12, Stop, (volADC1 + volADC2) / 2, "V", iBaseADC1, "A");
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