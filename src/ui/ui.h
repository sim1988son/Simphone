#ifndef _UI_H
#define _UI_H

#if __has_include("lvgl.h")
#include "lvgl.h"
#else
#include "lvgl/lvgl.h"
#endif


static char ssidwifi[64];

#define STATUSBAR_NUM  8
#define STARTAPP_NUM  17

typedef struct {
        lv_obj_t *icon;
        const char *symbol;
    } lv_status_bar_t;

typedef enum {
        ui_chargeIcon = 0,
        ui_alarmIcon ,
        ui_wifiIcon,
        ui_bleIcon,
        ui_volumeIcon,
        ui_waringIcon,
        ui_sdcardIcon,
        ui_flashIcon

    } statusbar_icon_t;

void ui_init(void);
void ui_startScreen_init(void);
void openStart(void);
// void ui_bootScreen_init(void);
void splash_screen_stage_boot(void);

    void splash_screen_stage_update( const char* msg, int value );
    void splash_screen_stage_finish( void );

void statusBar_show(void);
void statusBar_hide_icon(int i);
void statusBar_show_icon(int i);
void statusbar_style_icon( int i, lv_color_t value) ;
void statusbar_refresh(void);
lv_obj_t * ui_app_startScreen();
void ui_statusBar_update_timer( lv_timer_t  * timer );


lv_obj_t *create_button(lv_obj_t *parent, lv_event_cb_t callback);
lv_obj_t *create_label(lv_obj_t *parent, uint16_t Pos, const char *text, uint16_t xPos, uint16_t yPos,  uint8_t font);
lv_obj_t *create_switch(lv_obj_t *parent, uint16_t yPos, lv_event_cb_t callback);
// lv_obj_t *create_label_switch(lv_obj_t *parent, lv_event_cb_t callback, const char *text, uint16_t yPos, uint8_t font);
lv_obj_t *create_slider(lv_obj_t *parent, uint16_t xPos, uint16_t yPos, uint16_t width, uint16_t height, lv_event_cb_t callback);
// lv_obj_t *create_slider2(lv_obj_t *parent, const char *text, uint16_t xPos, uint16_t yPos, uint16_t width, uint16_t height, lv_event_cb_t callback);
lv_obj_t *create_mbox(lv_obj_t *parent, const char *text, uint16_t xPos, uint16_t yPos, uint16_t width, uint16_t height);
lv_obj_t *create_mboxbtn(lv_obj_t *parent, lv_event_cb_t callback, bool bbtn);
lv_obj_t *create_button_obj(lv_obj_t *parent, lv_event_cb_t callback, uint16_t yPos);
lv_obj_t *create_roller(lv_obj_t *parent, uint16_t roll, uint16_t xPos, uint16_t yPos);
lv_obj_t *create_obj(lv_obj_t *parent, uint16_t xPos, uint16_t yPos, uint16_t width, uint16_t height, lv_color_t color);
lv_obj_t *create_obj_color(lv_obj_t *parent, uint16_t xPos, uint16_t yPos, uint16_t w, uint32_t colorp, lv_event_cb_t callback);

// lv_obj_t * ui_app_notificationPanel();

#endif