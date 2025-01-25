//
// Created by forpaindream on 25-1-20.
//

#include "astra_ui_core.h"
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
  astra_animation(&astra_info_bar.y_info_bar, astra_info_bar.y_info_bar_trg, 96);
  astra_animation(&astra_info_bar.w_info_bar, astra_info_bar.w_info_bar_trg, 96);
}

void astra_refresh_pop_up()
{
  astra_animation(&astra_pop_up.y_pop_up, astra_pop_up.y_pop_up_trg, 94);
  astra_animation(&astra_pop_up.w_pop_up, astra_pop_up.w_pop_up_trg, 96);
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
    astra_animation(&astra_list_item_root.child_list_item[i]->y_list_item, astra_list_item_root.child_list_item[i]->y_list_item_trg, 88);
}

void astra_refresh_main_core_position()
{
  astra_refresh_list_item_position();
}

void astra_ui_widget_core()
{
  astra_refresh_widget_core_position();
  astra_draw_info_bar();
  astra_draw_pop_up();
}

void astra_ui_main_core()
{
  //无需修改
  astra_refresh_main_core_position();
  astra_draw_core();
}
