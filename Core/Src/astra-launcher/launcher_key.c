//
// Created by Fir on 24-11-26.
//

#include "launcher_key.h"

KEY_ACTION key[KEY_NUM] = {INVALID};
KEY_TYPE keyFlag = KEY_NOT_PRESSED;

bool launcher_get_key(KEY_INDEX _keyIndex)
{
  if (_keyIndex == KEY_0) return !HAL_GPIO_ReadPin(KEY1_GPIO_Port, KEY1_Pin);
  if (_keyIndex == KEY_1) return !HAL_GPIO_ReadPin(KEY2_GPIO_Port, KEY2_Pin);
}

bool launcher_get_any_key()
{
  for (int i = 0; i < KEY_NUM; i++)
  {
    if (launcher_get_key((KEY_INDEX) (i))) return true;
  }
  return false;
}

KEY_ACTION *launcher_get_key_map()
{
  return key;
}

KEY_TYPE *launcher_get_key_flag()
{
  return &keyFlag;
}

/**
 * @brief key scanner default. 默认按键扫描函数
 *
 * @note run per 5 ms.
 * @return keyValue
 */
void launcher_key_scan()
{
  static unsigned char _timeCnt = 0;
  static bool _lock = false;
  static KEY_FILTER _keyFilter = CHECKING;
  switch (_keyFilter)
  {
    case CHECKING:
      if (launcher_get_any_key())
      {
        if (launcher_get_key(KEY_0)) _keyFilter = KEY_0_CONFIRM;
        if (launcher_get_key(KEY_1)) _keyFilter = KEY_1_CONFIRM;
      }
      _timeCnt = 0;
      _lock = false;
      break;

    case KEY_0_CONFIRM:
    case KEY_1_CONFIRM:
      //filter
      if (launcher_get_any_key())
      {
        if (!_lock) _lock = true;
        _timeCnt++;

        //timer
        if (_timeCnt > 50)
        {
          keyFlag = KEY_PRESSED;
          //long press 1 s
          if (launcher_get_key(KEY_0))
          {
            key[KEY_0] = PRESS;
            key[KEY_1] = INVALID;
          }
          if (launcher_get_key(KEY_1))
          {
            key[KEY_1] = PRESS;
            key[KEY_0] = INVALID;
          }
          _timeCnt = 0;
          _lock = false;
          _keyFilter = RELEASED;
        }
      } else
      {
        if (_lock)
        {
          if (_keyFilter == KEY_0_CONFIRM)
          {
            key[KEY_0] = CLICK;
            key[KEY_1] = INVALID;
          }
          if (_keyFilter == KEY_1_CONFIRM)
          {
            key[KEY_1] = CLICK;
            key[KEY_0] = INVALID;
          }
          keyFlag = KEY_PRESSED;
          _keyFilter = RELEASED;
        } else
        {
          _keyFilter = CHECKING;
          key[KEY_0] = INVALID;
          key[KEY_1] = INVALID;
        }
      }
      break;

    case RELEASED:
      if (!launcher_get_any_key()) _keyFilter = CHECKING;
      break;

    default: break;
  }
}


/**
 * @brief default key tester. 默认按键测试函数
 */
void launcher_key_test()
{
  if (launcher_get_any_key())
  {
    for (unsigned char i = 0; i < KEY_NUM; i++)
    {
      if (key[i] == CLICK)
      {
        //do something when key clicked
        if (i == 0) break;
        if (i == 1) break;
      } else if (key[i] == PRESS)
      {
        //do something when key pressed
        if (i == 0) break;
        if (i == 1) break;
      }
    }
    memset(key, INVALID, sizeof(key));
  }
}

void launcher_key_call_back(uint8_t _freq, void (*_key1Clicked)(), void (*_key2Clicked)(), void (*_key1Pressed)(),
                            void (*_key2Pressed)())
{
  uint8_t _tick = HAL_GetTick() / 1000;
  if (_tick > _freq)
  {
    launcher_key_scan();
    _tick = 0;
  }

  if (*launcher_get_key_flag() == KEY_PRESSED)
  {
    *launcher_get_key_flag() = KEY_NOT_PRESSED;
    for (unsigned char i = 0; i < KEY_NUM; i++)
    {
      if (launcher_get_key_map()[i] == CLICK)
      {
        if (i == 0) { _key1Clicked(); } else if (i == 1) { _key2Clicked(); }
      } else if (launcher_get_key_map()[i] == PRESS)
      {
        if (i == 0) { _key1Pressed(); } else if (i == 1) { _key2Pressed(); }
      }
    }
    memset(launcher_get_key_map(), INVALID, sizeof(*launcher_get_key_map()));
    *launcher_get_key_flag() = KEY_NOT_PRESSED;
  }
}

uint8_t key1Cnt = 0;
uint8_t key2Cnt = 0;

void key1Clicked() { key1Cnt++; }
void key2Clicked() { key2Cnt++; }
void key1Pressed() { key1Cnt = 0; }
void key2Pressed() { key2Cnt = 0; }
