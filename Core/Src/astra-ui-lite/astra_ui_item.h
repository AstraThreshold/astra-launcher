//
// Created by Fir on 24-11-30.
//

#ifndef FUCKCLION_CORE_SRC_ASTRA_UI_LITE_ASTRA_UI_ITEM_H_
#define FUCKCLION_CORE_SRC_ASTRA_UI_LITE_ASTRA_UI_ITEM_H_

#include "astra_ui_draw_driver.h"
#include <stdbool.h>

/*** 信息栏 ***/
#define INFO_BAR_HEIGHT 15
#define INFO_BAR_OFFSET 10

typedef struct astra_info_bar_t
{
  char *content;
  uint16_t span;
  float y_info_bar, y_info_bar_trg, w_info_bar, w_info_bar_trg;
  bool is_running;
  uint32_t time_start;
  uint32_t time;
} astra_info_bar_t;

extern astra_info_bar_t astra_info_bar;

extern void astra_push_info_bar(char *_content, const uint16_t _span);
/*** 信息栏 ***/

/*** 弹窗 ***/
#define POP_UP_HEIGHT 20
#define POP_UP_OFFSET 8

typedef struct astra_pop_up_t
{
  char *content;
  uint16_t span;
  float y_pop_up, y_pop_up_trg, w_pop_up, w_pop_up_trg;
  bool is_running;
  uint32_t time_start;
  uint32_t time;
} astra_pop_up_t;

extern astra_pop_up_t astra_pop_up;

extern void astra_push_pop_up(char *_content, const uint16_t _span);
/*** 弹窗 ***/

/*** 列表项 ***/
#define MAX_LIST_CHILD_NUM 10
#define MAX_LIST_LAYER 10
#define SCREEN_HEIGHT 64
#define SCREEN_WIDTH 128
#define LIST_ITEM_SPACEING 15
#define LIST_ITEM_OFFSET 8
#define LIST_INFO_BAR_HEIGHT 3
#define LIST_FONT_TOP_MARGIN 4

typedef enum
{
  list_item,
  switch_item,
  button_item,
  slider_item,
} astra_list_item_type_t;

typedef struct astra_list_item_t
{
  uint8_t layer;
  astra_list_item_type_t type;
  char *content;
  float y_list_item, y_list_item_trg;
  void *value;
  uint8_t child_num;
  struct astra_list_item_t *child_list_item[MAX_LIST_CHILD_NUM];
  struct astra_list_item_t *parent;
} astra_list_item_t;

extern astra_list_item_t astra_list_item_root; //根节点 根列表项
extern bool astra_bind_value_to_list_item(astra_list_item_t *_item, void *_value);
extern bool astra_push_item_to_list(astra_list_item_t *_parent, astra_list_item_t *_child);
/*** 列表项 ***/

/*** 选择器 ***/
typedef struct astra_selector_t
{
  float y_selector, y_selector_trg, w_selector, w_selector_trg, h_selector, h_selector_trg;
  uint8_t selected_index;
  astra_list_item_t *selected_item;
} astra_selector_t;

extern astra_selector_t astra_selector;
extern bool astra_bind_item_to_selector(astra_list_item_t *_item);
extern void astra_selector_go_next_item();
extern void astra_selector_go_prev_item();
/*** 选择器 ***/

/*** 相机 ***/
typedef struct astra_camera_t
{
  float x_camera, x_camera_trg, y_camera, y_camera_trg;
  astra_selector_t *selector;
} astra_camera_t;

extern astra_camera_t astra_camera;
extern void astra_bind_selector_to_camera(astra_selector_t *_selector);
/*** 相机 ***/

#endif //FUCKCLION_CORE_SRC_ASTRA_UI_LITE_ASTRA_UI_ITEM_H_
