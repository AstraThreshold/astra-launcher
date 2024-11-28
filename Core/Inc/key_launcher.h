//
// Created by Fir on 24-11-26.
//

#ifndef KEY_H_
#define KEY_H_

#include "stdbool.h"
#include <main.h>
#include <string.h>

typedef enum keyFilter {
  CHECKING = 0,
  KEY_0_CONFIRM,
  KEY_1_CONFIRM,
  RELEASED,
} KEY_FILTER;

typedef enum keyAction {
  INVALID = 0,
  CLICK,
  PRESS,
} KEY_ACTION;

typedef enum KeyType {
  KEY_NOT_PRESSED = 0,
  KEY_PRESSED,
} KEY_TYPE;

typedef enum keyIndex {
  KEY_0 = 0,
  KEY_1,
  KEY_NUM,
} KEY_INDEX;

extern KEY_ACTION key[KEY_NUM];
extern KEY_TYPE keyFlag;

extern bool getKey(KEY_INDEX _keyIndex);
extern bool getAnyKey();
extern KEY_ACTION *getKeyMap();
extern KEY_TYPE *getKeyFlag();
extern void keyScan();
extern void keyTest();

extern void keyCallBack(uint8_t _freq, void(*_key1Clicked)(), void(*_key2Clicked)(), void(*_key1Pressed)(), void(*_key2Pressed)());

extern uint8_t key1Cnt;
extern uint8_t key2Cnt;

extern void key1Clicked();
extern void key2Clicked();
extern void key1Pressed();
extern void key2Pressed();

#endif
