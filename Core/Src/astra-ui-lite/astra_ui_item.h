//
// Created by Fir on 24-11-30.
//

#ifndef FUCKCLION_CORE_SRC_ASTRA_UI_LITE_ASTRA_UI_ITEM_H_
#define FUCKCLION_CORE_SRC_ASTRA_UI_LITE_ASTRA_UI_ITEM_H_

#include "astra_draw_driver.h"
#include <stdbool.h>

#define POP_HEIGHT 15
#define POP_OFFSET 4

typedef struct astra_info_bar_t {
  char *content;
  uint16_t span;
  int16_t y_info_bar, y_info_bar_trg, w_info_bar, w_info_bar_trg;
  bool is_running;
  uint16_t time_start;
  uint16_t time;
} astra_info_bar_t;

extern astra_info_bar_t astra_info_bar;
extern void astra_push_info_bar(char *_content, uint16_t _span);

extern void astra_draw_pop_window(const char *_content);


#endif //FUCKCLION_CORE_SRC_ASTRA_UI_LITE_ASTRA_UI_ITEM_H_
