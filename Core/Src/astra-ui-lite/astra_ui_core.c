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
      _key_start_time = launcher_get_tick_ms();
      //变量上限是0xFFFF 65535
    }
    if (launcher_get_tick_ms() - _key_start_time > 1000 && _key_clicked)
    {
      _key_press_span = launcher_get_tick_ms() - _key_start_time;
      if (_key_press_span <= 2500)
      {
        sprintf(_msg, "继续长按%.2f秒进入.", (2500 - _key_press_span) / 1000.0f);
        astra_push_info_bar(_msg, 2000);
      } else if (_key_press_span > 2500)
      {
        astra_push_info_bar("玩得开心! :p", 2000);
        in_astra = true;
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
  for (uint8_t i = 0; i < astra_selector.selected_item->parent->child_num; i++)
    astra_animation(&astra_selector.selected_item->parent->child_list_item[i]->y_list_item, astra_selector.selected_item->parent->child_list_item[i]->y_list_item_trg, 84);
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

void astra_ui_main_core()
{
  if (!in_astra) return;

  //退场动画
  if (!astra_exit_animation_finished)
  {
    do
    {
      oled_clear_buffer();
      //需要执行退场动画了
      //因为标志位先被置了对应的位 所以如果in_user_item=true就代表刚进user item 需要渲染列表项
      //如果in_user_item=false 就代表退出user item 需要渲染user item 即继续执行loop
      //如果就在item里面
      if (astra_selector.selected_item->in_user_item)
      {
        astra_refresh_camera_position();
        astra_refresh_main_core_position();
        astra_refresh_selector_position();
        astra_draw_list();
      } else if (!astra_selector.selected_item->in_user_item)
      {
        astra_selector.selected_item->loop_function();
      }
      // astra_draw_exit_animation();

    } while (!astra_draw_exit_animation());
    astra_exit_animation_finished = true;
    return;
  }

  if (astra_selector.selected_item->in_user_item)
  {
    // //退场动画
    // if (!astra_exit_animation_finished)
    // {
    //   for (uint8_t i = 0; i < 8; i++)
    //   {
    //     oled_clear_buffer();
    //     astra_refresh_camera_position();
    //     astra_refresh_main_core_position();
    //     astra_refresh_selector_position();
    //     astra_draw_list();
    //     astra_draw_exit_animation(i);
    //     oled_send_buffer();
    //     delay(5);
    //   }
    //   astra_exit_animation_finished = true;
    // }

    //初始化
    if (!astra_selector.selected_item->user_item_inited)
    {
      if (astra_selector.selected_item->init_function != NULL) astra_selector.selected_item->init_function();
      astra_selector.selected_item->user_item_inited = true;
    }

    if (astra_selector.selected_item->loop_function != NULL)
    {
      astra_selector.selected_item->user_item_looping = true;
      astra_selector.selected_item->loop_function();
    }

    return;
  }
  //无需修改
  astra_refresh_camera_position();
  astra_refresh_main_core_position();
  astra_refresh_selector_position();
  astra_draw_list();
}
