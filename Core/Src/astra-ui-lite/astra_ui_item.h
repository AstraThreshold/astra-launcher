//
// Created by Fir on 24-11-30.
//

#ifndef FUCKCLION_CORE_SRC_ASTRA_UI_LITE_ASTRA_UI_ITEM_H_
#define FUCKCLION_CORE_SRC_ASTRA_UI_LITE_ASTRA_UI_ITEM_H_

#include <stdbool.h>
#include "draw_driver.h"

typedef enum {
  OK,
  Wait,
  Loading,
  Err,
} astra_status_t;

extern void astra_draw_status_box(uint8_t _x, uint8_t _y, astra_status_t _status);
extern void astra_draw_data(uint8_t _x, uint8_t _y, double _data, char *_unit);
extern void astra_draw_status_bar(uint8_t _y, astra_status_t _status, double _data1, char *_unit1, double _data2, char *_unit2);

extern uint8_t terminal_x_min, terminal_y_min, terminal_x_max, terminal_y_max;
extern void astra_draw_terminal_prompter(uint8_t _x, uint8_t _y, char *_str);
extern void astra_draw_cursor(uint8_t _x, uint8_t _y, uint32_t _tick, uint8_t _blink_freq);
extern void astra_set_terminal_area(uint8_t _x_min, uint8_t _y_min, uint8_t _x_max, uint8_t _y_max);
extern bool astra_print_on_terminal(char *_type, char *_str);


#endif //FUCKCLION_CORE_SRC_ASTRA_UI_LITE_ASTRA_UI_ITEM_H_
