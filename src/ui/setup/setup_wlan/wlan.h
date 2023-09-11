#ifndef _UI_SET_WLAN_H
#define _UI_SET_WLAN_H

#if __has_include("lvgl.h")
#include "lvgl.h"
#else
#include "lvgl/lvgl.h"
#endif


void openAppwifi();

void add_wifi_name_list_btn(lv_obj_t *parent, const char *name, const char *src);

void lv_wifi_on(void);

void lv_wifi_off(void);

void lv_wifi_auto_on(void);

void lv_wifi_auto_off(void);

void lv_wifi_stanby_on(void);

void lv_wifi_stanby_off(void);

#endif

