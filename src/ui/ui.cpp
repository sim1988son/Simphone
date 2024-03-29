#include <Arduino.h>
#include "config.h"
#include "hardware/powerset.h"
#include "hardware/displayset.h"
#include "hardware/sdset.h"
#include "hardware/wifiset.h"
#include "ui/ui.h"
#include "ui/setup/setup.h"

#include <lgfx.h>
LGFX tftt;

lv_obj_t * ui_startScreen;
lv_obj_t * cont;
lv_obj_t * ui_tileView;
lv_obj_t * ui_start_app;

lv_obj_t * ui_bootScreen;
lv_obj_t * preload; 
lv_obj_t * preload_label;

lv_obj_t * ui_Panel2;
lv_obj_t * ui_backButton; 
lv_obj_t * ui_startButton;

lv_obj_t * ui_notificationPanel;
lv_obj_t * ui_actionPanel;
lv_obj_t * ui_actionDate;
lv_obj_t * ui_actionBattery;
lv_obj_t * ui_notificationText;
lv_obj_t * ui_dragPanel;
lv_obj_t * ui_Label7;

typedef struct Drag{
    bool dragging;
    bool active;
    int y;
} drag;

//drag lockscreen;
drag notification;

lv_obj_t * ui_btnwifi;
lv_obj_t * ui_btnble;
lv_obj_t * ui_btnvol;
lv_obj_t * ui_btnflash;

lv_obj_t * ui_statusBar;
lv_obj_t * ui_batteryBar;
lv_obj_t * ui_lockTime;
lv_obj_t * ui_batterysymbol;
lv_obj_t * ui_batterysymbolcharge;
lv_timer_t * ui_statusBar_timer;

#define LIGHT_COLOR_TEXT       lv_palette_darken(LV_PALETTE_GREY, 4)
#define DARK_COLOR_TEXT        lv_palette_lighten(LV_PALETTE_GREY, 5)

static bool statusbar_refresh_update = false;
static bool statusbar_refresh_time  = false;

lv_status_bar_t statusIcon[ STATUSBAR_NUM ] = {
    {NULL, LV_SYMBOL_CHARGE},
    {NULL, LV_SYMBOL_BELL},
    {NULL, LV_SYMBOL_WIFI},
    {NULL, LV_SYMBOL_BLUETOOTH},
    {NULL, LV_SYMBOL_VOLUME_MAX},
    {NULL, LV_SYMBOL_WARNING},
    {NULL, LV_SYMBOL_SD_CARD},
    {NULL, LV_SYMBOL_EDIT}
};

bool gui_power_event_cb( EventBits_t event, void *arg );
bool gui_power_loop_event_cb( EventBits_t event, void *arg );

void navigation(lv_obj_t *parent);
void notification_panel(lv_obj_t *parent);
void statusBar_hide_icon(int i);
void statusBar_show_icon(int i);

//===== event =====
static void event_launch(lv_event_t *e){
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj = lv_event_get_target(e);
    if (code == LV_EVENT_CLICKED){
        char buf[150];
        char *data = (char *)lv_event_get_user_data(e);
        // Settings list
        if (data == "Settings"){
            openSettings();  
        }
    }
}

static void event_action_wifi(lv_event_t *e){
    lv_event_code_t event = lv_event_get_code(e);
    lv_obj_t *obj = lv_event_get_target(e);
    // log_i("NOTIFI SET ");
    if (event == LV_EVENT_VALUE_CHANGED){
        if (lv_obj_has_state(obj, LV_STATE_CHECKED)){
            // log_i("NOTIFI WIFION OFF");
            wifictl_off();
            // wifictl_set_autoon( false );
        } else {
            // log_i("NOTIFI WIFION ON");
            wifictl_on();
            // wifictl_set_autoon( true );
        }
    }
}

static void event_action_ble(lv_event_t *e){
    lv_event_code_t event = lv_event_get_code(e);
    lv_obj_t *obj = lv_event_get_target(e);
    // log_i("NOTIFI SET ");
    if (event == LV_EVENT_VALUE_CHANGED){
        if (lv_obj_has_state(obj, LV_STATE_CHECKED)){
            // log_i("NOTIFI BLE OFF");
            // wifictl_on();
            // wifictl_set_autoon( true );
        } else {
            // log_i("NOTIFI BLE ON");
            // wifictl_off();
            // wifictl_set_autoon( false );
        }
    }
}

static void event_action_vol(lv_event_t *e){
    lv_event_code_t event = lv_event_get_code(e);
    lv_obj_t *obj = lv_event_get_target(e);
    // log_i("NOTIFI SET ");
    if (event == LV_EVENT_VALUE_CHANGED){
        if (lv_obj_has_state(obj, LV_STATE_CHECKED)){
            // log_i("NOTIFI SOUND OFF");
            // wifictl_on();
            // wifictl_set_autoon( true );
        } else {
            // log_i("NOTIFI SOUND ON");
            // wifictl_off();
            // wifictl_set_autoon( false );
        }
    }
}

static void event_action_flash(lv_event_t *e){
    lv_event_code_t event = lv_event_get_code(e);
    lv_obj_t *obj = lv_event_get_target(e);
    // log_i("NOTIFI SET ");
    if (event == LV_EVENT_VALUE_CHANGED){
        if (lv_obj_has_state(obj, LV_STATE_CHECKED)){
            // log_i("NOTIFI FLASH OF");
            // wifictl_on();
            // wifictl_set_autoon( true );
        } else {
            // log_i("NOTIFI FLASH ON");
            // wifictl_off();
            // wifictl_set_autoon( false );
        }
    }
}


static void event_navigate(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj = lv_event_get_target(e);
    lv_disp_t *display = lv_disp_get_default();
    lv_obj_t *actScr = lv_disp_get_scr_act(display);

    if (code == LV_EVENT_CLICKED){
        // lv_obj_add_flag(ui_systemKeyboard, LV_OBJ_FLAG_HIDDEN);

        if (obj == ui_backButton){
            // vibrate(100);
            if (actScr == ui_app_settingsScreenApps()){
                openSettings();
                log_i("BUTTON BACK 3");
            }else if (actScr == ui_app_settingsScreen()){
                openStart();
                log_i("BUTTON BACK 2");
            }else if (actScr == ui_startScreen){
                // openSettings();
                log_i("BUTTON BACK 0");
            }else if (actScr != ui_startScreen){
                openStart();
                log_i("BUTTON BACK 1");
            }else{
                // lv_obj_set_tile_id(ui_tileView, 0, 0, LV_ANIM_ON);
            }
        }
        if (obj == ui_startButton){
            // vibrate(100);
            if (actScr != ui_startScreen){
                openStart();
                log_i("BUTTON START 1");
            }else{
                // lv_obj_set_tile_id(ui_tileView, 0, 0, LV_ANIM_ON);
                log_i("BUTTON START 2");
            }
        }
    }
}
    
static void ui_event_notificationPanel(lv_event_t *e){
    lv_event_code_t event = lv_event_get_code(e);
    lv_obj_t *ta = lv_event_get_target(e);
    lv_indev_t *indev = lv_indev_get_act();
    if (event == LV_EVENT_PRESSING){
        if (notification.dragging){
            int vect = indev->proc.types.pointer.act_point.y - notification.y;
            if (notification.active){
                // closing notification panel
                if (vect <= 0 && vect >= -480){
                    lv_obj_set_y(ui_notificationPanel, vect);
                    if (vect >= -330){
                        lv_obj_set_y(ui_actionPanel, 50 + (vect * -1));
                    }
                }
            }else{
                // opening notification panel
                if (vect >= 0 && vect <= 430){
                    lv_obj_set_y(ui_notificationPanel, -430 + vect);
                    if (vect >= 150){
                        lv_obj_set_y(ui_actionPanel, 330 - (vect - 150));
                    }
                }
            }
        }
    }
    if (event == LV_EVENT_PRESSED){
        notification.y = indev->proc.types.pointer.act_point.y;
        if (!notification.active){
            // 0 - 30
            if (notification.y < 50){
                lv_obj_move_foreground(ui_notificationPanel);
                lv_obj_move_foreground(ui_statusBar);
                // dragging = true;
                lv_obj_set_style_opa(ui_notificationPanel, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                lv_obj_set_y(ui_actionPanel, 330);

                lv_obj_set_style_text_opa(ui_actionBattery, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                lv_obj_set_style_text_opa(ui_actionDate, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                lv_obj_set_style_text_opa(ui_notificationText, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            }
        }else{
            lv_obj_set_style_text_opa(ui_actionBattery, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_opa(ui_actionDate, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_opa(ui_notificationText, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        notification.dragging = true;
        // lv_obj_set_y(ui_notificationPanel, -440);
    }
    if (event == LV_EVENT_RELEASED){
        // printf("Release\tx:%d, y:%d\n", indev->proc.types.pointer.act_point.x, indev->proc.types.pointer.act_point.y);
        notification.dragging = false;
        int vect = indev->proc.types.pointer.act_point.y - notification.y;
        if (notification.active){
            if ((vect * -1) < 150){
                lv_obj_set_style_opa(ui_notificationPanel, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                lv_obj_set_y(ui_notificationPanel, 0);
                lv_obj_set_y(ui_actionPanel, 50);
                lv_obj_set_style_text_opa(ui_actionBattery, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                lv_obj_set_style_text_opa(ui_actionDate, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                lv_obj_set_style_text_opa(ui_notificationText, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                notification.active = true;
            }else{
                lv_obj_set_y(ui_notificationPanel, -430);
                lv_obj_set_style_opa(ui_notificationPanel, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                lv_obj_set_y(ui_actionPanel, 330);
                lv_obj_set_style_text_opa(ui_actionBattery, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                lv_obj_set_style_text_opa(ui_actionDate, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                lv_obj_set_style_text_opa(ui_notificationText, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                notification.active = false;
            }
        }else{
            if (vect > 150){
                lv_obj_set_style_opa(ui_notificationPanel, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                lv_obj_set_y(ui_notificationPanel, 0);
                lv_obj_set_y(ui_actionPanel, 50);
                lv_obj_set_style_text_opa(ui_actionBattery, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                lv_obj_set_style_text_opa(ui_actionDate, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                lv_obj_set_style_text_opa(ui_notificationText, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                notification.active = true;
            }else{
                lv_obj_set_y(ui_notificationPanel, -430);
                lv_obj_set_style_opa(ui_notificationPanel, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                lv_obj_set_y(ui_actionPanel, 330);
                lv_obj_set_style_text_opa(ui_actionBattery, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                lv_obj_set_style_text_opa(ui_actionDate, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                lv_obj_set_style_text_opa(ui_notificationText, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                notification.active = false;
            }
        }
    }
}

//==== create =======
lv_obj_t * create_tile(lv_obj_t *parent, char *name, const void *src, int col, int row, int size, int high, lv_event_cb_t callback){
    lv_obj_t *label;
    lv_obj_t *obj;
    lv_obj_t *obj2;
    lv_obj_t *icon;
    obj = lv_obj_create(parent);
    lv_obj_set_style_radius(obj, 10, 0);
    lv_obj_set_grid_cell(obj, LV_GRID_ALIGN_STRETCH, col, size, LV_GRID_ALIGN_STRETCH, row, high);
    // lv_obj_add_event_cb(obj, callback, LV_EVENT_ALL, name);
    lv_obj_set_style_bg_opa(obj, LV_OPA_0, LV_PART_MAIN | LV_STATE_DEFAULT); 
    lv_obj_set_style_border_opa(obj, LV_OPA_0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_all(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);

    obj2 = lv_obj_create(obj);
    lv_obj_set_width(obj2, 80);
    lv_obj_set_height(obj2, 80);
    lv_obj_set_align(obj2, LV_ALIGN_TOP_MID);
    lv_obj_add_event_cb(obj2, callback, LV_EVENT_ALL, name);
    lv_obj_set_style_radius(obj2, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(obj2, LV_OPA_20, LV_PART_MAIN | LV_STATE_DEFAULT); 
    // lv_obj_set_style_border_opa(obj, LV_OPA_0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_all(obj2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_clear_flag(obj2, LV_OBJ_FLAG_SCROLLABLE);

    icon = lv_img_create(obj2);
    lv_img_set_src(icon, src);
    lv_obj_set_width(icon, LV_SIZE_CONTENT);
    lv_obj_set_height(icon, LV_SIZE_CONTENT);
    lv_obj_set_align(icon, LV_ALIGN_CENTER);
    lv_obj_add_flag(icon, LV_OBJ_FLAG_ADV_HITTEST);
    lv_obj_clear_flag(icon, LV_OBJ_FLAG_SCROLLABLE);

    label = lv_label_create(obj);
    lv_label_set_text(label, name);
    lv_obj_set_align(label, LV_ALIGN_BOTTOM_MID);
    lv_obj_set_x(label, 0);
    lv_obj_set_y(label, 0);
    lv_obj_set_style_text_font(label, &lv_font_montserrat_16, 0);

    return obj;
}

///////////////////// DYNAMIC COMPONENTS ////////////////////

lv_obj_t *create_label(lv_obj_t *parent, uint16_t Pos, const char *text, uint16_t xPos, uint16_t yPos, uint8_t font)
{
    lv_obj_t *label = lv_label_create(parent);
    lv_obj_set_width(label, LV_SIZE_CONTENT);              /// 1
    // lv_obj_set_height(label, LV_SIZE_CONTENT); /// 1
    if(Pos == 0){lv_obj_set_align(label, LV_ALIGN_TOP_LEFT);}
    if(Pos == 1){lv_obj_set_align(label, LV_ALIGN_BOTTOM_LEFT);}
    if(Pos == 2){lv_obj_set_align(label, LV_ALIGN_BOTTOM_RIGHT);}
    if(Pos == 3){lv_obj_set_align(label, LV_ALIGN_TOP_RIGHT);}
    lv_obj_set_x(label, xPos);
    lv_obj_set_y(label, yPos);
    lv_label_set_text(label, text);
    if(font == 14)lv_obj_set_style_text_font(label, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    if(font == 16)lv_obj_set_style_text_font(label, &lv_font_montserrat_16, LV_PART_MAIN | LV_STATE_DEFAULT);
    if(font == 20)lv_obj_set_style_text_font(label, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
    return label;
}

lv_obj_t *create_slider(lv_obj_t *parent, uint16_t xPos, uint16_t yPos, uint16_t width, uint16_t height, lv_event_cb_t callback)
{

    lv_obj_t *slider = lv_slider_create(parent);
    lv_slider_set_range(slider, 0, 255);
    lv_obj_set_width(slider, width);
    lv_obj_set_height(slider, height);
    lv_obj_set_x(slider, xPos);
    lv_obj_set_y(slider, yPos);
    lv_obj_set_style_pad_all(slider, 7, LV_PART_KNOB);
    //lv_obj_set_style_x(slider, -14, LV_PART_KNOB);
    lv_obj_set_style_radius(slider, LV_RADIUS_CIRCLE, LV_PART_KNOB);
    // lv_obj_set_style_bg_color(slider,lv_color_white(), LV_PART_KNOB);
    lv_obj_add_event_cb(slider, callback, LV_EVENT_VALUE_CHANGED, NULL);
    return slider;
}


lv_obj_t *create_switch(lv_obj_t *parent, uint16_t yPos, lv_event_cb_t callback)
{

    lv_obj_t * Sw = lv_switch_create(parent);
    lv_obj_set_width(Sw, 55);  /// 1
    lv_obj_set_height(Sw, 30); /// 1
    lv_obj_set_x(Sw, -20);
    lv_obj_set_y(Sw, yPos-5);
    lv_obj_set_align(Sw, LV_ALIGN_TOP_RIGHT);
    lv_obj_add_event_cb(Sw, callback, LV_EVENT_VALUE_CHANGED, NULL);

    return Sw;
}

lv_obj_t *create_mbox(lv_obj_t *parent, const char *text, uint16_t xPos, uint16_t yPos, uint16_t width, uint16_t height)
{
    lv_obj_t * mbox = lv_obj_create(parent);
    lv_obj_clear_flag(mbox, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_width(mbox, width);  /// 1
    lv_obj_set_height(mbox, height); /// 1
    lv_obj_set_x(mbox, xPos);
    lv_obj_set_y(mbox, yPos);
    lv_obj_set_align(mbox, LV_ALIGN_TOP_LEFT);
    lv_color_t d = lv_theme_get_color_secondary(NULL);
    lv_obj_set_style_border_color(mbox, d, LV_PART_MAIN | LV_STATE_DEFAULT);
    
    lv_obj_t * ui_mboxTitle = lv_label_create(mbox);
    lv_label_set_text(ui_mboxTitle, text);
    lv_obj_align(ui_mboxTitle, LV_ALIGN_TOP_MID, 0, 0);
    lv_obj_set_style_text_font(ui_mboxTitle, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
    return mbox;
}

lv_obj_t *create_mboxbtn(lv_obj_t *parent, lv_event_cb_t callback, bool bbtn)
{
    lv_obj_t * btnLabel;

    lv_obj_t * Btn = lv_btn_create(parent);
    lv_obj_set_width(Btn, 100);  /// 1
    lv_obj_add_event_cb(Btn, callback, LV_EVENT_ALL, NULL);
    if(bbtn){
        lv_obj_align(Btn, LV_ALIGN_BOTTOM_LEFT, 0, 0);
        btnLabel = lv_label_create(Btn);
        lv_label_set_text(btnLabel, "OK");
        lv_obj_center(btnLabel);
    }else{
        lv_obj_align(Btn, LV_ALIGN_BOTTOM_RIGHT, 0, 0);
        btnLabel = lv_label_create(Btn);
        lv_label_set_text(btnLabel, "Cancel");
        lv_obj_center(btnLabel);
    }
    return Btn;
}


lv_obj_t *create_button(lv_obj_t *parent, lv_event_cb_t callback)
{
    lv_obj_t *button = lv_obj_create(parent);
    lv_obj_set_width(button, 70);  /// 1
    lv_obj_set_height(button, 40); /// 1
    lv_obj_set_x(button, 0);
    lv_obj_set_y(button, 30);
    lv_obj_set_align(button, LV_ALIGN_TOP_LEFT);
    // lv_obj_set_scrollbar_mode(button, LV_SCROLLBAR_MODE_ACTIVE);
    lv_obj_set_style_radius(button, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    // lv_obj_set_style_bg_color(panel, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(button, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(button, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(button, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(button, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(button, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(button, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    // lv_obj_add_flag(button, LV_OBJ_FLAG_CHECKABLE);
    lv_obj_add_event_cb(button, callback, LV_EVENT_ALL, NULL);

    lv_obj_t *label = lv_label_create(button);
    lv_obj_set_x(label, 20);
    lv_obj_set_y(label, 0);
    lv_obj_set_align(label, LV_ALIGN_LEFT_MID);
    lv_label_set_text(label, LV_SYMBOL_LEFT);
    lv_obj_set_style_text_font(label, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
    return button;
}

lv_obj_t *create_obj(lv_obj_t *parent, uint16_t xPos, uint16_t yPos, uint16_t width, uint16_t height, lv_color_t color)
{
    lv_obj_t * obj = lv_obj_create(parent);
    lv_obj_set_width(obj, width);  /// 1
    lv_obj_set_height(obj, height); /// 1
    lv_obj_set_x(obj, xPos);
    lv_obj_set_y(obj, yPos);
    lv_obj_set_align(obj, LV_ALIGN_TOP_MID);
    lv_obj_set_style_radius(obj, 7, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(obj, color, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(obj, lv_palette_main(LV_PALETTE_GREY), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    return obj;
}

lv_obj_t *create_obj_color(lv_obj_t *parent, uint16_t xPos, uint16_t yPos, uint16_t w, uint32_t colorp, lv_event_cb_t callback)
{
    lv_obj_t * obj = lv_obj_create(parent);
    lv_obj_set_width(obj, w);  /// 1
    lv_obj_set_height(obj, w); /// 1
    lv_obj_set_x(obj, xPos);
    lv_obj_set_y(obj, yPos);
    lv_obj_set_align(obj, LV_ALIGN_TOP_LEFT);
    lv_obj_set_style_radius(obj, w/2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(obj, lv_color_hex(colorp), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_event_cb(obj, callback, LV_EVENT_ALL , NULL);

    // obj = lv_obj_create(obj);
    // lv_obj_set_width(obj, w/2);  /// 1
    // lv_obj_set_height(obj, w/2); /// 1
    // lv_obj_set_align(obj, LV_ALIGN_CENTER);
    // lv_obj_set_style_radius(obj, w/4, LV_PART_MAIN | LV_STATE_DEFAULT);
    // lv_obj_set_style_bg_color(obj, colors, LV_PART_MAIN | LV_STATE_DEFAULT);
    // lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    // lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    // lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    // lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    // lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    // lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    return obj;
}

lv_obj_t *create_roller(lv_obj_t *parent, uint16_t roll, uint16_t xPos, uint16_t yPos)
{
    const char * opts;
    lv_obj_t * roller = lv_roller_create(parent);
    lv_obj_set_width(roller, 70);  /// 1
    lv_obj_set_x(roller, xPos);
    lv_obj_set_y(roller, yPos);
    lv_obj_set_align(roller, LV_ALIGN_CENTER);
    lv_obj_set_style_radius(roller, 0, LV_PART_MAIN | LV_PART_SELECTED);
    lv_obj_set_style_bg_opa(roller, 0, LV_PART_MAIN);
    // lv_obj_set_style_border_color(roller, lv_color_hex(0x323232), LV_PART_MAIN | LV_PART_SELECTED);
    // lv_obj_set_style_outline_color(roller, lv_color_hex(0x323232), LV_PART_MAIN | LV_PART_SELECTED);
    // lv_obj_set_style_shadow_color(roller, lv_color_hex(0x323232), LV_PART_MAIN | LV_PART_SELECTED);
    // lv_obj_set_style_shadow_width(roller, 10, LV_PART_MAIN | LV_PART_SELECTED);
    // lv_obj_set_style_text_color(roller, lv_palette_main(LV_PALETTE_BLUE), LV_PART_SELECTED);
    // lv_obj_set_style_bg_opa(roller, 255, LV_PART_MAIN | LV_PART_SELECTED);
    // lv_obj_set_style_border_width(roller, 0, LV_PART_MAIN | LV_PART_SELECTED);
    lv_obj_set_style_pad_left(roller, 0, LV_PART_MAIN | LV_PART_SELECTED);
    lv_obj_set_style_pad_right(roller, 0, LV_PART_MAIN | LV_PART_SELECTED);
    lv_obj_set_style_pad_top(roller, 0, LV_PART_MAIN | LV_PART_SELECTED);
    lv_obj_set_style_pad_bottom(roller, 0, LV_PART_MAIN | LV_PART_SELECTED);
    lv_obj_set_style_text_font(roller, &lv_font_montserrat_24, LV_PART_MAIN | LV_PART_SELECTED);
    if(roll == 60)opts = "00\n01\n02\n03\n04\n05\n06\n07\n08\n09\n10\n11\n12\n13\n14\n15\n16\n17\n18\n19\n20\n21\n22\n23\n24\n25\n26\n27\n28\n29\n30\n31\n32\n33\n34\n35\n36\n37\n38\n39\n40\n41\n42\n43\n44\n45\n46\n47\n48\n49\n50\n51\n52\n53\n54\n55\n56\n57\n58\n59";
    if(roll == 24)opts = "0\n1\n2\n3\n4\n5\n6\n7\n8\n9\n10\n11\n12\n13\n14\n15\n16\n17\n18\n19\n20\n21\n22\n23\n24"; //\n16\n17\n18\n19\n20\n21\n22\n23\n24\n25\n16\n17\n18\n19\n20\n21\n22\n23\n24\n25\n16\n17\n18\n19\n20\n21\n22\n23\n24\n25\n16\n17\n18\n19\n20\n21\n22\n23\n24\n25
    if(roll == 28)opts = "1\n2\n3\n4\n5\n6\n7\n8\n9\n10\n11\n12\n13\n14\n15\n16\n17\n18\n19\n20\n21\n22\n23\n24\n25\n26\n27\n28";
    if(roll == 29)opts = "1\n2\n3\n4\n5\n6\n7\n8\n9\n10\n11\n12\n13\n14\n15\n16\n17\n18\n19\n20\n21\n22\n23\n24\n25\n26\n27\n28\n29";
    if(roll == 30)opts = "1\n2\n3\n4\n5\n6\n7\n8\n9\n10\n11\n12\n13\n14\n15\n16\n17\n18\n19\n20\n21\n22\n23\n24\n25\n26\n27\n28\n29\n30";
    if(roll == 31)opts = "1\n2\n3\n4\n5\n6\n7\n8\n9\n10\n11\n12\n13\n14\n15\n16\n17\n18\n19\n20\n21\n22\n23\n24\n25\n26\n27\n28\n29\n30\n31";
    if(roll == 12)opts = "1\n2\n3\n4\n5\n6\n7\n8\n9\n10\n11\n12";
    if(roll == 44)opts = "01\n02\n03\n04\n05\n06\n07\n08\n09\n10\n11\n12\n13\n14\n15\n16\n17\n18\n19\n20\n21\n22\n23\n24\n25\n26\n27\n28\n29\n30\n31";
    if(roll == 2)opts = "AM\nPM";
    lv_roller_set_options(roller ,opts ,LV_ROLLER_MODE_INFINITE);
    lv_roller_set_visible_row_count(roller, 3);
    return roller;
}

lv_obj_t *create_button_obj(lv_obj_t *parent, lv_event_cb_t callback, uint16_t yPos)
{
    lv_obj_t *button = lv_obj_create(parent);
    lv_obj_set_width(button, 280);  /// 1
    lv_obj_set_height(button, 40); /// 1
    lv_obj_set_x(button, 20);
    lv_obj_set_y(button, yPos);
    lv_obj_clear_flag(button, LV_OBJ_FLAG_SCROLLABLE); /// Flags
    lv_obj_set_align(button, LV_ALIGN_TOP_LEFT);
    lv_obj_set_style_radius(button, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    //lv_obj_set_style_bg_color(button, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(button, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(button, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(button, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(button, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(button, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(button, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_event_cb(button, callback, LV_EVENT_ALL, NULL);
    //lv_obj_add_flag(button, LV_OBJ_FLAG_CHECKABLE);

    lv_obj_t *label = lv_label_create(button);
    //lv_obj_set_style_text_color(button, lv_color_hex(0x323232), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_x(label, 0);
    lv_obj_set_y(label, 0);
    lv_obj_set_align(label, LV_ALIGN_RIGHT_MID);
    lv_label_set_text(label, LV_SYMBOL_RIGHT);
    lv_obj_set_style_text_font(label, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);

    return button;
}

//=====
bool statusbar_sdset_event_cb( EventBits_t event, void *arg );
bool statusbar_wifictl_event_cb( EventBits_t event, void *arg );


void status_bar(lv_obj_t *parent){
    // ui_statusBar

    ui_statusBar = lv_obj_create(parent);
    lv_obj_set_width(ui_statusBar, 320);
    lv_obj_set_height(ui_statusBar, 30);
    lv_obj_set_x(ui_statusBar, 0);
    lv_obj_set_y(ui_statusBar, 0);
    lv_obj_set_align(ui_statusBar, LV_ALIGN_TOP_MID);
    lv_obj_clear_flag(ui_statusBar, LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_style_radius(ui_statusBar, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_statusBar, lv_palette_lighten(LV_PALETTE_GREY, 1), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_statusBar, 25, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_statusBar, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui_statusBar, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui_statusBar, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui_statusBar, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui_statusBar, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // ui_batteryBar

    ui_batterysymbol = lv_bar_create(ui_statusBar);
    lv_obj_set_size(ui_batterysymbol, 30, 16);
    lv_bar_set_value(ui_batterysymbol, 70, LV_ANIM_OFF);
    lv_obj_align(ui_batterysymbol, LV_ALIGN_TOP_RIGHT, -10, 5);
    lv_obj_set_style_bg_opa(ui_batterysymbol, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_batterysymbol, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_all(ui_batterysymbol, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_batterysymbol, display_get_display_darkon() ? DARK_COLOR_TEXT : LIGHT_COLOR_TEXT, LV_PART_INDICATOR);//lv_palette_main(LV_PALETTE_GREEN), LV_PART_INDICATOR);
    lv_obj_set_style_bg_color(ui_batterysymbol, lv_palette_main(LV_PALETTE_GREY), LV_PART_MAIN | LV_STATE_DEFAULT);//lv_palette_main(LV_PALETTE_GREEN), LV_PART_INDICATOR);
    lv_obj_set_style_radius(ui_batterysymbol, 0, LV_PART_INDICATOR);
    lv_obj_set_style_radius(ui_batterysymbol, 4, LV_PART_MAIN | LV_STATE_DEFAULT);

    for( int i = 0 ; i < STATUSBAR_NUM ; i++ ) {
        statusIcon[i].icon = lv_label_create( ui_statusBar);
        lv_img_set_src( statusIcon[i].icon, statusIcon[i].symbol );
        if (i == 0){
            lv_obj_align_to(statusIcon[i].icon, ui_batterysymbol, LV_ALIGN_OUT_LEFT_MID, -7, 0);
        }else{
            lv_obj_align_to(statusIcon[i].icon, statusIcon[i-1].icon, LV_ALIGN_OUT_LEFT_MID, -7, 0);
        }
    }

    for( int i = 0 ; i < STATUSBAR_NUM ; i++ ) {
        lv_obj_add_flag(statusIcon[i].icon, LV_OBJ_FLAG_HIDDEN); // hide the header
    }
    
    // ui_lockTime

    ui_lockTime = lv_label_create(ui_statusBar);
    lv_obj_set_width(ui_lockTime, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_lockTime, LV_SIZE_CONTENT);
    lv_obj_set_x(ui_lockTime, 10);
    lv_obj_set_y(ui_lockTime, 7);
    lv_obj_set_align(ui_lockTime, LV_ALIGN_TOP_LEFT);
    lv_label_set_text(ui_lockTime, "22:33");

    sdset_register_cb( SDCARD_ON , statusbar_sdset_event_cb, "statusbar sdset" );
    wifictl_register_cb( WIFICTL_CONNECT | WIFICTL_DISCONNECT | WIFICTL_OFF | WIFICTL_ON | WIFICTL_SCAN | WIFICTL_WPS_SUCCESS | WIFICTL_WPS_FAILED | WIFICTL_CONNECT_IP, statusbar_wifictl_event_cb, "statusbar wifi" );
    
    ui_statusBar_timer = lv_timer_create(ui_statusBar_update_timer, 250,  NULL);

    statusBar_show_icon(ui_chargeIcon);

    // statusbar_refresh();
    bool sdsetstatus = sdset_get_status();
    sdset_send_event_cb( SDCARD_ON, (void *)&sdsetstatus );
}

void ui_statusBar_update_timer( lv_timer_t  * timer ) {
    if ( statusbar_refresh_update ) {
        statusbar_refresh();
        statusbar_refresh_update = false;
    }

    if ( statusbar_refresh_time ) {
        statusbar_refresh_time = false;
        lv_label_set_text(ui_lockTime, "22:33");
    } else {
        statusbar_refresh_time = true;
        lv_label_set_text(ui_lockTime, "22 33");
    }
    // lv_label_set_text(ui_lockTime, updatelockTime().c_str());
}


bool statusbar_sdset_event_cb( EventBits_t event, void *arg ) {
    switch( event ) {
        case SDCARD_ON:     if ( *(bool*)arg ) statusBar_show_icon( ui_sdcardIcon );
                            else statusBar_hide_icon( ui_sdcardIcon );
                            break;
    }
    statusbar_refresh();
    return( true );
}

bool statusbar_wifictl_event_cb( EventBits_t event, void *arg ) {
    switch( event ) {
        case WIFICTL_CONNECT:       statusbar_style_icon( ui_wifiIcon, display_get_display_darkon() ? DARK_COLOR_TEXT : LIGHT_COLOR_TEXT );
                                    // statusbar_wifi_set_state( true, (char *)arg );
                                    statusBar_show_icon( ui_wifiIcon );
                                    break;
        case WIFICTL_CONNECT_IP:    statusbar_style_icon( ui_wifiIcon, display_get_display_darkon() ? DARK_COLOR_TEXT : LIGHT_COLOR_TEXT );
                                    // statusbar_wifi_set_ip_state( true, (char *)arg );
                                    statusBar_show_icon( ui_wifiIcon );
                                    break;
        case WIFICTL_DISCONNECT:    statusbar_style_icon( ui_wifiIcon, lv_palette_main(LV_PALETTE_GREY) );
                                    // statusbar_wifi_set_state( false, (char *)arg );
                                    statusBar_show_icon( ui_wifiIcon );
                                    break;
        case WIFICTL_OFF:           statusbar_style_icon( ui_wifiIcon, lv_palette_main(LV_PALETTE_GREY) );
                                    statusBar_hide_icon( ui_wifiIcon );
                                    // statusbar_wifi_set_state( false, "" );
                                    break;
        case WIFICTL_ON:            statusbar_style_icon( ui_wifiIcon, lv_palette_main(LV_PALETTE_GREY) );
                                    // statusbar_wifi_set_state( true, (char *)arg );
                                    statusBar_show_icon( ui_wifiIcon );
                                    break;
        case WIFICTL_WPS_SUCCESS:   statusbar_style_icon( ui_wifiIcon, lv_palette_main(LV_PALETTE_GREY) );
                                    // statusbar_wifi_set_state( true, (char *)arg );
                                    statusBar_show_icon( ui_wifiIcon );
                                    break;
        case WIFICTL_WPS_FAILED:    statusbar_style_icon( ui_wifiIcon, lv_palette_main(LV_PALETTE_GREY) );
                                    // statusbar_wifi_set_state( true, (char *)arg );
                                    statusBar_show_icon( ui_wifiIcon );
                                    break;
        case WIFICTL_SCAN:          statusbar_style_icon( ui_wifiIcon, lv_palette_main(LV_PALETTE_GREY) );
                                    // statusbar_wifi_set_state( true, (char *)arg );
                                    statusBar_show_icon( ui_wifiIcon );
                                    break;
    }
    statusbar_refresh();
    return( true );
}

void statusbar_refresh(void){
    for ( int i = 0 ; i < STATUSBAR_NUM ; i++ ) {
        // if(statusIcon[ui_wifiIcon].icon && wifi_status()) lv_obj_set_style_text_color(statusIcon[ui_wifiIcon].icon, display_get_display_darkon() ? DARK_COLOR_TEXT : LIGHT_COLOR_TEXT, 0);
        if (i == 0){
            lv_obj_align_to(statusIcon[i].icon, ui_batterysymbol, LV_ALIGN_OUT_LEFT_MID, -7, 0);
        }else{
            if (lv_obj_has_flag(statusIcon[i-1].icon, LV_OBJ_FLAG_HIDDEN)){
                lv_obj_align_to(statusIcon[i].icon, statusIcon[i-1].icon, LV_ALIGN_RIGHT_MID, 0, 0);
            }else{
                lv_obj_align_to(statusIcon[i].icon, statusIcon[i-1].icon, LV_ALIGN_OUT_LEFT_MID, -7, 0);
            }
        }
    }
}

void statusbar_refresh_plus(void){
    statusbar_style_icon( ui_wifiIcon, display_get_display_darkon() ? DARK_COLOR_TEXT : LIGHT_COLOR_TEXT );
    lv_obj_set_style_bg_color(ui_batterysymbol, display_get_display_darkon() ? DARK_COLOR_TEXT : LIGHT_COLOR_TEXT, LV_PART_INDICATOR);
}

void statusBar_hide_icon(int i){
    if ( i >= STATUSBAR_NUM  ) return;
    lv_obj_add_flag(statusIcon[i].icon, LV_OBJ_FLAG_HIDDEN);
    statusbar_refresh_update = true;
}

void statusBar_show_icon(int i){
    if ( i >= STATUSBAR_NUM  ) return;
    lv_obj_clear_flag(statusIcon[i].icon, LV_OBJ_FLAG_HIDDEN);
    statusbar_refresh_update = true;
}

void statusbar_style_icon( int i, lv_color_t value) {
    if ( i >= STATUSBAR_NUM  ) return;
    lv_obj_set_style_text_color(statusIcon[i].icon, value, 0);
}

void statusBar_show(void){
    statusbar_refresh_update = true;
}

void ui_init(void){                                     
    lv_disp_t *display = lv_disp_get_default();
    lv_theme_t *theme = lv_theme_default_init(display, 
                        lv_color_hex(0x055fff), 
                        lv_palette_main(LV_PALETTE_GREY),
                        display_get_display_darkon(), 
                        LV_FONT_DEFAULT);
    lv_disp_set_theme(display, theme);

    ui_startScreen_init();
    ui_settingsScreen_init();
    ui_settingsScreenApps_init();
    lv_disp_load_scr(ui_startScreen);
}

lv_obj_t * ui_app_startScreen(){
    return (ui_startScreen);
}

void openSettings(void){
    lv_obj_clean(ui_app_settingsScreen());
    ui_settingsScreen_init();
    lv_obj_set_parent(ui_Panel2, ui_app_settingsScreen());
    lv_obj_set_parent(ui_notificationPanel, ui_app_settingsScreen());
    lv_obj_set_parent(ui_statusBar, ui_app_settingsScreen());
    lv_disp_load_scr(ui_app_settingsScreen());
}

void openlaunch(void){
    lv_obj_set_parent(ui_Panel2, ui_app_settingsScreenApps());
    lv_obj_set_parent(ui_notificationPanel, ui_app_settingsScreenApps());
    lv_obj_set_parent(ui_statusBar, ui_app_settingsScreenApps());
    // lv_disp_load_scr(ui_app_settingsScreenApps());
}

void openStart(){
    // log_i("LV load ui_startScreen");
    lv_obj_set_parent(ui_Panel2, ui_startScreen);
    lv_obj_set_parent(ui_notificationPanel, ui_startScreen);
    lv_obj_set_parent(ui_statusBar, ui_startScreen);
    lv_disp_load_scr(ui_startScreen);
}

void ui_startScreen_init(void){
    // ui_startScreen
    ui_startScreen = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_startScreen, LV_OBJ_FLAG_SCROLLABLE);

    // tile view
    ui_tileView = lv_tileview_create(ui_startScreen);
    lv_obj_set_width(ui_tileView, 320);
    lv_obj_set_height(ui_tileView, 400);
    lv_obj_set_x(ui_tileView, 0);
    lv_obj_set_y(ui_tileView, 30);
    lv_obj_set_align(ui_tileView, LV_ALIGN_TOP_MID);
    lv_obj_set_scrollbar_mode(ui_tileView, LV_SCROLLBAR_MODE_OFF);
    static lv_coord_t col_dsc[] = {80, 80, 80, LV_GRID_TEMPLATE_LAST};
    static lv_coord_t row_dsc[] = {110, 110, 110, 110, 110, LV_GRID_TEMPLATE_LAST};
        
        /*Create a container with grid*/
    cont = lv_obj_create(ui_tileView);
    lv_obj_set_style_grid_column_dsc_array(cont, col_dsc, 0);
    lv_obj_set_style_grid_row_dsc_array(cont, row_dsc, 0);
    lv_obj_set_size(cont, 320, 400);
    // lv_obj_center(cont);
    lv_obj_set_align(cont, LV_ALIGN_TOP_MID);
    lv_obj_set_layout(cont, LV_LAYOUT_GRID);

    lv_obj_set_style_pad_top(cont, 20, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(cont, 20, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(cont, 20, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_column(cont, 20, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_row(cont, 20, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(cont, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(cont, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(cont, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_scrollbar_mode(cont, LV_SCROLLBAR_MODE_OFF);

    
    ui_start_app = create_tile(cont, "Alarm",      LV_SYMBOL_SETTINGS, 0, 0, 1, 1, event_launch);
    ui_start_app = create_tile(cont, "Weather",    LV_SYMBOL_SETTINGS, 1, 0, 1, 1, event_launch);
    ui_start_app = create_tile(cont, "Messaging",  LV_SYMBOL_SETTINGS, 2, 0, 1, 1, event_launch);
    
    ui_start_app = create_tile(cont, "Stoper",     LV_SYMBOL_SETTINGS, 0, 1, 1, 1, event_launch);
    ui_start_app = create_tile(cont, "Calendar",   LV_SYMBOL_SETTINGS, 1, 1, 1, 1, event_launch);
    ui_start_app = create_tile(cont, "Files",      LV_SYMBOL_SD_CARD,  2, 1, 1, 1, event_launch);

    ui_start_app = create_tile(cont, "Phone",      LV_SYMBOL_SETTINGS, 0, 2, 1, 1, event_launch);
    ui_start_app = create_tile(cont, "Photos",     LV_SYMBOL_SETTINGS, 1, 2, 1, 1, event_launch);
    ui_start_app = create_tile(cont, "Settings",   LV_SYMBOL_SETTINGS, 2, 2, 1, 1, event_launch);

    ui_start_app = create_tile(cont, "OneDrive",   LV_SYMBOL_SETTINGS, 0, 3, 1, 1, event_launch);
    ui_start_app = create_tile(cont, "Wallet",     LV_SYMBOL_SETTINGS, 1, 3, 1, 1, event_launch);
    ui_start_app = create_tile(cont, "WiFi1",      LV_SYMBOL_SETTINGS, 2, 3, 1, 1, event_launch);

    ui_start_app = create_tile(cont, "Sleep",      LV_SYMBOL_SETTINGS, 0, 4, 1, 1, event_launch);
    ui_start_app = create_tile(cont, "Calculator", LV_SYMBOL_SETTINGS, 1, 4, 1, 1, event_launch);
    ui_start_app = create_tile(cont, "Music",      LV_SYMBOL_SETTINGS, 2, 4, 1, 1, event_launch);
    
    navigation(ui_startScreen);
    notification_panel(ui_startScreen);
    status_bar(ui_startScreen);
    statusBar_show();

    power_register_cb( POWERMGM_STANDBY | POWERMGM_WAKEUP | POWERMGM_SILENCE_WAKEUP | POWERMGM_PMU_LONG_BUTTON, gui_power_event_cb, "gui" );
    power_register_loop_cb( POWERMGM_WAKEUP | POWERMGM_SILENCE_WAKEUP, gui_power_loop_event_cb, "gui loop" );
}

bool gui_power_event_cb( EventBits_t event, void *arg ) {
    // TTGOClass *ttgo = TTGOClass::getWatch();

    switch ( event ) {
        case POWERMGM_STANDBY:          //log_i("go standby");
                                        // if ( !display_get_block_return_maintile() ) {
                                            // mainbar_jump_to_maintile( LV_ANIM_OFF );
                                        // }                               
                                        // ttgo->stopLvglTick();
                                        break;
        case POWERMGM_WAKEUP:           //log_i("go wakeup");
                                        // ttgo->startLvglTick();
                                        lv_disp_trig_activity( NULL );
                                        break;
        case POWERMGM_SILENCE_WAKEUP:   //log_i("go silence wakeup");
                                        // ttgo->startLvglTick();
                                        lv_disp_trig_activity( NULL );
                                        break;
        case POWERMGM_PMU_LONG_BUTTON:  // ttgo->startLvglTick();
                                        lv_disp_trig_activity( NULL );
                                        // motor_vibe(5);
                                        // if ( display_get_screenshot() ) {
                                        //     screenshot_take();
                                        //     screenshot_save();
                                        // }
                                        // else {
                                        //     mainbar_jump_to_maintile( LV_ANIM_OFF );
                                        // }
                                        break;
        case POWERMGM_DISABLE_INTERRUPTS:
                                        // TTGOClass::getWatch()->stopLvglTick();
                                        break;
        case POWERMGM_ENABLE_INTERRUPTS:
                                        // TTGOClass::getWatch()->startLvglTick();
                                        break;                                        
    }
    return( true );
}

bool gui_power_loop_event_cb( EventBits_t event, void *arg ) {
    switch ( event ) {
        case POWERMGM_WAKEUP:           if ( lv_disp_get_inactive_time( NULL ) < display_get_display_times() * 1000 || display_get_display_times() == DISPLAY_MAX_TIMEOUT ) {
                                            lv_task_handler();
                                        }
                                        else {
                                            power_set_event( POWERMGM_STANDBY_REQUEST );
                                        }
                                        break;
        case POWERMGM_SILENCE_WAKEUP:   if ( lv_disp_get_inactive_time( NULL ) < display_get_display_times() * 1000 ) {
                                            lv_task_handler();
                                        }
                                        else {
                                            power_set_event( POWERMGM_STANDBY_REQUEST );
                                        }
                                        break;
    }
    return( true );
}

void splash_screen_stage_boot(){
    // log_i("LV load ui_bootScreen");

    ui_bootScreen = lv_obj_create(lv_scr_act());
    lv_obj_clear_flag(ui_bootScreen, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_width(ui_bootScreen, lv_pct(100));
    lv_obj_set_height(ui_bootScreen, lv_pct(100));
    lv_obj_set_x(ui_bootScreen, 0);
    lv_obj_set_y(ui_bootScreen, 0);
    lv_obj_set_align(ui_bootScreen, LV_ALIGN_CENTER);

    /*Create an Arc*/
    lv_obj_t * arc = lv_arc_create(ui_bootScreen);
    lv_obj_set_size(arc, 200, 200);
    lv_arc_set_rotation(arc, 270);
    lv_arc_set_bg_angles(arc, 0, 360);
    // lv_arc_set_change_rate(arc, 30);
    lv_arc_set_value(arc, 0);
    lv_obj_remove_style(arc, NULL, LV_PART_KNOB);   /*Be sure the knob is not displayed*/
    lv_obj_clear_flag(arc, LV_OBJ_FLAG_CLICKABLE);  /*To not allow adjusting by click*/
    lv_obj_center(arc);

    lv_obj_t * ui_bootlabel= lv_label_create(ui_bootScreen);
    lv_obj_set_width(ui_bootlabel, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_bootlabel, LV_SIZE_CONTENT);
    lv_obj_set_x(ui_bootlabel, 0);
    lv_obj_set_y(ui_bootlabel, 0);
    lv_obj_set_align(ui_bootlabel, LV_ALIGN_CENTER);
    lv_label_set_text(ui_bootlabel, LV_SYMBOL_POWER);
    lv_obj_set_style_text_font(ui_bootlabel, &lv_font_montserrat_48, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_timer_handler();

    for( int bl = 0 ; bl < display_get_display_brig() ; bl++ ) {
        tftt.setBrightness( bl );
        delay(5);
    }  

    for (int i = 0; i < 100; i++){
        lv_arc_set_value(arc, i);
        lv_timer_handler();
        delay(30);    
    }
    
    for( int bl = display_get_display_brig() ; bl >= 0 ; bl-- ) {
        tftt.setBrightness( bl );
        delay(5);
    }
    lv_obj_del(ui_bootScreen);
    lv_timer_handler();
}

// void splash_screen_stage_update( const char* msg, int value ) {;
//     // lv_disp_trig_activity( NULL );
//     // lv_timer_handler();
//     // //delay(100);
//     // lv_bar_set_value( preload, value, LV_ANIM_ON );
//     // lv_label_set_text( preload_label, msg );
    
//     // for( int bl = 0 ; bl < 500 ; bl++ ) {
//     //     lv_timer_handler();
//     //     delay(1);
//     // }
// }

// void splash_screen_stage_finish( void ) {
//     // for( int bl = display_get_display_brig() ; bl >= 0 ; bl-- ) {
//     //     tftt.setBrightness( bl );
//     //     delay(5);
//     // }
//     // lv_obj_del(ui_bootScreen);
//     // lv_timer_handler();
// }

// panel notyfication
void create_action_tile(lv_obj_t *parent, lv_obj_t *obj2, char *name, const void *src, bool checked, int col, int row, int size, int action, lv_event_cb_t callback){
    lv_obj_t *label;
    lv_obj_t *icon;
    obj2 = lv_btn_create(parent);
    lv_obj_set_style_radius(obj2, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_grid_cell(obj2, LV_GRID_ALIGN_STRETCH, col, size, LV_GRID_ALIGN_STRETCH, row, 1);
    lv_obj_add_flag(obj2, LV_OBJ_FLAG_CHECKABLE | LV_OBJ_FLAG_SCROLL_ON_FOCUS);
    lv_obj_add_event_cb(obj2, callback, LV_EVENT_VALUE_CHANGED , NULL);
    if (!checked){
        lv_obj_add_state(obj2, LV_STATE_CHECKED); /*Make the chekbox checked*/
    }else{
        lv_obj_clear_state(obj2, LV_STATE_CHECKED); /*MAke the checkbox unchecked*/
    }
    lv_obj_clear_flag(obj2, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_opa(obj2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(obj2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(obj2, 10, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_opa(obj2, 255, LV_PART_MAIN | LV_STATE_CHECKED);

    icon = lv_label_create(obj2);
    lv_label_set_text(icon, statusIcon[action].symbol);
    lv_obj_set_align(icon, LV_ALIGN_CENTER);
    lv_obj_set_style_text_font(icon, &lv_font_montserrat_28, LV_PART_MAIN | LV_STATE_DEFAULT);
}

void notification_panel(lv_obj_t *parent){

    // ui_notificationPanel
    ui_notificationPanel = lv_obj_create(parent);
    lv_obj_set_width(ui_notificationPanel, 320);
    lv_obj_set_height(ui_notificationPanel, 480);
    lv_obj_set_x(ui_notificationPanel, 0);
    lv_obj_set_y(ui_notificationPanel, -430);
    lv_obj_set_align(ui_notificationPanel, LV_ALIGN_TOP_MID);
    lv_obj_add_flag(ui_notificationPanel, LV_OBJ_FLAG_CHECKABLE);
    lv_obj_clear_flag(ui_notificationPanel, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_scrollbar_mode(ui_notificationPanel, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_style_radius(ui_notificationPanel, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    // lv_obj_set_style_bg_color(ui_notificationPanel, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_notificationPanel, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_notificationPanel, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_all(ui_notificationPanel, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_opa(ui_notificationPanel, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_event_cb(ui_notificationPanel, ui_event_notificationPanel, LV_EVENT_ALL, NULL);

    // ui_actionPanel
    ui_actionPanel = lv_obj_create(ui_notificationPanel);

    static lv_coord_t col_dsc[] = {60, 60, 60, 60, LV_GRID_TEMPLATE_LAST};
    static lv_coord_t row_dsc[] = {60, 60, 60, LV_GRID_TEMPLATE_LAST};

    lv_obj_set_style_grid_column_dsc_array(ui_actionPanel, col_dsc, 0);
    lv_obj_set_style_grid_row_dsc_array(ui_actionPanel, row_dsc, 0);
    lv_obj_set_width(ui_actionPanel, 320);
    lv_obj_set_height(ui_actionPanel, LV_SIZE_CONTENT);
    lv_obj_set_x(ui_actionPanel, 0);
    lv_obj_set_y(ui_actionPanel, 50);
    lv_obj_set_align(ui_actionPanel, LV_ALIGN_TOP_MID);
    lv_obj_clear_flag(ui_actionPanel, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_scrollbar_mode(ui_actionPanel, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_layout(ui_actionPanel, LV_LAYOUT_GRID);
    lv_obj_set_style_radius(ui_actionPanel, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    // lv_obj_set_style_bg_color(ui_actionPanel, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_actionPanel, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_actionPanel, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_column(ui_actionPanel, 16, LV_PART_MAIN | LV_STATE_DEFAULT);

    create_action_tile(ui_actionPanel, ui_btnwifi,  "WiFi",     NULL, wifictl_get_autoon() , 0, 0, 1, 2, event_action_wifi);
    create_action_tile(ui_actionPanel, ui_btnble,   "Buetooth", NULL, false, 1, 0, 1, 3, event_action_ble);
    create_action_tile(ui_actionPanel, ui_btnvol,   "Volume",   NULL, false, 2, 0, 1, 4, event_action_vol);
    create_action_tile(ui_actionPanel, ui_btnflash, "Flash",    NULL, false, 3, 0, 1, 7, event_action_flash);

    // ui_actionDate
    ui_actionDate = lv_label_create(ui_notificationPanel);
    lv_obj_set_width(ui_actionDate, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_actionDate, LV_SIZE_CONTENT);
    lv_obj_set_x(ui_actionDate, 10);
    lv_obj_set_y(ui_actionDate, 30);
    lv_obj_set_align(ui_actionDate, LV_ALIGN_TOP_LEFT);
    lv_label_set_text(ui_actionDate, "Czw, 3 lis");
    lv_obj_set_style_text_font(ui_actionDate, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);

    // ui_actionBattery
    ui_actionBattery = lv_label_create(ui_notificationPanel);
    lv_obj_set_width(ui_actionBattery, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_actionBattery, LV_SIZE_CONTENT);
    lv_obj_set_x(ui_actionBattery, -10);
    lv_obj_set_y(ui_actionBattery, 30);
    lv_obj_set_align(ui_actionBattery, LV_ALIGN_TOP_RIGHT);
    lv_label_set_text(ui_actionBattery, "35%");
    lv_obj_set_style_text_font(ui_actionBattery, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui_actionBattery, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui_actionBattery, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui_actionBattery, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui_actionBattery, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // ui_notificationText
    ui_notificationText = lv_label_create(ui_notificationPanel);
    lv_obj_set_width(ui_notificationText, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_notificationText, LV_SIZE_CONTENT);
    lv_obj_set_x(ui_notificationText, 10);
    lv_obj_set_y(ui_notificationText, 140);
    lv_label_set_text(ui_notificationText, "No notifications");
    lv_obj_set_style_text_font(ui_notificationText, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);

    // ui_dragPanel
    ui_dragPanel = lv_obj_create(ui_notificationPanel);
    lv_obj_set_width(ui_dragPanel, 320);
    lv_obj_set_height(ui_dragPanel, 10);
    lv_obj_set_x(ui_dragPanel, 0);
    lv_obj_set_y(ui_dragPanel, 0);
    lv_obj_set_align(ui_dragPanel, LV_ALIGN_BOTTOM_MID);
    lv_obj_clear_flag(ui_dragPanel, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_scrollbar_mode(ui_dragPanel, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_style_radius(ui_dragPanel, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_dragPanel, lv_palette_main(LV_PALETTE_BLUE), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_dragPanel, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_dragPanel, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // ui_Label7
    ui_Label7 = lv_label_create(ui_dragPanel);
    lv_obj_set_width(ui_Label7, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_Label7, LV_SIZE_CONTENT);
    lv_obj_set_x(ui_Label7, 0);
    lv_obj_set_y(ui_Label7, -5);
    lv_obj_set_align(ui_Label7, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label7, "_____");
}

void navigation(lv_obj_t *parent)
{
    // ui_Panel2 ui_backButton ui_startButton

    ui_Panel2 = lv_obj_create(parent);

    lv_obj_set_width(ui_Panel2, 320);
    lv_obj_set_height(ui_Panel2, 50);

    lv_obj_set_x(ui_Panel2, 0);
    lv_obj_set_y(ui_Panel2, 0);

    lv_obj_set_align(ui_Panel2, LV_ALIGN_BOTTOM_MID);

    lv_obj_clear_flag(ui_Panel2, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_set_style_radius(ui_Panel2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    // lv_obj_set_style_bg_color(ui_Panel2, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Panel2, 25, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_Panel2, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    // lv_obj_set_style_border_side(ui_Panel2, LV_BORDER_SIDE_TOP, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_all(ui_Panel2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // ui_startButton
    lv_obj_t * label;

    ui_startButton = lv_obj_create(ui_Panel2);
    lv_obj_set_width(ui_startButton, 80);
    lv_obj_set_height(ui_startButton, 40);
    lv_obj_set_x(ui_startButton, 60);
    lv_obj_set_y(ui_startButton, 0);
    lv_obj_set_align(ui_startButton, LV_ALIGN_CENTER);
    lv_obj_set_style_radius(ui_startButton, LV_RADIUS_CIRCLE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_startButton, LV_OPA_0, LV_PART_MAIN | LV_STATE_DEFAULT); 
    lv_obj_set_style_border_opa(ui_startButton, LV_OPA_0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_all(ui_startButton, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_clear_flag(ui_startButton, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_add_event_cb(ui_startButton, event_navigate, LV_EVENT_ALL, NULL);

    label = lv_img_create(ui_startButton);
    lv_img_set_src(label, LV_SYMBOL_STOP);
    lv_obj_set_width(label, LV_SIZE_CONTENT);
    lv_obj_set_height(label, LV_SIZE_CONTENT);
    lv_obj_add_flag(label, LV_OBJ_FLAG_ADV_HITTEST);
    lv_obj_clear_flag(label, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_align(label, LV_ALIGN_CENTER);
    lv_obj_set_style_text_font(label, &lv_font_montserrat_28, LV_PART_MAIN | LV_STATE_DEFAULT);

    // ui_backButton

    ui_backButton = lv_obj_create(ui_Panel2);
    lv_obj_set_width(ui_backButton, 80);
    lv_obj_set_height(ui_backButton, 40);
    lv_obj_set_x(ui_backButton, -60);
    lv_obj_set_y(ui_backButton, 0);
    lv_obj_set_align(ui_backButton, LV_ALIGN_CENTER);
    lv_obj_set_style_radius(ui_backButton, LV_RADIUS_CIRCLE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_backButton, LV_OPA_0, LV_PART_MAIN | LV_STATE_DEFAULT); 
    lv_obj_set_style_border_opa(ui_backButton, LV_OPA_0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_all(ui_backButton, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_clear_flag(ui_backButton, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_add_event_cb(ui_backButton, event_navigate, LV_EVENT_ALL, NULL);

    label = lv_img_create(ui_backButton);
    lv_img_set_src(label, LV_SYMBOL_LEFT);
    lv_obj_set_width(label, LV_SIZE_CONTENT);
    lv_obj_set_height(label, LV_SIZE_CONTENT);
    lv_obj_add_flag(label, LV_OBJ_FLAG_ADV_HITTEST);
    lv_obj_clear_flag(label, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_align(label, LV_ALIGN_CENTER);
    lv_obj_set_style_text_font(label, &lv_font_montserrat_28, LV_PART_MAIN | LV_STATE_DEFAULT);


}