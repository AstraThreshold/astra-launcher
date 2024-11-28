//
// Created by Fir on 24-11-28.
//

#ifndef FUCKCLION_CORE_SRC_OLED_LAUNCHER_H_
#define FUCKCLION_CORE_SRC_OLED_LAUNCHER_H_

#include "../Src/u8g2/u8g2.h"
#include "main.h"
#include "spi.h"

#define OLED_DC_Clr() HAL_GPIO_WritePin(OLED_DC_GPIO_Port, OLED_DC_Pin, GPIO_PIN_RESET)
#define OLED_DC_Set() HAL_GPIO_WritePin(OLED_DC_GPIO_Port, OLED_DC_Pin, GPIO_PIN_SET)
#define OLED_RST_Clr() HAL_GPIO_WritePin(OLED_RST_GPIO_Port, OLED_RST_Pin, GPIO_PIN_RESET)
#define OLED_RST_Set() HAL_GPIO_WritePin(OLED_RST_GPIO_Port, OLED_RST_Pin, GPIO_PIN_SET)

extern void u8g2Init(u8g2_t *u8g2);
extern void oledInit();
extern u8g2_t u8g2;

#endif //FUCKCLION_CORE_SRC_OLED_LAUNCHER_H_
