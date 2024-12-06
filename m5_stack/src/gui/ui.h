// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.5.0
// LVGL version: 8.3.11
// Project name: m5

#ifndef _M5_UI_H
#define _M5_UI_H

#ifdef __cplusplus
extern "C" {
#endif

#if defined __has_include
#if __has_include("lvgl.h")
#include "lvgl.h"
#elif __has_include("lvgl/lvgl.h")
#include "lvgl/lvgl.h"
#else
#include "lvgl.h"
#endif
#else
#include "lvgl.h"
#endif

#include "ui_helpers.h"
#include "ui_events.h"

// SCREEN: ui_Screen1
void ui_Screen1_screen_init(void);
void ui_event_Screen1(lv_event_t * e);
extern lv_obj_t * ui_Screen1;
extern lv_obj_t * ui_Image1;
extern lv_obj_t * ui_Image2;
extern lv_obj_t * ui_Image3;
extern lv_obj_t * ui_Image4;
extern lv_obj_t * ui_Image5;
extern lv_obj_t * ui_Image6;
extern lv_obj_t * ui_Label1;
extern lv_obj_t * ui_press;
extern lv_obj_t * ui_Label3;
extern lv_obj_t * ui_humi;
extern lv_obj_t * ui_Label5;
extern lv_obj_t * ui_Label6;
extern lv_obj_t * ui_Spinbox1;
extern lv_obj_t * ui_Spinbox2;
extern lv_obj_t * ui_time;
extern lv_obj_t * ui_temp;
extern lv_obj_t * ui_Label9;
// CUSTOM VARIABLES
extern lv_obj_t * uic_Screen1;

// SCREEN: ui_Screen2
void ui_Screen2_screen_init(void);
void ui_event_Screen2(lv_event_t * e);
extern lv_obj_t * ui_Screen2;
extern lv_obj_t * ui_Checkbox1;
extern lv_obj_t * ui_Button2;
// CUSTOM VARIABLES

// EVENTS

extern lv_obj_t * ui____initial_actions0;

// IMAGES AND IMAGE SETS
LV_IMG_DECLARE(ui_img_group1_png);    // assets/Group1.png
LV_IMG_DECLARE(ui_img_weathe_png);    // assets/weathe.png
LV_IMG_DECLARE(ui_img_999427845);    // assets/download (1).png
LV_IMG_DECLARE(ui_img_humi_png);    // assets/humi.png
LV_IMG_DECLARE(ui_img_wifi1_png);    // assets/wifi1.png
LV_IMG_DECLARE(ui_img_cloud1_png);    // assets/cloud1.png

// UI INIT
void ui_init(void);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif
