/**
 ******************************************************************************
 * @file    lv_100ask_stm32_tool_clock.c
 * @author  百问科技
 * @version V1.2
 * @date    2020-12-12
 * @brief	计算器应用
 ******************************************************************************
 * Change Logs:
 * Date           Author          Notes
 * 2020-12-12     zhouyuebiao     First version
 * 2021-01-25     zhouyuebiao     V1.2 
 ******************************************************************************
 * @attention
 *
 * Copyright (C) 2008-2021 深圳百问网科技有限公司<https://www.100ask.net/>
 * All rights reserved
 *
 ******************************************************************************
 */

/*********************
 *      INCLUDES
 *********************/
#include <stdio.h>
#include <stdlib.h>
#include "lv_clock.h"
#include <time.h>


/**********************
 *  STATIC VARIABLES
 **********************/

typedef struct _lv_clock
{
    lv_obj_t *time_label; // 时间标签
    lv_obj_t *date_label; // 日期标签
    lv_obj_t *weekday_label; // 星期标签
}lv_clock_t;


static void clock_date_task_callback(lv_timer_t *timer)
{
    static const char *week_day[7] = { "星期天","星期一","星期二","星期三","星期四","星期五","星期六" };
    static time_t unix_time;
    static struct tm *time_info;

    unix_time = time(NULL);
    time_info = localtime(&unix_time);

    int year = time_info->tm_year+1900;
    int month = time_info->tm_mon + 1;
    int day = time_info->tm_mday;
    int weekday = time_info->tm_wday;
    int hour = time_info->tm_hour;
    int minutes = time_info->tm_min;
    int second = time_info->tm_sec;

    if (timer != NULL && timer->user_data != NULL)
    {
        lv_clock_t * clock = (lv_clock_t *)(timer->user_data);
        if (clock->time_label != NULL)
        {
            lv_label_set_text_fmt(clock->time_label, "%02d:%02d:%02d", hour, minutes, second);
            lv_obj_align_to(clock->time_label, lv_obj_get_parent(clock->time_label), LV_ALIGN_CENTER, 0, 0);
        }

        if (clock->date_label != NULL)
        {
            lv_label_set_text_fmt(clock->date_label, "%d年%02d月%02d日", year, month, day);
            lv_obj_align_to(clock->date_label, lv_obj_get_parent(clock->date_label), LV_ALIGN_TOP_MID, 2, 0);
        }

        if (clock->weekday_label != NULL)
        {
            lv_label_set_text_fmt(clock->weekday_label, "%s", week_day[weekday]);
            lv_obj_align_to(clock->weekday_label, lv_obj_get_parent(clock->weekday_label), LV_ALIGN_BOTTOM_MID, -2, 0);
        }
    }
}


void lv_100ask_stm32_tool_clock()
{
    static lv_style_t date_time_clock_style; // 最外层对象的样式
    lv_style_reset(&date_time_clock_style); // 重置样式
    lv_style_init(&date_time_clock_style); // 初始化样式
    lv_style_set_radius(&date_time_clock_style, 5); // 设置样式圆角
    lv_style_set_bg_opa(&date_time_clock_style, LV_OPA_20); // 设置样式背景透明度
    lv_style_set_border_width(&date_time_clock_style, 0); // 设置样式边框宽度
    lv_style_set_bg_color(&date_time_clock_style, lv_color_white()); // 设置样式背景颜色，白色
    lv_style_set_pad_left(&date_time_clock_style, 1); // 设置样式左边padding填充宽度
    lv_style_set_pad_right(&date_time_clock_style, 1); // 设置样式右边padding填充宽度
    lv_style_set_pad_top(&date_time_clock_style, 0); // 设置样式顶部padding填充宽度
    lv_style_set_pad_bottom(&date_time_clock_style, 0); // 设置样式底部padding填充宽度

    static lv_style_t time_style; // 时间对象样式
    lv_style_reset(&time_style);
    lv_style_init(&time_style);
    lv_style_set_bg_opa(&time_style, LV_OPA_COVER);
    lv_style_set_border_width(&time_style, 0);
    lv_style_set_radius(&time_style, 5);
    lv_style_set_bg_color(&time_style, lv_palette_main(LV_PALETTE_BLUE));
    lv_style_set_pad_left(&time_style, 0);
    lv_style_set_pad_right(&time_style, 0);
    lv_style_set_pad_top(&time_style, 0);
    lv_style_set_pad_bottom(&time_style, 0);

    static lv_style_t date_style; // 日期对象样式
    lv_style_reset(&date_style);
    lv_style_init(&date_style);
    lv_style_set_bg_opa(&date_style, LV_OPA_COVER);
    lv_style_set_border_width(&date_style, 0);
    lv_style_set_radius(&date_style, 5);
    lv_style_set_bg_color(&date_style, lv_palette_main(LV_PALETTE_BLUE));
    lv_style_set_pad_left(&date_style, 0);
    lv_style_set_pad_right(&date_style, 0);

    /* Time font */
    static lv_style_t time_label_style; // 时间标签样式
    lv_style_reset(&time_label_style); // 重置样式
    lv_style_init(&time_label_style); // 初始化样式
    lv_style_set_text_color(&time_label_style , lv_color_white()); // 设置标签样式文本颜色
    lv_style_set_text_font(&time_label_style, &lv_font_montserrat_32); // 设置字体风格
    lv_style_set_text_opa(&time_label_style, LV_OPA_COVER); // 设置字体透明度
    lv_style_set_bg_opa(&time_label_style, LV_OPA_0); // 设置样式背景透明度

    /* Date font */
    static lv_style_t date_label_style; // 日期标签样式
    lv_style_reset(&date_label_style);
    lv_style_init(&date_label_style);
    lv_style_set_text_opa(&date_label_style, LV_OPA_COVER);
    lv_style_set_bg_opa(&date_label_style, LV_OPA_0);
    lv_style_set_text_color(&date_label_style , lv_color_white());
    lv_style_set_text_font(&date_label_style, &lv_font_simsun_16_cjk);

    /* Week font */
    static lv_style_t week_lable_style; // 日期标签样式
    lv_style_reset(&week_lable_style);
    lv_style_init(&week_lable_style);
    lv_style_set_text_opa(&week_lable_style, LV_OPA_COVER);
    lv_style_set_bg_opa(&week_lable_style, LV_OPA_0);
    lv_style_set_text_color(&week_lable_style, lv_color_white());
    lv_style_set_text_font(&week_lable_style, &lv_font_simsun_16_cjk);

    /* Time & Date */
    lv_obj_t *time_date_obj = lv_obj_create(lv_scr_act()); // 基于屏幕创建时间日期对象
    if (time_date_obj == NULL)
    {
        printf("[%s:%d] time_date_obj create failed\n", __FUNCTION__, __LINE__);
        return;
    }

    lv_obj_set_size(time_date_obj, 320, 100); // 设置对象大小
    lv_obj_center(time_date_obj); // 对象居屏幕中间显示
    lv_obj_add_style(time_date_obj, &date_time_clock_style, LV_STATE_DEFAULT); //给time_date_obj对象添加样式

    /*Time display*/
    lv_obj_t *time_obj = lv_obj_create(time_date_obj); // 基于time_date_obj对象创建时间对象
    if (time_obj == NULL)
    {
        printf("[%s:%d] time_obj create failed\n", __FUNCTION__, __LINE__);
        return;
    }

    lv_obj_set_size(time_obj, 158, 100); // 设置对象大小
    lv_obj_align_to(time_obj, time_date_obj, LV_ALIGN_LEFT_MID, 0, 0); // 设置time_obj对象基于time_date_obj对象左边中间对齐
    lv_obj_add_style(time_obj, &time_style, LV_STATE_DEFAULT);  // 给time_obj对象添加样式

    static lv_clock_t lv_clock = { 0 };

    lv_clock.time_label = lv_label_create(time_obj); // 基于time_obj对象创建时间显示标签对象 lv_clock.time_label
    if (lv_clock.time_label == NULL)
    {
        printf("[%s:%d] time_label create failed\n", __FUNCTION__, __LINE__);
        return ;
    }

    lv_obj_add_style(lv_clock.time_label, &time_label_style, LV_STATE_DEFAULT); // 给对象 lv_clock.time_label添加样式

    /*Date display*/
    lv_obj_t *date_obj = lv_obj_create(time_date_obj); // 基于time_date_obj对象创建date_obj对象
    if (date_obj == NULL)
    {
        printf("[%s:%d] date_obj create failed\n", __FUNCTION__, __LINE__);
        return ;
    }
    lv_obj_set_size(date_obj, 158, 100); // 设置对象大小
    lv_obj_align_to(date_obj, time_date_obj, LV_ALIGN_RIGHT_MID, 0, 0); //设置date_obj对象基于time_date_obj对象右边中部对齐
    lv_obj_add_style(date_obj, &date_style, LV_STATE_DEFAULT); // 给date_obj对象添加样式

    lv_clock.date_label = lv_label_create(date_obj); // 基于date_obj对象创建lv_clock.date_label日期显示对象
    if (lv_clock.date_label == NULL)
    {
        printf("[%s:%d] date_label create failed\n", __FUNCTION__, __LINE__);
        return ;
    }
    lv_obj_add_style(lv_clock.date_label, &date_label_style, LV_STATE_DEFAULT); // 给lv_clock.date_label对象添加样式

    /*Week display*/
    lv_clock.weekday_label = lv_label_create(date_obj); // 基于date_obj对象创建星期显示lv_clock.weekday_label对象
    if (lv_clock.weekday_label == NULL)
    {
        printf("[%s:%d] weekday_label create failed\n", __FUNCTION__, __LINE__);
        return;
    }
    lv_obj_add_style(lv_clock.weekday_label, &week_lable_style, LV_STATE_DEFAULT); // 给对象lv_clock.weekday_label添加样式

    // 设置时间标签lv_clock.time_label对象基于父对象居中对齐
    lv_obj_align_to(lv_clock.time_label, lv_obj_get_parent(lv_clock.time_label), LV_ALIGN_CENTER, 0, 0);
    // 设置时间标签lv_clock.date_label对象基于父对象顶部中间对齐
    lv_obj_align_to(lv_clock.date_label, lv_obj_get_parent(lv_clock.date_label), LV_ALIGN_TOP_MID, 2, 0);
    // 设置时间标签lv_clock.weekday_label对象基于父对象底部中间对齐
    lv_obj_align_to(lv_clock.weekday_label, lv_obj_get_parent(lv_clock.weekday_label), LV_ALIGN_BOTTOM_MID, -2, 0);

    lv_timer_t* task_timer = lv_timer_create(clock_date_task_callback, 200, (void *)&lv_clock); // 创建定时任务，200ms刷新一次
    if (task_timer == NULL)
    {
        printf("[%s:%d] lv_timer_create failed\n", __FUNCTION__, __LINE__);
    }
}
