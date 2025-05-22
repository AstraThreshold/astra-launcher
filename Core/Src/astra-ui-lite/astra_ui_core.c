//
// Created by forpaindream on 25-1-20.
//

#include "astra_ui_core.h"
#include <stdio.h>
#include "astra_ui_drawer.h"
#include <tgmath.h>

bool in_astra = false;

/**
 * @brief 进入astra ui lite
 *
 * @note 需要运行在循环中
 * @note 可以通过按键等传感器进行触发 当in_astra为true时进入astra ui lite
 */
void ad_astra()
{
  /**自行修改**/
  if (in_astra) return;
  static int64_t _key_press_span = 0;
  static uint32_t _key_start_time = 0;
  static bool _key_clicked = false;
  static char _msg[100] = {};

  if (HAL_GPIO_ReadPin(KEY1_GPIO_Port, KEY1_Pin) == GPIO_PIN_RESET || HAL_GPIO_ReadPin(KEY2_GPIO_Port, KEY2_Pin) == GPIO_PIN_RESET)
  {
    if (!_key_clicked)
    {
      _key_clicked = true;
      _key_start_time = get_ticks();
      //变量上限是0xFFFF 65535
    }
    if (get_ticks() - _key_start_time > 1000 && _key_clicked)
    {
      _key_press_span = get_ticks() - _key_start_time;
      if (_key_press_span <= 2500)
      {
        sprintf(_msg, "继续长按%.2f秒进入.", (2500 - _key_press_span) / 1000.0f);
        astra_push_info_bar(_msg, 2000);
      } else if (_key_press_span > 2500)
      {
        astra_push_info_bar("have fun! :p", 2000);
        in_astra = true;
        astra_init_list();
        _key_clicked = false;
        _key_start_time = 0;
        _key_press_span = 0;
      }
    }
  } else
  {
    _key_clicked = false;
    if (_key_press_span != 0)
    {
      astra_push_info_bar("bye!", 2000);
      _key_press_span = 0;
    }
  }
  /**自行修改**/
}

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
  astra_animation(&astra_info_bar.w_info_bar, astra_info_bar.w_info_bar_trg, 95);
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

  astra_animation(&astra_camera.x_camera, astra_camera.x_camera_trg, 95);
  astra_animation(&astra_camera.y_camera, astra_camera.y_camera_trg, 96);
}

void astra_refresh_widget_core_position()
{
  //需要调用所有的widget refresh函数
  astra_refresh_info_bar();
  astra_refresh_pop_up();
}

void astra_init_list()
{
  //做动画
  for (uint8_t i = 0; i < astra_get_root_list()->child_num; i++)
    astra_get_root_list()->child_list_item[i]->y_list_item = 0;
  astra_selector.selected_index = 0;
  astra_selector.selected_item = astra_get_root_list()->child_list_item[0];
  astra_selector.y_selector = OLED_HEIGHT;
  astra_selector.h_selector = OLED_HEIGHT;
}

void astra_init_core()
{
  astra_init_list();
  astra_bind_item_to_selector(astra_get_root_list());
  astra_bind_selector_to_camera(astra_get_selector());
}

void astra_refresh_list_item_position()
{
  for (uint8_t i = 0; i < astra_selector.selected_item->parent->child_num; i++)
    astra_animation(&astra_selector.selected_item->parent->child_list_item[i]->y_list_item, astra_selector.selected_item->parent->child_list_item[i]->y_list_item_trg, 84);
}

void astra_refresh_selector_position()
{
  astra_set_font(u8g2_font_my_chinese);
  astra_selector.y_selector_trg = astra_selector.selected_item->y_list_item_trg - oled_get_str_height() + 1;
  if (astra_selector.selected_item->type == switch_item || astra_selector.selected_item->type == slider_item)
    astra_selector.w_selector_trg = OLED_WIDTH - 18;
  else astra_selector.w_selector_trg = oled_get_UTF8_width(astra_selector.selected_item->content) + 12;
  astra_selector.h_selector_trg = 15;
  astra_animation(&astra_selector.y_selector, astra_selector.y_selector_trg, 91);
  astra_animation(&astra_selector.w_selector, astra_selector.w_selector_trg, 92);
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

void astra_ui_main_core()
{
  if (!in_astra) return;

  //切换in user item的逻辑
  if (astra_exit_animation_status == 1)
  {
    if (astra_selector.selected_item->type == user_item)
    {
      astra_user_item_t* _selected_user_item = astra_to_user_item(astra_selector.selected_item);
      if (_selected_user_item->entering_user_item)
        _selected_user_item->in_user_item = 1;
      else if (_selected_user_item->exiting_user_item)
      {
        if (_selected_user_item->user_item_inited && _selected_user_item->user_item_looping)
          _selected_user_item->exit_function();
        _selected_user_item->in_user_item = 0;
      }
    }
  }

  //渲染的逻辑
  if (astra_selector.selected_item->type == user_item && astra_to_user_item(astra_selector.selected_item)->in_user_item)
  {
    astra_user_item_t* _selected_user_item = astra_to_user_item(astra_selector.selected_item);
    //初始化
    if (!_selected_user_item->user_item_inited)
    {
      if (_selected_user_item->init_function != NULL)
        _selected_user_item->init_function();
      _selected_user_item->user_item_inited = true;
    }

    if (_selected_user_item->loop_function != NULL)
    {
      _selected_user_item->user_item_looping = true;
      _selected_user_item->loop_function();
    }
  } else
  {
    astra_refresh_main_core_position();
    astra_refresh_selector_position();
    astra_refresh_camera_position();
    astra_draw_list();
  }

  //退场动画
  //上面都是正常应当绘制的内容 退场动画需要绘制时 只需要在上面的基础上绘制遮罩即可
  if (!astra_exit_animation_finished)
    astra_draw_exit_animation();
}
