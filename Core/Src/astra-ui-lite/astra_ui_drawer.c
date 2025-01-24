//
// Created by forpaindream on 25-1-20.
//

#include "astra_ui_drawer.h"

void astra_draw_info_bar()
{
  if (!astra_info_bar.is_running) return;

  //弹窗到位后才开始计算时间
  if (astra_info_bar.y_info_bar == astra_info_bar.y_info_bar_trg) astra_info_bar.time = launcher_get_tick_ms();

  //时间到了就收回
  if (astra_info_bar.time - astra_info_bar.time_start >= astra_info_bar.span)
  {
    astra_info_bar.y_info_bar_trg = 0 - 2 * INFO_BAR_HEIGHT; //收回
    if (astra_info_bar.y_info_bar == astra_info_bar.y_info_bar_trg) astra_info_bar.is_running = false; //等归位后结束生命周期
  }

  oled_set_font(u8g2_font_my_chinese);
  oled_set_draw_color(1);
  oled_draw_R_box((int16_t)(OLED_WIDTH/2 - astra_info_bar.w_info_bar/2) + 3, (int16_t)(astra_info_bar.y_info_bar - 4) + 3,
                  (int16_t)(astra_info_bar.w_info_bar), INFO_BAR_HEIGHT + 4, 4);

  oled_set_draw_color(0); //黑遮罩打底
  oled_draw_R_box((int16_t)(OLED_WIDTH/2 - (astra_info_bar.w_info_bar + 4)/2), (int16_t)(astra_info_bar.y_info_bar - 4),
                  (int16_t)(astra_info_bar.w_info_bar + 4), INFO_BAR_HEIGHT + 6, 4);

  oled_set_draw_color(1);
  oled_draw_R_box((int16_t)(OLED_WIDTH/2 - astra_info_bar.w_info_bar/2), (int16_t)(astra_info_bar.y_info_bar - 4),
                    (int16_t)(astra_info_bar.w_info_bar), INFO_BAR_HEIGHT + 4, 3);
  //向上移动四个像素 同时向下多画四个像素 只用下半部分圆角

  oled_set_draw_color(2);
  oled_draw_H_line((int16_t)(OLED_WIDTH/2 - astra_info_bar.w_info_bar/2 + 2),
                   (int16_t)(astra_info_bar.y_info_bar + INFO_BAR_HEIGHT - 2),
                   (int16_t)(astra_info_bar.w_info_bar - 4));
  oled_draw_pixel((int16_t)(OLED_WIDTH/2 - astra_info_bar.w_info_bar/2 + 1),
                  (int16_t)(astra_info_bar.y_info_bar + INFO_BAR_HEIGHT - 3));
  oled_draw_pixel((int16_t)(OLED_WIDTH/2 + astra_info_bar.w_info_bar/2 - 2),
                  (int16_t)(astra_info_bar.y_info_bar + INFO_BAR_HEIGHT - 3));


  oled_draw_UTF8((int16_t)(OLED_WIDTH/2 - astra_info_bar.w_info_bar/2 + 6),
                 (int16_t)(astra_info_bar.y_info_bar + oled_get_str_height() - 2),
                 astra_info_bar.content);
}

void astra_draw_pop_up()
{
  if (!astra_pop_up.is_running) return;

  //弹窗到位后才开始计算时间
  if (astra_pop_up.y_pop_up == astra_pop_up.y_pop_up_trg) astra_pop_up.time = launcher_get_tick_ms();

  //时间到了就收回
  if (astra_pop_up.time - astra_pop_up.time_start >= astra_pop_up.span)
  {
    astra_pop_up.y_pop_up_trg = 0 - 2 * INFO_BAR_HEIGHT; //收回
    if (astra_pop_up.y_pop_up == astra_pop_up.y_pop_up_trg) astra_pop_up.is_running = false; //等归位后结束生命周期
  }

  oled_set_font(u8g2_font_my_chinese);
  oled_set_draw_color(1); //阴影打底
  oled_draw_R_box((int16_t)(OLED_WIDTH/2 - astra_pop_up.w_pop_up/2 + 1), (int16_t)astra_pop_up.y_pop_up + 3,
                    (int16_t)(astra_pop_up.w_pop_up + 4),
                    POP_UP_HEIGHT, 4);

  oled_set_draw_color(0); //黑遮罩
  oled_draw_R_box((int16_t)(OLED_WIDTH/2 - (astra_pop_up.w_pop_up + 4)/2 - 2), (int16_t)(astra_pop_up.y_pop_up - 2),
                  (int16_t)(astra_pop_up.w_pop_up + 8), POP_UP_HEIGHT + 4, 5);

  oled_set_draw_color(1);
  oled_draw_R_box((int16_t)(OLED_WIDTH/2 - astra_pop_up.w_pop_up/2 - 2), (int16_t)astra_pop_up.y_pop_up,
                    (int16_t)(astra_pop_up.w_pop_up + 4),
                    POP_UP_HEIGHT, 3);

  oled_set_draw_color(2);
  oled_draw_H_line((int16_t)(OLED_WIDTH/2 - astra_pop_up.w_pop_up/2),
                   (int16_t)(astra_pop_up.y_pop_up + POP_UP_HEIGHT - 2),
                   (int16_t)astra_pop_up.w_pop_up);
  oled_draw_pixel((int16_t)(OLED_WIDTH/2 - astra_pop_up.w_pop_up/2 - 1),
                  (int16_t)(astra_pop_up.y_pop_up + POP_UP_HEIGHT - 3));
  oled_draw_pixel((int16_t)(OLED_WIDTH/2 + astra_pop_up.w_pop_up/2),
                  (int16_t)(astra_pop_up.y_pop_up + POP_UP_HEIGHT - 3));

  oled_draw_UTF8((int16_t)(OLED_WIDTH/2 - astra_pop_up.w_pop_up/2 + 3),
                 (int16_t)(astra_pop_up.y_pop_up + oled_get_str_height() + 1),
                 astra_pop_up.content);
}

void astra_draw_list_item()
{
  //todo 顶部状态栏样式绘制测试 后期删除掉
  oled_draw_H_line(0, 1, 66);
  oled_draw_H_line(0, 0, 67);
  for (uint8_t i = 67; i <= 99; i++) if (i % 2 == 1) oled_draw_pixel(i, 1);
  for (uint8_t i = 68; i <= 100; i++) if (i % 2 == 0) oled_draw_pixel(i, 0);
  for (uint8_t i = 101; i <= 111; i++) if (i % 3 == 0) oled_draw_pixel(i, 1);
  for (uint8_t i = 102; i <= 112; i++) if (i % 3 == 1) oled_draw_pixel(i, 0);
  for (uint8_t i = 112; i <= 124; i++) if (i % 5 == 0) oled_draw_pixel(i, 1);
  for (uint8_t i = 114; i <= 124; i++) if (i % 5 == 1) oled_draw_pixel(i, 0);
  // oled_draw_pixel(126, 1);

  //右侧进度条
  oled_draw_V_line(123, 0, 64);
  oled_draw_V_line(127, 0, 64);
  oled_draw_box(124, 10, 3, 20);
  oled_set_draw_color(0);
  oled_draw_H_line(124,18,3);
  oled_draw_H_line(124,20,3);
  oled_draw_H_line(124,22,3);
  oled_set_draw_color(1);
  oled_draw_box(124, 0, 3, 4);
  oled_draw_box(124, 60, 3, 4);
  oled_set_draw_color(0);
  oled_draw_H_line(124, 2, 3);
  oled_draw_pixel(125, 1);
  oled_draw_H_line(124, 61, 3);
  oled_draw_pixel(125, 62);


  //todo 这里后期都要改成使用pointer来绘制 不能从根节点出发
  //todo 这里所有的trg都要改成坐标本身 加入camera之后
  oled_set_font(u8g2_font_my_chinese);
  for (unsigned char i = 0; i < astra_list_item_root.child_num; i++)
  {
    oled_set_draw_color(1);
    //绘制开头的指示器
    if (astra_list_item_root.child_list_item[i]->type == list_item)
    {
      oled_draw_H_line(2, astra_list_item_root.child_list_item[i]->y_list_item_trg - oled_get_str_height() / 2 - 2, 4);
      oled_draw_H_line(2, astra_list_item_root.child_list_item[i]->y_list_item_trg - oled_get_str_height() / 2, 5);
      oled_draw_H_line(2, astra_list_item_root.child_list_item[i]->y_list_item_trg - oled_get_str_height() / 2 + 2, 3);
    }
    else if (astra_list_item_root.child_list_item[i]->type == switch_item)
    {
      oled_draw_circle(4, astra_list_item_root.child_list_item[i]->y_list_item_trg - oled_get_str_height() / 2 + 1, 3);
      oled_draw_V_line(4, astra_list_item_root.child_list_item[i]->y_list_item_trg - oled_get_str_height() / 2 , 3);
    }
    else if (astra_list_item_root.child_list_item[i]->type == button_item)
    {
      oled_draw_box(2, astra_list_item_root.child_list_item[i]->y_list_item_trg - oled_get_str_height() / 2 - 1, 4, 4);
      oled_draw_H_line(3, astra_list_item_root.child_list_item[i]->y_list_item_trg - oled_get_str_height() / 2 + 4, 4);
      oled_draw_V_line(7, astra_list_item_root.child_list_item[i]->y_list_item_trg - oled_get_str_height() / 2, 5);
    }
    else if (astra_list_item_root.child_list_item[i]->type == slider_item)
    {
      oled_draw_V_line(3, astra_list_item_root.child_list_item[i]->y_list_item_trg - oled_get_str_height() / 2 - 1, 5);
      oled_draw_V_line(6, astra_list_item_root.child_list_item[i]->y_list_item_trg - oled_get_str_height() / 2 - 1, 5);
      oled_draw_box(2, astra_list_item_root.child_list_item[i]->y_list_item_trg - oled_get_str_height() / 2 - 2, 3, 3);
      oled_draw_box(5, astra_list_item_root.child_list_item[i]->y_list_item_trg - oled_get_str_height() / 2 + 2, 3, 3);
    }
    else
    {
      oled_draw_str(0, astra_list_item_root.child_list_item[i]->y_list_item_trg, "-");
    }
    oled_draw_UTF8(10, astra_list_item_root.child_list_item[i]->y_list_item_trg, astra_list_item_root.child_list_item[i]->content);
  }
}

void astra_draw_widget()
{
  //需要调用所有的控件draw函数 需要在core后面执行 否则会被core覆盖
  astra_draw_info_bar();
  astra_draw_pop_up();
}

void astra_draw_core()
{
  //需要调用所有的主页面draw函数
  astra_draw_list_item();
}
