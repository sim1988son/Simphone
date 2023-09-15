#ifndef _UI_SET_START_H
#define _UI_SET_START_H

#if __has_include("lvgl.h")
#include "lvgl.h"
#else
#include "lvgl/lvgl.h"
#endif

// #define STARTSETUP_JSON_CONFIG_FILE    "/start_setup.json"

// typedef struct {
//     int start_trans = 255;
//     int start_bg = 1; 
// } start_config_t;

void openAppStart();
void AppStart();
// void start_save_config(void);
// void start_load_config(void);
void set_obj_color_bg(void);
void set_opa(int value);
lv_obj_t * create_obj(lv_obj_t *parent, int col, int row);

#endif