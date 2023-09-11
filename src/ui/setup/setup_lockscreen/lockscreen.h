#ifndef _UI_SET_LOCKSCREEN_H
#define _UI_SET_LOCKSCREEN_H

#if __has_include("lvgl.h")
#include "lvgl.h"
#else
#include "lvgl/lvgl.h"
#endif

// #define LOCKSETUP_JSON_CONFIG_FILE    "/lock_setup.json"

// typedef struct {

// } lock_config_t;

void openAppLockScreen();
// void lock_save_config(void);
// void lock_load_config(void);
// void lock_obj_color_bg(void);


#endif