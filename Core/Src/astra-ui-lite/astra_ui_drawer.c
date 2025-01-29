//
// Created by forpaindream on 25-1-20.
//

#include "astra_ui_drawer.h"

#include <stdio.h>

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
  //顶部状态栏
  oled_draw_H_line(0, 1, 66);
  oled_draw_H_line(0, 0, 67);
  for (uint8_t i = 67; i <= 99; i++) if (i % 2 == 1) oled_draw_pixel(i, 1);
  for (uint8_t i = 68; i <= 100; i++) if (i % 2 == 0) oled_draw_pixel(i, 0);
  for (uint8_t i = 101; i <= 111; i++) if (i % 3 == 0) oled_draw_pixel(i, 1);
  for (uint8_t i = 102; i <= 112; i++) if (i % 3 == 1) oled_draw_pixel(i, 0);
  for (uint8_t i = 112; i <= 124; i++) if (i % 5 == 0) oled_draw_pixel(i, 1);
  for (uint8_t i = 114; i <= 124; i++) if (i % 5 == 1) oled_draw_pixel(i, 0);

  //右侧进度条
  oled_draw_V_line(123, 0, 64);
  oled_draw_V_line(127, 0, 64);

  //滑块以及滑块内的三条横线
  static uint8_t _length_each_part = 0;
  _length_each_part = (SCREEN_HEIGHT - 10) / astra_selector.selected_item->parent->child_num;
  oled_draw_box(124, 5 + astra_selector.selected_index * _length_each_part, 3, _length_each_part);
  oled_set_draw_color(0);
  oled_draw_H_line(124,_length_each_part + astra_selector.selected_index * _length_each_part,3);  //中间横线

  //两边横线
  if (_length_each_part >= 9)
  {
    oled_draw_H_line(124,_length_each_part - 2 + astra_selector.selected_index * _length_each_part,3);
    oled_draw_H_line(124,_length_each_part + 2 + astra_selector.selected_index * _length_each_part,3);
  }

  oled_set_draw_color(1);
  oled_draw_box(124, 0, 3, 4);
  oled_draw_box(124, 60, 3, 4);
  oled_set_draw_color(0);
  oled_draw_H_line(124, 2, 3);
  oled_draw_pixel(125, 1);
  oled_draw_H_line(124, 61, 3);
  oled_draw_pixel(125, 62);
}

//todo 视野外的部分将不会被渲染 但是现在坐标值小于屏幕范围的左值待定 并未缜密测试
void astra_draw_list_item()
{
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
  // oled_draw_box(0,4,72,15);
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
