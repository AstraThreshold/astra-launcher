//
// Created by forpaindream on 25-1-20.
//

#include "astra_ui_core.h"
#include "astra_ui_drawer.h"
#include <tgmath.h>

void astra_animation(int16_t *_pos, int16_t _posTrg, int16_t _speed)
{
  if (*_pos != _posTrg)
  {
    if (fabs(*_pos - _posTrg) <= 1.0f) *_pos = _posTrg;
    else *_pos += (_posTrg - *_pos) / ((100 - _speed) / 1.0f);
  }
}

//新的时基动画
void astra_animation_time_based(int16_t *_pos, int16_t _posTrg, int16_t _time)
{

}

void astra_refresh_info_bar()
{
  astra_animation(&astra_info_bar.y_info_bar, astra_info_bar.y_info_bar_trg, 92);
  astra_animation(&astra_info_bar.w_info_bar, astra_info_bar.w_info_bar_trg, 96);
}

void astra_refresh_pop_up()
{
  astra_animation(&astra_pop_up.y_pop_up, astra_pop_up.y_pop_up_trg, 92);
  astra_animation(&astra_pop_up.w_pop_up, astra_pop_up.w_pop_up_trg, 96);
}

void astra_refresh_position()
{
  //需要调用所有的refresh函数
  astra_refresh_info_bar();
  astra_refresh_pop_up();
}

void astra_ui_core()
{
  //无需修改
  astra_refresh_position();
  astra_draw_canvas();
}
