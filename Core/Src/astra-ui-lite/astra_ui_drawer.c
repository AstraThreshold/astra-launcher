//
// Created by forpaindream on 25-1-20.
//

#include "astra_ui_drawer.h"

#include <math.h>
#include <stdio.h>

void astra_exit_animation(float *_pos, float _posTrg, float _speed)
{
  if (*_pos != _posTrg)
  {
    if (fabs(*_pos - _posTrg) <= 1.0f) *_pos = _posTrg;
    else *_pos += (_posTrg - *_pos) / (100.0f - _speed) / 1.0f;
  }
}

uint8_t astra_exit_animation_status = 0;

void astra_draw_exit_animation()
{
  //完成完整的退场动画 astra_exit_animation_status的取值依次如下
  //0 触发退场动画 遮罩开始落下
  //1 遮罩落下完成 此时屏幕被遮罩填满 开始变更背景内容
  //2 遮罩开始抬升
  //0 遮罩抬升完成 退场动画完成
  static float _temp_h = -8;
  static float _temp_h_trg = OLED_HEIGHT + 8;

  oled_set_draw_color(0);
  oled_draw_box(0, 0, OLED_WIDTH, _temp_h);  //遮罩
  oled_set_draw_color(1);

  // 沙漏
  uint8_t _x_hourglass_offset = OLED_WIDTH / 2 - 8;
  int8_t _y_hourglass = _temp_h - OLED_HEIGHT/2 - 18;
  if (_y_hourglass + 20 >= 0) {
    // 绘制顶部和底部矩形及中间擦除
    oled_draw_box(_x_hourglass_offset, _y_hourglass + 2, 13, 3);
    oled_set_draw_color(0);
    oled_draw_H_line(_x_hourglass_offset + 2, _y_hourglass + 3, 9);
    oled_set_draw_color(1);

    // 主体结构
    oled_draw_V_line(_x_hourglass_offset + 1, _y_hourglass + 4, 5);
    oled_draw_V_line(_x_hourglass_offset + 11, _y_hourglass + 4, 5);

    // 斜线部分循环绘制
    for (uint8_t i = 0; i < 5; ++i) {
      int8_t _current_y = _y_hourglass + 8 + i;
      int8_t _left_x = (i < 3) ? (_x_hourglass_offset + 1 + i) : (_x_hourglass_offset + 4);
      int8_t _right_x = (i < 3) ? (_x_hourglass_offset + 10 - i) : (_x_hourglass_offset + 7);
      oled_draw_H_line(_left_x, _current_y, 2);
      oled_draw_H_line(_right_x, _current_y, 2);
    }

    // 中间收口部分
    for (uint8_t i = 0; i < 3; ++i) {
      int8_t _current_y = _y_hourglass + 13 + i;
      oled_draw_H_line(_x_hourglass_offset + 3 - i, _current_y, 2);
      oled_draw_H_line(_x_hourglass_offset + 8 + i, _current_y, 2);
    }

    // 底部竖线
    oled_draw_V_line(_x_hourglass_offset + 1, _y_hourglass + 16, 3);
    oled_draw_V_line(_x_hourglass_offset + 11, _y_hourglass + 16, 3);

    // 底部矩形
    oled_draw_box(_x_hourglass_offset, _y_hourglass + 19, 13, 3);
    oled_set_draw_color(0);
    oled_draw_H_line(_x_hourglass_offset + 2, _y_hourglass + 20, 9);
    oled_set_draw_color(1);

    // 散点像素数组化绘制
    const uint8_t points[][2] = {{5,7}, {7,7}, {6,8}, {6,10}, {6,14}, {6,16},
                                 {5,17}, {7,17}, {4,18}, {6,18}, {8,18}};
    for (uint8_t i = 0; i < sizeof(points)/sizeof(points[0]); ++i) {
      oled_draw_pixel(_x_hourglass_offset + points[i][0], _y_hourglass + points[i][1]);
    }
  }

  if (_temp_h + 3 >= 0)
  {
    //下面是遮罩下方横线
    for (uint8_t i = 0; i <= 3; ++i)
    {
      oled_draw_H_line(0, _temp_h + i, OLED_WIDTH);
    }
  }

  //棋盘格过渡
  for (int16_t i = 0; i <= OLED_WIDTH ; i += 2)
  {
    for (int16_t j = _temp_h - 5; j <= _temp_h - 1; j++)
    {
      if (j % 2 == 0) oled_draw_pixel(i + 1, j);
      if (j % 2 == 1) oled_draw_pixel(i, j);
    }
  }

  astra_exit_animation(&_temp_h, _temp_h_trg, 94);

  //下落过程
  if (astra_exit_animation_status == 0 && _temp_h == _temp_h_trg && _temp_h == OLED_HEIGHT + 8)
  {
    astra_exit_animation_status = 1; //落下来了
    return;
  }

  //上面astra_exit_animation_status=1之后 return了 进到core里刷新了背景显示内容 下一次进到本函数就可以把标志位置为2
  if (astra_exit_animation_status == 1)
  {
    // _temp_h_trg = OLED_HEIGHT + 8;
    _temp_h_trg = -8; //使其开始上升
    astra_exit_animation_status = 2; //开始抬起
    return;
  }

  if (astra_exit_animation_status == 2 && _temp_h == _temp_h_trg && _temp_h == -8)
  {
    astra_exit_animation_finished = true;
    astra_exit_animation_status = 0; //退场动画完成
    _temp_h = -8;
    _temp_h_trg = OLED_HEIGHT + 8;
    return;
  }
}

void astra_draw_info_bar()
{
  if (!astra_info_bar.is_running) return;

  //弹窗到位后才开始计算时间
  if (astra_info_bar.y_info_bar == astra_info_bar.y_info_bar_trg) astra_info_bar.time = launcher_get_tick_ms();

  //时间到了就收回
  if (astra_info_bar.time - astra_info_bar.time_start >= astra_info_bar.span)
  {
    astra_info_bar.y_info_bar_trg = 0 - 2 * INFO_BAR_HEIGHT; //收回
    if (astra_info_bar.y_info_bar == astra_info_bar.y_info_bar_trg) astra_info_bar.is_running = false; //等归位后结束生命周期
  }

  oled_set_font(u8g2_font_my_chinese);
  oled_set_draw_color(1);
  oled_draw_R_box((int16_t)(OLED_WIDTH/2 - astra_info_bar.w_info_bar/2) + 3, (int16_t)(astra_info_bar.y_info_bar - 4) + 3,
                  (int16_t)(astra_info_bar.w_info_bar), INFO_BAR_HEIGHT + 4, 4);

  oled_set_draw_color(0); //黑遮罩打底
  oled_draw_R_box((int16_t)(OLED_WIDTH/2 - (astra_info_bar.w_info_bar + 4)/2), (int16_t)(astra_info_bar.y_info_bar - 4),
                  (int16_t)(astra_info_bar.w_info_bar + 4), INFO_BAR_HEIGHT + 6, 4);

  oled_set_draw_color(1);
  oled_draw_R_box((int16_t)(OLED_WIDTH/2 - astra_info_bar.w_info_bar/2), (int16_t)(astra_info_bar.y_info_bar - 4),
                    (int16_t)(astra_info_bar.w_info_bar), INFO_BAR_HEIGHT + 4, 3);
  //向上移动四个像素 同时向下多画四个像素 只用下半部分圆角

  oled_set_draw_color(2);
  oled_draw_H_line((int16_t)(OLED_WIDTH/2 - astra_info_bar.w_info_bar/2 + 2),
                   (int16_t)(astra_info_bar.y_info_bar + INFO_BAR_HEIGHT - 2),
                   (int16_t)(astra_info_bar.w_info_bar - 4));
  oled_draw_pixel((int16_t)(OLED_WIDTH/2 - astra_info_bar.w_info_bar/2 + 1),
                  (int16_t)(astra_info_bar.y_info_bar + INFO_BAR_HEIGHT - 3));
  oled_draw_pixel((int16_t)(OLED_WIDTH/2 + astra_info_bar.w_info_bar/2 - 2),
                  (int16_t)(astra_info_bar.y_info_bar + INFO_BAR_HEIGHT - 3));

  oled_draw_UTF8((int16_t)(OLED_WIDTH/2 - astra_info_bar.w_info_bar/2 + 6),
                 (int16_t)(astra_info_bar.y_info_bar + oled_get_str_height() - 2),
                 astra_info_bar.content);
}

void astra_draw_pop_up()
{
  if (!astra_pop_up.is_running) return;

  //弹窗到位后才开始计算时间
  if (astra_pop_up.y_pop_up == astra_pop_up.y_pop_up_trg) astra_pop_up.time = launcher_get_tick_ms();

  //时间到了就收回
  if (astra_pop_up.time - astra_pop_up.time_start >= astra_pop_up.span)
  {
    astra_pop_up.y_pop_up_trg = 0 - 2 * INFO_BAR_HEIGHT; //收回
    if (astra_pop_up.y_pop_up == astra_pop_up.y_pop_up_trg) astra_pop_up.is_running = false; //等归位后结束生命周期
  }

  oled_set_font(u8g2_font_my_chinese);
  oled_set_draw_color(1); //阴影打底
  oled_draw_R_box((int16_t)(OLED_WIDTH/2 - astra_pop_up.w_pop_up/2 + 1), (int16_t)astra_pop_up.y_pop_up + 3,
                    (int16_t)(astra_pop_up.w_pop_up + 4),
                    POP_UP_HEIGHT, 4);

  oled_set_draw_color(0); //黑遮罩
  oled_draw_R_box((int16_t)(OLED_WIDTH/2 - (astra_pop_up.w_pop_up + 4)/2 - 2), (int16_t)(astra_pop_up.y_pop_up - 2),
                  (int16_t)(astra_pop_up.w_pop_up + 8), POP_UP_HEIGHT + 4, 5);

  oled_set_draw_color(1);
  oled_draw_R_box((int16_t)(OLED_WIDTH/2 - astra_pop_up.w_pop_up/2 - 2), (int16_t)astra_pop_up.y_pop_up,
                    (int16_t)(astra_pop_up.w_pop_up + 4),
                    POP_UP_HEIGHT, 3);

  oled_set_draw_color(2);
  oled_draw_H_line((int16_t)(OLED_WIDTH/2 - astra_pop_up.w_pop_up/2),
                   (int16_t)(astra_pop_up.y_pop_up + POP_UP_HEIGHT - 2),
                   (int16_t)astra_pop_up.w_pop_up);
  oled_draw_pixel((int16_t)(OLED_WIDTH/2 - astra_pop_up.w_pop_up/2 - 1),
                  (int16_t)(astra_pop_up.y_pop_up + POP_UP_HEIGHT - 3));
  oled_draw_pixel((int16_t)(OLED_WIDTH/2 + astra_pop_up.w_pop_up/2),
                  (int16_t)(astra_pop_up.y_pop_up + POP_UP_HEIGHT - 3));

  oled_draw_UTF8((int16_t)(OLED_WIDTH/2 - astra_pop_up.w_pop_up/2 + 3),
                 (int16_t)(astra_pop_up.y_pop_up + oled_get_str_height() + 1),
                 astra_pop_up.content);
}

void astra_draw_list_appearance()
{
  oled_set_draw_color(1);
  //顶部状态栏
  oled_draw_H_line(0, 1, 66);
  oled_draw_H_line(0, 0, 67);
  // 参数化绘制配置
  const struct {
    uint8_t _start;
    uint8_t _end;
    uint8_t _step;
    uint8_t _y;
  } draw_cfg[] = {
    {67,  99, 2, 1},  // 奇数序列优化为步长2
    {68, 100, 2, 0},  // 偶数序列优化为步长2
    {102,111, 3, 1},  // 原i%3==0等效步长3（数学变换后起始点+1）
    {103,112, 3, 0},  // 原i%3==1等效步长3（数学变换后起始点+2）
    {115,124, 5, 1},  // 原i%5==0等效步长5（数学变换后起始点+3）
    {116,124, 5, 0}   // 原i%5==1等效步长5（数学变换后起始点+2）
  };

  for (uint8_t j = 0; j < sizeof(draw_cfg)/sizeof(draw_cfg[0]); ++j) {
    for (uint8_t i = draw_cfg[j]._start; i <= draw_cfg[j]._end; i += draw_cfg[j]._step) {
      oled_draw_pixel(i, draw_cfg[j]._y);
    }
  }

  //右侧进度条
  oled_draw_V_line(OLED_WIDTH - 5, 0, OLED_HEIGHT);
  oled_draw_V_line(OLED_WIDTH - 1, 0, OLED_HEIGHT);

  //滑块
  static float _length_each_part = 0;
  _length_each_part = ceilf((SCREEN_HEIGHT - 10.0f) / (float)astra_selector.selected_item->parent->child_num);
  oled_draw_box(OLED_WIDTH - 4, 5 + astra_selector.selected_index * _length_each_part, 3, _length_each_part);

  // char test[10];
  // sprintf(test, "%.2f", _length_each_part);
  // oled_draw_UTF8(80, 40, test);

  //滑块内横线
  oled_set_draw_color(0);
  oled_draw_H_line(OLED_WIDTH - 4, _length_each_part + (float)astra_selector.selected_index * _length_each_part, 3);  //中间横线

  //长度允许的情况下绘制上下横线
  if (_length_each_part >= 9)
  {
    oled_draw_H_line(OLED_WIDTH - 4, floorf(_length_each_part - 2.0f + (float)astra_selector.selected_index * _length_each_part), 3);
    oled_draw_H_line(OLED_WIDTH - 4, floorf(_length_each_part + 2.0f + (float)astra_selector.selected_index * _length_each_part), 3);
  }

  oled_set_draw_color(1);
  oled_draw_box(OLED_WIDTH - 4, 0, 3, 4);
  oled_draw_box(OLED_WIDTH - 4, OLED_HEIGHT - 4, 3, 4);
  oled_set_draw_color(0);
  oled_draw_H_line(OLED_WIDTH - 4, 2, 3);
  oled_draw_pixel(OLED_WIDTH - 3, 1);
  oled_draw_H_line(OLED_WIDTH - 4, OLED_HEIGHT - 3, 3);
  oled_draw_pixel(OLED_WIDTH - 3, OLED_HEIGHT - 2);
}

//todo 视野外的部分将不会被渲染 但是现在坐标值小于屏幕范围的左值待定 并未缜密测试
void astra_draw_list_item()
{
  oled_set_draw_color(1);
  //selector内包含的item的parent即是当前正在被绘制的页面
  oled_set_font(u8g2_font_my_chinese);
  for (unsigned char i = 0; i < astra_selector.selected_item->parent->child_num; i++)
  {
    static int16_t _temp_y = 0; //just fucking saving time.
    _temp_y = (int16_t)(astra_selector.selected_item->parent->child_list_item[i]->y_list_item + astra_camera.y_camera);

    oled_set_draw_color(1);
    //绘制开头的指示器
    if (astra_selector.selected_item->parent->child_list_item[i]->type == list_item)
    {
      if (_temp_y - oled_get_str_height() / 2 + 2 > LIST_INFO_BAR_HEIGHT && _temp_y - oled_get_str_height() / 2 - 2 < SCREEN_HEIGHT)
      {
        oled_draw_H_line((int16_t)(2 + astra_camera.x_camera), _temp_y - oled_get_str_height() / 2 - 2, 4);
        oled_draw_H_line((int16_t)(2 + astra_camera.x_camera), _temp_y - oled_get_str_height() / 2, 5);
        oled_draw_H_line((int16_t)(2 + astra_camera.x_camera), _temp_y - oled_get_str_height() / 2 + 2, 3);
      }
    }
    else if (astra_selector.selected_item->parent->child_list_item[i]->type == switch_item)
    {
      if (_temp_y - oled_get_str_height() / 2 + 1 + 6 > LIST_INFO_BAR_HEIGHT && _temp_y - oled_get_str_height() / 2 + 1 < SCREEN_HEIGHT)
      {
        oled_draw_circle((int16_t)(4 + astra_camera.x_camera), _temp_y - oled_get_str_height() / 2 + 1, 3);
        oled_draw_V_line((int16_t)(4 + astra_camera.x_camera), _temp_y - oled_get_str_height() / 2 , 3);
      }
    }
    else if (astra_selector.selected_item->parent->child_list_item[i]->type == button_item)
    {
      if (_temp_y - oled_get_str_height() / 2 > LIST_INFO_BAR_HEIGHT && _temp_y - oled_get_str_height() / 2 - 1 < SCREEN_HEIGHT)
      {
        oled_draw_box((int16_t)(2 + astra_camera.x_camera), _temp_y - oled_get_str_height() / 2 - 1, 4, 4);
        oled_draw_H_line((int16_t)(3 + astra_camera.x_camera), _temp_y - oled_get_str_height() / 2 + 4, 4);
        oled_draw_V_line((int16_t)(7 + astra_camera.x_camera), _temp_y - oled_get_str_height() / 2, 5);
      }
    }
    else if (astra_selector.selected_item->parent->child_list_item[i]->type == slider_item)
    {
      if (_temp_y - oled_get_str_height() / 2 + 2 + 3 > LIST_INFO_BAR_HEIGHT && _temp_y - oled_get_str_height() / 2 - 2 < SCREEN_HEIGHT)
      {
        oled_draw_V_line((int16_t)(3 + astra_camera.x_camera), _temp_y - oled_get_str_height() / 2 - 1, 5);
        oled_draw_V_line((int16_t)(6 + astra_camera.x_camera), _temp_y - oled_get_str_height() / 2 - 1, 5);
        oled_draw_box((int16_t)(2 + astra_camera.x_camera), _temp_y - oled_get_str_height() / 2 - 2, 3, 3);
        oled_draw_box((int16_t)(5 + astra_camera.x_camera), _temp_y - oled_get_str_height() / 2 + 2, 3, 3);
      }
    }
    else
    {
      if (_temp_y > LIST_INFO_BAR_HEIGHT && _temp_y - oled_get_str_height() < SCREEN_HEIGHT) oled_draw_str((int16_t)(2 + astra_camera.x_camera), _temp_y, "-");
    }
    if (_temp_y > LIST_INFO_BAR_HEIGHT && _temp_y - oled_get_str_height() < SCREEN_HEIGHT) oled_draw_UTF8((int16_t)(10 + astra_camera.x_camera), _temp_y, astra_selector.selected_item->parent->child_list_item[i]->content);
  }
}

void astra_draw_selector()
{
  oled_set_draw_color(2);
  oled_draw_box((int16_t)(0 + astra_camera.x_camera), (int16_t)(astra_selector.y_selector + astra_camera.y_camera), astra_selector.w_selector, astra_selector.h_selector);

  //棋盘格过渡
  oled_set_draw_color(1);
  for (int16_t i = astra_selector.w_selector + astra_camera.x_camera; i <= astra_selector.w_selector + astra_camera.x_camera + 7; i += 2)
  {
    for (int16_t j = astra_selector.y_selector + astra_camera.y_camera; j <= astra_selector.y_selector + astra_camera.y_camera + astra_selector.h_selector - 1; j++)
    {
      if (j % 2 == 0) oled_draw_pixel(i + 1, j);
      if (j % 2 == 1) oled_draw_pixel(i, j);
    }
  }
}

void astra_draw_widget()
{
  //需要调用所有的控件draw函数 需要在core后面执行 否则会被core覆盖
  astra_draw_info_bar();
  astra_draw_pop_up();
}

void astra_draw_list()
{
  //调用所有的列表相关draw函数
  astra_draw_list_appearance();
  astra_draw_list_item();
  astra_draw_selector();
}
