//
// Created by Fir on 24-12-7.
//

#ifndef FUCKCLION_CORE_SRC_ASTRA_LAUNCHER_LAUNCHER_HOME_PAGE_H_
#define FUCKCLION_CORE_SRC_ASTRA_LAUNCHER_LAUNCHER_HOME_PAGE_H_

#include "launcher_adc.h"
#include "../astra-ui-lite/draw_driver.h"
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

#define LINE_HEIGHT 11

typedef enum {
  OK,
  Ready,
  Wait,
  Loading,
  Busy,
  Err,
  Stop,
} launcher_status_t;

typedef enum {
  info,
  uart,
  f411,
} launcher_terminal_prompter_t;

extern void launcher_draw_status_box(uint8_t _x, uint8_t _y, launcher_status_t _status);
extern void launcher_draw_data(uint8_t _x, uint8_t _y, double _data, char *_unit);
extern void launcher_draw_status_bar(uint8_t _y, launcher_status_t _status, double _data1, char *_unit1, double _data2, char *_unit2);

extern int8_t terminal_x_min;
extern int16_t terminal_y_min, terminal_x_max, terminal_y_max;
extern void launcher_draw_terminal_prompter(uint8_t _x, uint8_t _y, launcher_terminal_prompter_t _type);
extern void launcher_draw_cursor(uint8_t _x, uint8_t _y, uint32_t _tick, uint8_t _blink_freq);
extern void launcher_set_terminal_area(uint8_t _x_min, uint8_t _y_min, uint8_t _x_max, uint8_t _y_max);

#define TERMINAL_BUFFER_SIZE 10
extern int32_t terminal_buffer_size;

typedef struct terminal_buffer_t {
  int x, y;
  launcher_terminal_prompter_t type;
  char* str;
  struct terminal_buffer_t* next; //指向直接后继元素
} terminal_buffer_t;

extern terminal_buffer_t* terminal_buffer_head;

extern void launcher_push_str_to_terminal(launcher_terminal_prompter_t _type, char* _str);
extern void launcher_terminal_buffer_pop_front();
extern void launcher_terminal_print_test();

extern void launcher_draw_home_page();

#endif //FUCKCLION_CORE_SRC_ASTRA_LAUNCHER_LAUNCHER_HOME_PAGE_H_
