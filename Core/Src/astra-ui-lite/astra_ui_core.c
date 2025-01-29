//
// Created by forpaindream on 25-1-20.
//

#include "astra_ui_core.h"

#include <stdio.h>

#include "astra_ui_drawer.h"
#include <tgmath.h>

void astra_animation(float *_pos, float _posTrg, float _speed)
{
  if (*_pos != _posTrg)
  {
    if (fabs(*_pos - _posTrg) <= 1.0f) *_pos = _posTrg;
    else *_pos += (_posTrg - *_pos) / (100.0f - _speed) / 1.0f;
  }
}

void astra_refresh_info_bar()
{
  astra_animation(&astra_info_bar.y_info_bar, astra_info_bar.y_info_bar_trg, 94);
  astra_animation(&astra_info_bar.w_info_bar, astra_info_bar.w_info_bar_trg, 94);
}

void astra_refresh_pop_up()
{
  astra_animation(&astra_pop_up.y_pop_up, astra_pop_up.y_pop_up_trg, 94);
  astra_animation(&astra_pop_up.w_pop_up, astra_pop_up.w_pop_up_trg, 96);
}

void astra_refresh_camera_position()
{
  //15为selector的高度
  if (astra_camera.selector->y_selector_trg + 15 + astra_camera.y_camera_trg > SCREEN_HEIGHT)  //向下超出屏幕 需要向下移动
    astra_camera.y_camera_trg = SCREEN_HEIGHT - astra_camera.selector->y_selector_trg - 15;

  if (astra_camera.selector->y_selector_trg + astra_camera.y_camera_trg < 0)  //向上超出屏幕 需要向上移动
    astra_camera.y_camera_trg = 0 - astra_camera.selector->y_selector_trg + LIST_FONT_TOP_MARGIN;

  astra_animation(&astra_camera.x_camera, astra_camera.x_camera_trg, 96);
  astra_animation(&astra_camera.y_camera, astra_camera.y_camera_trg, 96);
}

void astra_refresh_widget_core_position()
{
  //需要调用所有的widget refresh函数
  astra_refresh_info_bar();
  astra_refresh_pop_up();
}

void astra_refresh_list_item_position()
{
  for (uint8_t i = 0; i < astra_list_item_root.child_num; i++)
    astra_animation(&astra_list_item_root.child_list_item[i]->y_list_item, astra_list_item_root.child_list_item[i]->y_list_item_trg, 84);
}

void astra_refresh_selector_position()
{
  astra_selector.y_selector_trg = astra_selector.selected_item->y_list_item_trg - oled_get_str_height() + 1;
  astra_selector.w_selector_trg = oled_get_UTF8_width(astra_selector.selected_item->content) + 12;
  astra_selector.h_selector_trg = 15;
  astra_animation(&astra_selector.y_selector, astra_selector.y_selector_trg, 91);
  astra_animation(&astra_selector.w_selector, astra_selector.w_selector_trg, 93);
  astra_animation(&astra_selector.h_selector, astra_selector.h_selector_trg, 93);
}

void astra_refresh_main_core_position()
{
  astra_refresh_list_item_position();
}

void astra_ui_widget_core()
{
  astra_refresh_widget_core_position();
  astra_draw_widget();
}

void null_function1() {}

void astra_ui_main_core()
{
  //无需修改
  astra_refresh_camera_position();
  astra_refresh_main_core_position();
  astra_refresh_selector_position();
  astra_draw_list();
}
