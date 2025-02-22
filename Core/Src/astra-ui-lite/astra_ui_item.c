//
// Created by Fir on 24-11-30.
//

#include "astra_ui_item.h"

#include <string.h>
#include <sys/types.h>

#include "astra_ui_core.h"
#include "../astra-launcher/launcher_delay.h"

astra_info_bar_t astra_info_bar = {0, 1, 0 - 2 * INFO_BAR_HEIGHT, 0 - 2 * INFO_BAR_HEIGHT, 80, 80, false, 0, 1};

void astra_push_info_bar(char *_content, const uint16_t _span)
{
  //设定显示时间的概念，超过了显示时间，就将ytrg设为初始位置，如果在显示时间之内，有新的消息涌入，则y和ytrg都不变，继续显示，且显示时间清零
  //只有显示时间到了的时候，才会复位

  astra_info_bar.time = launcher_get_tick_ms();
  astra_info_bar.content = _content;
  astra_info_bar.span = _span;
  astra_info_bar.is_running = false; //每次进入该函数都代表有新的消息涌入，所以需要重置is_running

  //展开弹窗 收回弹窗和同步时间戳需要在循环中进行 所以移到了drawer中
  if (!astra_info_bar.is_running)
  {
    astra_info_bar.time_start = launcher_get_tick_ms();
    astra_info_bar.y_info_bar_trg = 0;
    astra_info_bar.is_running = true;
  }

  oled_set_font(u8g2_font_my_chinese);
  astra_info_bar.w_info_bar_trg = oled_get_UTF8_width(astra_info_bar.content) + INFO_BAR_OFFSET;
}

astra_pop_up_t astra_pop_up = {0, 1, 0 - 2 * POP_UP_HEIGHT, 0 - 2 * POP_UP_HEIGHT, 80, 80, false, 0, 1};

void astra_push_pop_up(char *_content, const uint16_t _span)
{
  astra_pop_up.time = launcher_get_tick_ms();
  astra_pop_up.content = _content;
  astra_pop_up.span = _span;
  astra_pop_up.is_running = false;

  //弹出
  if (!astra_pop_up.is_running)
  {
    astra_pop_up.time_start = launcher_get_tick_ms();
    astra_pop_up.y_pop_up_trg = 20;
    astra_pop_up.is_running = true;
  }

  oled_set_font(u8g2_font_my_chinese);
  astra_pop_up.w_pop_up_trg = oled_get_UTF8_width(astra_pop_up.content) + POP_UP_OFFSET;
}

astra_list_item_t astra_list_item_root = {};

bool astra_bind_value_to_list_item(astra_list_item_t *_item, void *_value)
{
  if (_item == NULL) return false;
  if (_value == NULL) return false;
  if (_item->type == list_item) return false;

  _item->value = _value;

  return true;
}

bool astra_bind_init_function_to_user_item(astra_list_item_t *_user_item, void (*_init_function)())
{
  if (_user_item == NULL) return false;
  if (_user_item->type != user_item) return false;
  if (_init_function == NULL) return false;

  _user_item->init_function = _init_function;

  return true;
}

bool astra_bind_loop_function_to_user_item(astra_list_item_t *_user_item, void (*_loop_function)())
{
  if (_user_item == NULL) return false;
  if (_user_item->type != user_item) return false;
  if (_loop_function == NULL) return false;

  _user_item->loop_function = _loop_function;

  return true;
}

astra_selector_t astra_selector = {0, };

bool astra_bind_item_to_selector(astra_list_item_t *_item)
{
  if (_item == NULL) return false;

  //找item在父节点中的序号
  uint8_t _temp_index = 0;
  for (uint8_t i = 0; i < _item->parent->child_num; i++)
  {
    if (_item->parent->child_list_item[i] == _item)
    {
      _temp_index = i;
      break;
    }
  }

  //坐标在refresh内部更新
  if (astra_selector.selected_item == NULL)
  {
    astra_selector.y_selector = 2 * SCREEN_HEIGHT;  //给个初始坐标做动画
    astra_selector.h_selector = 160;
  }
  astra_selector.selected_index = _temp_index;
  astra_selector.selected_item = _item;

  return true;
}

void astra_selector_go_next_item()
{
  //到达最末端
  if (astra_selector.selected_index == astra_selector.selected_item->parent->child_num - 1)
  {
    astra_selector.selected_item = astra_selector.selected_item->parent->child_list_item[0];
    astra_selector.selected_index = 0;
    return;
  }

  astra_selector.selected_item = astra_selector.selected_item->parent->child_list_item[++astra_selector.selected_index];
}

void astra_selector_go_prev_item()
{
  //到达最前端
  if (astra_selector.selected_index == 0)
  {
    astra_selector.selected_item = astra_selector.selected_item->parent->child_list_item[astra_selector.selected_item->parent->child_num - 1];
    astra_selector.selected_index = astra_selector.selected_item->parent->child_num - 1;
    return;
  }

  astra_selector.selected_item = astra_selector.selected_item->parent->child_list_item[--astra_selector.selected_index];
}

bool astra_exit_animation_finished = false;

void astra_selector_jump_to_next_layer()
{
  if (!in_astra) return;

  if (astra_selector.selected_item->type == user_item)
  {
    astra_exit_animation_finished = false;
    astra_selector.selected_item->in_user_item = true;
    astra_selector.selected_item->user_item_inited = false;
    astra_selector.selected_item->user_item_looping = false;
    return;
  }

  if (astra_selector.selected_item->child_num == 0) return;

  //给选择的item的子item坐标清零 做动画
  for (uint8_t i = 0; i < astra_selector.selected_item->child_num; i++)
    astra_selector.selected_item->child_list_item[i]->y_list_item = 0;

  astra_selector.selected_index = 0;
  astra_selector.selected_item = astra_selector.selected_item->child_list_item[0];
}

void astra_selector_jump_to_prev_layer()
{
  //todo 这个if待测试
  if (astra_selector.selected_item->type == user_item && astra_selector.selected_item->in_user_item)
  {
    astra_exit_animation_finished = false; //需要重新绘制退场动画
    astra_selector.selected_item->in_user_item = false;
    astra_selector.selected_item->user_item_inited = false;
    astra_selector.selected_item->user_item_looping = false;
    return;
  }

  if (astra_selector.selected_item->parent->layer == 0 && in_astra)
  {
    if (ALLOW_EXIT_ASTRA_UI_BY_USER) in_astra = false;
    return;
  }

  //给选择的item的父item的父item的所有子item坐标清零 做动画
  for (uint8_t i = 0; i < astra_selector.selected_item->parent->parent->child_num; i++)
      astra_selector.selected_item->parent->parent->child_list_item[i]->y_list_item = 0;

  //找到当前选择的item的父item在它的父item中的位置
  uint8_t _temp_index = 0;
  for (uint8_t i = 0; i < astra_selector.selected_item->parent->parent->child_num; i++)
  {
    if (astra_selector.selected_item->parent->parent->child_list_item[i] == astra_selector.selected_item->parent)
    {
      _temp_index = i;
      break;
    }
  }
  astra_selector.selected_index = _temp_index;
  astra_selector.selected_item = astra_selector.selected_item->parent;
}

bool astra_push_item_to_list(astra_list_item_t *_parent, astra_list_item_t *_child)
{
  if (_parent == NULL) return false;
  if (_child == NULL) return false;
  if (_parent->child_num >= MAX_LIST_CHILD_NUM) return false;
  if (_parent->layer >= MAX_LIST_LAYER) return false;

  _child->layer = _parent->layer + 1;
  _child->child_num = 0;

  oled_set_font(u8g2_font_my_chinese);
  if (_parent->child_num == 0) _child->y_list_item_trg = oled_get_str_height() + LIST_FONT_TOP_MARGIN - 1;
  else _child->y_list_item_trg = _parent->child_list_item[_parent->child_num - 1]->y_list_item_trg + LIST_ITEM_SPACEING;

  if (_parent->layer == 0 && _parent->child_num == 0)
  {
    astra_bind_item_to_selector(_child);  //初始化并绑定selector
    astra_bind_selector_to_camera(&astra_selector);  //初始化并绑定camera
  }

  _parent->child_list_item[_parent->child_num++] = _child;
  _child->parent = _parent;

  return true;
}

astra_camera_t astra_camera = {0, 0, 0, 0}; //在refresh加上camera的坐标

void astra_bind_selector_to_camera(astra_selector_t *_selector)
{
  if (_selector == NULL) return;

  astra_camera.selector = _selector;  //坐标在refresh内部更新
}




