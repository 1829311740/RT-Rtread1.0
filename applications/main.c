/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-5-10      ShiHao       first version
 */

#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>
#include "humi.h"
#include <drv_lcd.h>
#include <rttlogo.h>
#include <drv_neo_pixel.h>
#include <uart2.h>
#define DBG_TAG "main"
#define DBG_LVL         DBG_LOG
#include <rtdbg.h>
#define PWM_DEV_NAME        "pwm4"  /* PWM设备名称 */
#define PWM_DEV_CHANNEL     1       /* PWM通道 */      //pb6
#define LED_NUMS   (19 + 30) /* LED 的数目 */
#define LED_BRIGHTNESS 128   /* LED 的亮度 */
#define LED_REPS       1     /* LED 颜色的周期数 */
#define LED_SATURATION 255   /* LED 的饱和度 */



int main(void)
{
    rt_uint16_t humi,temp;
    ahtt();
    lcd_clear(BLACK);
    lcd_show_image(0, 0, 64, 64, image_rttlogo);
    lcd_show_image(65, 0, 165, 64, gImage);
    lcd_set_color(BLACK,WRITE  );
    neo_pixel_ops_t *neo_ops;
       uint16_t hue = 7; /* 第一个灯珠的颜色 */

       /* 初始化 neo_pixel 库 */
       neo_pixel_init(&neo_ops, LED_NUMS);
       /* 等待驱动初始化完成 */
       rt_thread_mdelay(10);
    while (1)
       {
        //lcd_show_num(65,0,humidity=ahtt1(),2,32);
            lcd_show_chinese(50, 90, 0,32);
            lcd_show_chinese(82, 90, 1,32);
            lcd_show_chinese(112, 90, 2,32);
            lcd_show_num(145,90,temp=ahtt2(),2,32);
            lcd_show_chinese2(176, 95, 0,24);
            lcd_show_chinese(50, 144, 3,32);
            lcd_show_chinese(82, 144, 4,32);
            lcd_show_chinese(112, 144, 5,32);
            lcd_show_num(144,144,humi=ahtt1(),2,32);
            lcd_show_string(176,144, 32, "%");
            rt_thread_mdelay(1000);
            neo_ops->tool->rainbow(hue, LED_REPS, LED_SATURATION, LED_BRIGHTNESS, RT_TRUE);
                   /* 显示 */
                   neo_ops->show();
                   /* 刷新周期10ms */
                   rt_thread_mdelay(10);
                   hue += 100;
       }

    return 0;
}

