#ifndef _UI_SETUP_H
#define _UI_SETUP_H

#if __has_include("lvgl.h")
#include "lvgl.h"
#else
#include "lvgl/lvgl.h"
#endif




void openSettings(void);

lv_obj_t *create_header(lv_obj_t *parent, const char *text, int yPos=0);
lv_obj_t *create_btn_header(lv_obj_t *parent, const char *text, uint16_t xPos, uint16_t yPos, uint16_t width, uint16_t height, lv_event_cb_t callback, lv_obj_t * data);
void add_item(lv_obj_t *parent, char *name, char *src, lv_event_cb_t callback);
lv_obj_t * ui_app_settingsScreen();
void event_setup_back(lv_event_t *e);

#endif