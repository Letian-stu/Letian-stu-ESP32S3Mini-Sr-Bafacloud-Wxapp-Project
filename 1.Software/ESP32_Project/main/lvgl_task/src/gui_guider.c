/*
 * @Author: StuTian
 * @Date: 2022-09-05 14:07
 * @LastEditors: Letian-stu
 * @LastEditTime: 2023-03-12 13:27
 * @FilePath: /ESP32_Project/main/lvgl_task/src/gui_guider.c
 * @Description:
 * Copyright (c) 2022 by StuTian 1656733975@qq.com, All Rights Reserved.
 */
/*
 * Copyright 2022 NXP
 * SPDX-License-Identifier: MIT
 */
#include <stdio.h>
#include "string.h"
#include "lvgl/lvgl.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "gui_guider.h"
#include "gui_anim.h"
#include "app_task.h"

#define TAG "UI"

#define HOME_PAGE_OUT_TIME 300
#define BOOT_PAGE_OUT_TIME 5000
enum PAGE page = PAGE_HOME;

static const lv_coord_t obj_width = 110;
static const lv_coord_t obj_height = 110;

/**
 * @description: 
 * @param {uint16_t} pagenum
 * @return {*}
 */
static void add_home_group_obj(uint16_t pagenum)
{
    lv_group_remove_all_objs(guider_ui.group);

    switch (pagenum)
    {
    case PAGE_WEATHER:
        lv_group_add_obj(guider_ui.group, guider_ui.weather_btn);
        lv_group_add_obj(guider_ui.group, guider_ui.set_btn);
        lv_group_add_obj(guider_ui.group, guider_ui.camera_btn);
        lv_group_add_obj(guider_ui.group, guider_ui.image_btn);
        lv_group_add_obj(guider_ui.group, guider_ui.web_btn);
        break;
    case PAGE_CONTROL:
        lv_group_add_obj(guider_ui.group, guider_ui.set_btn);
        lv_group_add_obj(guider_ui.group, guider_ui.camera_btn);
        lv_group_add_obj(guider_ui.group, guider_ui.image_btn);
        lv_group_add_obj(guider_ui.group, guider_ui.web_btn);
        lv_group_add_obj(guider_ui.group, guider_ui.weather_btn);
        break;
    case PAGE_CAMERA:
        lv_group_add_obj(guider_ui.group, guider_ui.camera_btn);
        lv_group_add_obj(guider_ui.group, guider_ui.image_btn);
        lv_group_add_obj(guider_ui.group, guider_ui.web_btn);
        lv_group_add_obj(guider_ui.group, guider_ui.weather_btn);
        lv_group_add_obj(guider_ui.group, guider_ui.set_btn);
        break;
    case PAGE_IMAGE:
        lv_group_add_obj(guider_ui.group, guider_ui.image_btn);
        lv_group_add_obj(guider_ui.group, guider_ui.web_btn);
        lv_group_add_obj(guider_ui.group, guider_ui.weather_btn);
        lv_group_add_obj(guider_ui.group, guider_ui.set_btn);
        lv_group_add_obj(guider_ui.group, guider_ui.camera_btn);
        break;
    case PAGE_WEBCARD:
        lv_group_add_obj(guider_ui.group, guider_ui.web_btn);
        lv_group_add_obj(guider_ui.group, guider_ui.weather_btn);
        lv_group_add_obj(guider_ui.group, guider_ui.set_btn);
        lv_group_add_obj(guider_ui.group, guider_ui.camera_btn);
        lv_group_add_obj(guider_ui.group, guider_ui.image_btn);
        break;
    }
}

/**
 * @description: 
 * @return {*}
 */
static void remove_home_group_obj(void)
{
    lv_group_remove_obj(guider_ui.weather_btn);
    lv_group_remove_obj(guider_ui.set_btn);
    lv_group_remove_obj(guider_ui.camera_btn);
    lv_group_remove_obj(guider_ui.image_btn);
    lv_group_remove_obj(guider_ui.web_btn);
    if (guider_ui.back_btn != NULL)
    {
        lv_group_add_obj(guider_ui.group, guider_ui.back_btn);
    }
}

/**
 * @description: 
 * @param {lv_event_t} *e
 * @return {*}
 */
void lv_btn_back_event_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code)
    {
    case LV_EVENT_CLICKED:
        if(Cam_Handle != NULL)
        {
            vTaskSuspend(Cam_Handle);
        }
        page_screen_anim(guider_ui.page, 0, 240, HOME_PAGE_OUT_TIME, 100, (lv_anim_exec_xcb_t)lv_obj_set_y, lv_anim_path_linear);
        page_screen_anim(guider_ui.home, -240, 0, HOME_PAGE_OUT_TIME, 100, (lv_anim_exec_xcb_t)lv_obj_set_y, lv_anim_path_linear);
        add_home_group_obj(page);
        break;
    case LV_EVENT_FOCUSED:
    default:
        break;
    }
}

/**
 * @description: 
 * @param {lv_event_t} *e
 * @return {*}
 */
static void lv_btn_event_cb(lv_event_t *e)
{
    lv_obj_t *obj = lv_event_get_target(e);
    lv_event_code_t code = lv_event_get_code(e);

    if (obj == guider_ui.weather_btn)
    {
        switch (code)
        {
        case LV_EVENT_CLICKED:
            page_screen_anim(guider_ui.home, 0, 240, HOME_PAGE_OUT_TIME, 0, (lv_anim_exec_xcb_t)lv_obj_set_y, lv_anim_path_linear);
            setup_weather_screen(&guider_ui, HOME_PAGE_OUT_TIME, 0);
            remove_home_group_obj();
            page = PAGE_WEATHER;
            break;
        case LV_EVENT_FOCUSED:
            lv_label_set_text(guider_ui.home_label, "Weather:天气");
            break;
        default:
            break;
        }
    }
    else if (obj == guider_ui.set_btn)
    {
        switch (code)
        {
        case LV_EVENT_CLICKED:
            page_screen_anim(guider_ui.home, 0, 240, HOME_PAGE_OUT_TIME, 0, (lv_anim_exec_xcb_t)lv_obj_set_y, lv_anim_path_linear);
            setup_set_screen(&guider_ui, HOME_PAGE_OUT_TIME, 0);
            remove_home_group_obj();
            lv_group_add_obj(guider_ui.group, guider_ui.ledbtn);
            lv_group_add_obj(guider_ui.group, guider_ui.fanbtn);
            lv_group_add_obj(guider_ui.group, guider_ui.keybtn);
            page = PAGE_CONTROL;
            break;
        case LV_EVENT_FOCUSED:
            lv_label_set_text(guider_ui.home_label, "Control:控制");
            break;
        default:
            break;
        }
    }
    else if (obj == guider_ui.camera_btn)
    {
        switch (code)
        {
        case LV_EVENT_CLICKED:
            page_screen_anim(guider_ui.home, 0, 240, HOME_PAGE_OUT_TIME, 0, (lv_anim_exec_xcb_t)lv_obj_set_y, lv_anim_path_linear);
            setup_camera_screen(&guider_ui, HOME_PAGE_OUT_TIME, 0);
            remove_home_group_obj();
            lv_group_add_obj(guider_ui.group, guider_ui.takepic_btn);
            page = PAGE_CAMERA;
            break;
        case LV_EVENT_FOCUSED:
            lv_label_set_text(guider_ui.home_label, "Camera:拍照");
            break;
        default:
            break;
        }
    }
    else if (obj == guider_ui.image_btn)
    {
        switch (code)
        {
        case LV_EVENT_CLICKED:
            page_screen_anim(guider_ui.home, 0, 240, HOME_PAGE_OUT_TIME, 0, (lv_anim_exec_xcb_t)lv_obj_set_y, lv_anim_path_linear);
            setup_image_screen(&guider_ui, HOME_PAGE_OUT_TIME, 0);
            remove_home_group_obj();
            page = PAGE_IMAGE;
            break;
        case LV_EVENT_FOCUSED:
            lv_label_set_text(guider_ui.home_label, "Photo:图片");
            break;
        default:
            break;
        }
    }
    else if (obj == guider_ui.web_btn)
    {
        switch (code)
        {
        case LV_EVENT_CLICKED:
            page_screen_anim(guider_ui.home, 0, 240, HOME_PAGE_OUT_TIME, 0, (lv_anim_exec_xcb_t)lv_obj_set_y, lv_anim_path_linear);
            setup_web_screen(&guider_ui, HOME_PAGE_OUT_TIME, 0);
            remove_home_group_obj();
            page = PAGE_WEBCARD;
            break;
        case LV_EVENT_FOCUSED:
            lv_label_set_text(guider_ui.home_label, "Web:网页");
            break;
        default:
            break;
        }
    }
}

static void set_temp(void *bar, int32_t temp)
{
    lv_bar_set_value(bar, temp, LV_ANIM_ON);
}

/**
 * @description: 
 * @param {lv_ui} *ui
 * @return {*}
 */
void setup_boot_screen(lv_ui *ui)
{
    ui->boot = lv_obj_create(lv_scr_act());
    lv_obj_set_size(ui->boot, 280, 240);
    lv_obj_align(ui->boot, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_scrollbar_mode(ui->boot, LV_SCROLLBAR_MODE_OFF);

    static lv_style_t style_indic;
    lv_style_init(&style_indic);
    lv_style_set_bg_img_src(&style_indic, &img_skew_strip);
    lv_style_set_bg_img_tiled(&style_indic, true);
    lv_style_set_bg_img_opa(&style_indic, LV_OPA_30);

    ui->user_img = lv_img_create(ui->boot);
    lv_obj_set_size(ui->user_img, 200, 60);
    lv_obj_align(ui->user_img, LV_ALIGN_CENTER, 0, -50);
    lv_img_set_src(ui->user_img, &_StuLeTian_200x60);

    ui->bar = lv_bar_create(ui->boot);
    lv_obj_add_style(ui->bar, &style_indic, LV_PART_INDICATOR);
    lv_obj_set_size(ui->bar, 250, 20); // 15
    lv_obj_align(ui->bar, LV_ALIGN_CENTER, 0, 60);
    lv_bar_set_mode(ui->bar, LV_BAR_MODE_RANGE);
    lv_bar_set_value(ui->bar, 0, LV_ANIM_ON);
    
    ui->bar_img = lv_img_create(ui->boot);
    lv_obj_align(ui->bar_img, LV_ALIGN_LEFT_MID, 0, 20);
    lv_img_set_src(ui->bar_img, &_rocket_60x35);

    page_screen_anim(ui->bar, 0, 100, BOOT_PAGE_OUT_TIME, (uint32_t)NULL, (lv_anim_exec_xcb_t)set_temp, lv_anim_path_ease_in);
    page_screen_anim(ui->bar_img, 0, 200, BOOT_PAGE_OUT_TIME, (uint32_t)NULL, (lv_anim_exec_xcb_t)lv_obj_set_x, lv_anim_path_ease_in);

    lv_obj_del_delayed(ui->boot, BOOT_PAGE_OUT_TIME);
}

/**
 * @description: 
 * @param {lv_ui} *ui
 * @param {uint32_t} delay
 * @return {*}
 */
void setup_home_screen(lv_ui *ui, uint32_t delay)
{
    ui->home = lv_obj_create(lv_scr_act());
    lv_obj_set_size(ui->home, 280, 240);
    lv_obj_align(ui->home, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_scrollbar_mode(ui->home, LV_SCROLLBAR_MODE_OFF);

    // ui->home_http_label = lv_label_create(ui->home);
    // lv_obj_set_size(ui->home_http_label, 270, 30);
    // lv_obj_align(ui->home_http_label, LV_ALIGN_TOP_MID, 0, 10);
    // lv_label_set_long_mode(ui->home_http_label, LV_LABEL_LONG_SCROLL_CIRCULAR); 
    // lv_obj_set_style_text_font(ui->home_http_label, &myFont, 0);
    // lv_label_set_text(ui->home_http_label, "NULL");

    ui->home_label = lv_label_create(ui->home);
    lv_obj_align(ui->home_label, LV_ALIGN_BOTTOM_MID, 0, -20);
    lv_obj_set_style_text_font(ui->home_label, &myFont, 0);
    lv_label_set_text(ui->home_label, "Clock");

    ui->panel = lv_obj_create(ui->home);
    lv_obj_remove_style_all(ui->panel);
    lv_obj_align(ui->panel, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_size(ui->panel, 280, 130);
    lv_obj_set_scroll_snap_x(ui->panel, LV_SCROLL_SNAP_CENTER);
    lv_obj_set_flex_align(ui->panel, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_flex_flow(ui->panel, LV_FLEX_FLOW_ROW);
    lv_obj_set_scrollbar_mode(ui->panel, LV_SCROLLBAR_MODE_OFF);

    ui->weather_btn = lv_btn_create(ui->panel);
    lv_obj_remove_style_all(ui->weather_btn);                                                     
    lv_obj_set_size(ui->weather_btn, obj_width, obj_height);                                           
    lv_obj_set_style_radius(ui->weather_btn, 20, 0);                                               
    lv_obj_set_style_bg_color(ui->weather_btn, lv_color_hex(COLOR_DODGER_BLUE), 0);                
    lv_obj_set_style_bg_opa(ui->weather_btn, LV_OPA_0, 0);                                         
    lv_obj_set_style_bg_color(ui->weather_btn, lv_color_hex(COLOR_DODGER_BLUE), LV_STATE_FOCUSED); 
    lv_obj_set_style_bg_opa(ui->weather_btn, LV_OPA_20, LV_STATE_FOCUSED);                         
    lv_obj_set_style_bg_opa(ui->weather_btn, LV_OPA_50, LV_STATE_PRESSED);                         
    lv_obj_add_event_cb(ui->weather_btn, lv_btn_event_cb, LV_EVENT_ALL, NULL);
    ui->weather = lv_img_create(ui->weather_btn);
    lv_obj_set_size(ui->weather, obj_width, obj_height);
    lv_obj_align(ui->weather, LV_ALIGN_CENTER, 0, 0);
    lv_img_set_src(ui->weather, &_weather_110x110);
    
    ui->set_btn = lv_btn_create(ui->panel);
    lv_obj_remove_style_all(ui->set_btn);                                                
    lv_obj_set_size(ui->set_btn, obj_width, obj_height);                                        
    lv_obj_set_style_radius(ui->set_btn, 20, 0);                                           
    lv_obj_set_style_bg_color(ui->set_btn, lv_color_hex(COLOR_DODGER_BLUE), 0);               
    lv_obj_set_style_bg_opa(ui->set_btn, LV_OPA_0, 0);                                         
    lv_obj_set_style_bg_color(ui->set_btn, lv_color_hex(COLOR_DODGER_BLUE), LV_STATE_FOCUSED); 
    lv_obj_set_style_bg_opa(ui->set_btn, LV_OPA_20, LV_STATE_FOCUSED);                         
    lv_obj_set_style_bg_opa(ui->set_btn, LV_OPA_50, LV_STATE_PRESSED);                        
    lv_obj_add_event_cb(ui->set_btn, lv_btn_event_cb, LV_EVENT_ALL, NULL);
    ui->set = lv_img_create(ui->set_btn);
    lv_obj_set_size(ui->set, obj_width, obj_height);
    lv_obj_align(ui->set, LV_ALIGN_CENTER, 0, 0);
    lv_img_set_src(ui->set, &_set_110x110);

    ui->camera_btn = lv_btn_create(ui->panel);
    lv_obj_remove_style_all(ui->camera_btn);                                                  
    lv_obj_set_size(ui->camera_btn, obj_width, obj_height);                                         
    lv_obj_set_style_radius(ui->camera_btn, 20, 0);                                           
    lv_obj_set_style_bg_color(ui->camera_btn, lv_color_hex(COLOR_DODGER_BLUE), 0);               
    lv_obj_set_style_bg_opa(ui->camera_btn, LV_OPA_0, 0);                                         
    lv_obj_set_style_bg_color(ui->camera_btn, lv_color_hex(COLOR_DODGER_BLUE), LV_STATE_FOCUSED); 
    lv_obj_set_style_bg_opa(ui->camera_btn, LV_OPA_20, LV_STATE_FOCUSED);                         
    lv_obj_set_style_bg_opa(ui->camera_btn, LV_OPA_50, LV_STATE_PRESSED);                        
    lv_obj_add_event_cb(ui->camera_btn, lv_btn_event_cb, LV_EVENT_ALL, NULL);
    ui->camera = lv_img_create(ui->camera_btn);
    lv_obj_set_size(ui->camera, obj_width, obj_height);
    lv_obj_align(ui->camera, LV_ALIGN_CENTER, 0, 0);
    lv_img_set_src(ui->camera, &_camera_110x110);

    ui->image_btn = lv_btn_create(ui->panel);
    lv_obj_remove_style_all(ui->image_btn);                                                      
    lv_obj_set_size(ui->image_btn, obj_width, obj_height);                                           
    lv_obj_set_style_radius(ui->image_btn, 20, 0);                                               
    lv_obj_set_style_bg_color(ui->image_btn, lv_color_hex(COLOR_DODGER_BLUE), 0);                
    lv_obj_set_style_bg_opa(ui->image_btn, LV_OPA_0, 0);                                         
    lv_obj_set_style_bg_color(ui->image_btn, lv_color_hex(COLOR_DODGER_BLUE), LV_STATE_FOCUSED); 
    lv_obj_set_style_bg_opa(ui->image_btn, LV_OPA_20, LV_STATE_FOCUSED);                         
    lv_obj_set_style_bg_opa(ui->image_btn, LV_OPA_50, LV_STATE_PRESSED);                         
    lv_obj_add_event_cb(ui->image_btn, lv_btn_event_cb, LV_EVENT_ALL, NULL);
    ui->image = lv_img_create(ui->image_btn);
    lv_obj_set_size(ui->image, obj_width, obj_height);
    lv_obj_align(ui->image, LV_ALIGN_CENTER, 0, 0);
    lv_img_set_src(ui->image, &_image_110x110);

    ui->web_btn = lv_btn_create(ui->panel);
    lv_obj_remove_style_all(ui->web_btn);                                      
    lv_obj_set_size(ui->web_btn, obj_width, obj_height);                            
    lv_obj_set_style_radius(ui->web_btn, 20, 0);                                
    lv_obj_set_style_bg_color(ui->web_btn, lv_color_hex(COLOR_DODGER_BLUE), 0); 
    lv_obj_set_style_bg_opa(ui->web_btn, LV_OPA_0, 0);                          
    lv_obj_set_style_bg_color(ui->web_btn, lv_color_hex(COLOR_DODGER_BLUE), LV_STATE_FOCUSED);
    lv_obj_set_style_bg_opa(ui->web_btn, LV_OPA_20, LV_STATE_FOCUSED); 
    lv_obj_set_style_bg_opa(ui->web_btn, LV_OPA_50, LV_STATE_PRESSED); 
    lv_obj_add_event_cb(ui->web_btn, lv_btn_event_cb, LV_EVENT_ALL, NULL);
    ui->web = lv_img_create(ui->web_btn);
    lv_obj_set_size(ui->web, obj_width, obj_height);
    lv_obj_align(ui->web, LV_ALIGN_CENTER, 0, 0);
    lv_img_set_src(ui->web, &_web_110x110);

    /*Update the buttons position manually for first*/
    lv_event_send(ui->panel, LV_EVENT_SCROLL, NULL);
    /*Be sure the fist button is in the middle*/
    lv_obj_scroll_to_view(lv_obj_get_child(ui->panel, 0), LV_ANIM_OFF);

    add_home_group_obj(PAGE_WEATHER);

    page_screen_anim(ui->home, -240, 0, HOME_PAGE_OUT_TIME, delay, (lv_anim_exec_xcb_t)lv_obj_set_y, lv_anim_path_linear);
}

/**
 * @description: 
 * @param {lv_ui} *ui
 * @return {*}
 */
void setup_ui(lv_ui *ui)
{
#define UI_WRITE

    lv_obj_set_scrollbar_mode(lv_scr_act(), LV_SCROLLBAR_MODE_OFF);
#ifdef UI_WRITE
    setup_boot_screen(ui);
    setup_home_screen(ui, BOOT_PAGE_OUT_TIME);
#else
    setup_home_screen(ui, 0);

#endif
}
