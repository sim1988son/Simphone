#include <Arduino.h>
#include <WiFi.h>
// #include <vector>
// #include <string.h>
// #include "littleFS.h"
// #include "hardware/json_psram_allocator.h"
// #include "hardware/alloc.h"

#include "config.h"
#include "hardware/powerset.h"
#include "hardware/wifiset.h"
#include "ui/ui.h"
#include "ui/setup/setup.h"
#include "ui/setup/setup_wlan/wlan.h"
// #include "ui/setup/setup_datetime/datetime.h"


TaskHandle_t ntScanTaskHandler;

const char * wifiobj=NULL;


lv_obj_t * ui_wifiScreen;
lv_obj_t * ui_wifilabelip;
lv_obj_t * ui_wifibtn;
lv_obj_t * ui_wifilabel_onoff;
lv_obj_t * ui_wifilabel_standby;
lv_obj_t * ui_wifilabel_autoon;
lv_obj_t * ui_wifiswitch_onoff;
lv_obj_t * ui_wifiswitch_standby;
lv_obj_t * ui_wifiswitch_autoon;
lv_obj_t * ui_wifispinner;
lv_obj_t * listwifi;
lv_obj_t * ui_wifimboxlist;
lv_obj_t * ui_wifimboxlistbtnOK;
lv_obj_t * ui_wifimboxlistbtnCL;
lv_obj_t * ui_wifimboxlistbtnTR;
lv_obj_t * ui_wifimboxssid;
lv_obj_t * ui_wifimboxPassword;
// lv_timer_t * timer_wifi;
lv_obj_t * ui_systemKeyboard;


static void ui_wifiScreen_event(lv_event_t *e);
static void wifi_onoff_event_handler(lv_event_t *e);
static void wifi_enabled_on_standby_onoff_event_handler(lv_event_t *e);
static void wifi_autoon_onoff_event_handler(lv_event_t *e);

bool wifi_setup_wifictl_event_cb( EventBits_t event, void *arg );
bool wifi_setup_autoon_event_cb( EventBits_t event, void *arg );

void keyboard(lv_obj_t *parent)
{
    // ui_systemKeyboard
    ui_systemKeyboard = lv_keyboard_create(parent);
    lv_obj_clear_flag(ui_systemKeyboard, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_set_width(ui_systemKeyboard, 320);
    lv_obj_set_height(ui_systemKeyboard, 150);

    lv_obj_set_x(ui_systemKeyboard, 0);
    lv_obj_set_y(ui_systemKeyboard, 0);

    lv_obj_set_align(ui_systemKeyboard, LV_ALIGN_BOTTOM_MID);

    lv_obj_add_flag(ui_systemKeyboard, LV_OBJ_FLAG_HIDDEN);

    lv_obj_set_style_radius(ui_systemKeyboard, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_systemKeyboard, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_systemKeyboard, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_systemKeyboard, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_set_style_radius(ui_systemKeyboard, 0, LV_PART_ITEMS | LV_STATE_DEFAULT);
}

static void ui_wifiScreen_event(lv_event_t *e){
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj = lv_event_get_target(e);

    if (code == LV_EVENT_CLICKED){
        
        if (obj == ui_wifimboxlistbtnOK) {
            wifiset_insert_network( wifiobj, lv_textarea_get_text( ui_wifimboxPassword ) );
            // networkConnector();
            lv_obj_add_flag(ui_wifimboxlist, LV_OBJ_FLAG_HIDDEN);
        } else if (obj == ui_wifimboxlistbtnCL) {
            lv_obj_add_flag(ui_wifimboxlist, LV_OBJ_FLAG_HIDDEN);
        } else if (obj == ui_wifimboxlistbtnTR){
            wifiset_delete_network(wifiobj);
        }
        // wifiset_save_config();
    }
}

static void wifi_onoff_event_handler(lv_event_t *e){
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj = lv_event_get_target(e);

    if (code == LV_EVENT_VALUE_CHANGED){
        if (lv_obj_has_state(obj, LV_STATE_CHECKED)){
            log_i("WIFION EVENT TRUE");
            wifictl_on();
        } else {
            log_i("WIFION EVENT FALSE");
            wifictl_off();
        } 
         
    }
}

static void wifi_enabled_on_standby_onoff_event_handler(lv_event_t *e){
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj = lv_event_get_target(e);

    if (code == LV_EVENT_VALUE_CHANGED){
        if (lv_obj_has_state(obj, LV_STATE_CHECKED)){
            // log_i("STANBY EVENT TRUE");
            wifictl_set_enable_on_standby(lv_obj_has_state(obj, LV_STATE_CHECKED));
        } else {
            // log_i("STANBY EVENT FALSE");
            wifictl_set_enable_on_standby(lv_obj_has_state(obj, LV_STATE_CHECKED));
        }
        // wifiset_save_config();
    }
}

static void wifi_autoon_onoff_event_handler(lv_event_t *e){
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj = lv_event_get_target(e);

    if (code == LV_EVENT_VALUE_CHANGED){
        if (lv_obj_has_state(obj, LV_STATE_CHECKED)){
            log_i("AUTOON EVENT TRUE");
            wifictl_set_autoon(lv_obj_has_state(obj, LV_STATE_CHECKED));
        } else {
            log_i("AUTOON EVENT FALSE");
            wifictl_set_autoon(lv_obj_has_state(obj, LV_STATE_CHECKED));
        }
        // wifiset_save_config();
    }
}

static void event_text_input(lv_event_t *e) {
  lv_event_code_t code = lv_event_get_code(e);
  lv_obj_t *ta = lv_event_get_target(e);

  if (code == LV_EVENT_FOCUSED) {
    lv_obj_move_foreground(ui_systemKeyboard);
    lv_keyboard_set_textarea(ui_systemKeyboard, ta);
    lv_obj_clear_flag(ui_systemKeyboard, LV_OBJ_FLAG_HIDDEN);
  }

  if (code == LV_EVENT_DEFOCUSED) {
    lv_keyboard_set_textarea(ui_systemKeyboard, NULL);
    lv_obj_add_flag(ui_systemKeyboard, LV_OBJ_FLAG_HIDDEN);
  }
}

void openAppwifi(){

    ui_wifiScreen = create_header(ui_app_settingsScreen(), "WiFi");
    ui_wifibtn = create_btn_header(ui_wifiScreen, LV_SYMBOL_LEFT, 0, 0, 70, 50, event_setup_back, ui_wifiScreen);

    // wifictl_set_enable_on_standby(false);

    ui_wifilabel_onoff = create_label(ui_wifiScreen,0, "WiFI" , 20, 70, 20);
    ui_wifiswitch_onoff = create_switch(ui_wifiScreen, 70, wifi_onoff_event_handler);

    ui_wifilabel_autoon = create_label(ui_wifiScreen,0, "WiFI auto ON" , 20, 120, 20);
    ui_wifiswitch_autoon = create_switch(ui_wifiScreen, 120, wifi_autoon_onoff_event_handler);
    
    ui_wifilabel_standby = create_label(ui_wifiScreen,0, "WiFI stanBY enable" , 20, 170, 20);
    ui_wifiswitch_standby = create_switch(ui_wifiScreen, 170, wifi_enabled_on_standby_onoff_event_handler);
    
    // ui_wifilabelip = create_label(ui_wifiScreen,0, "Stan" , 20, 150, 20);

    /*Create a spinner*/
    ui_wifispinner = lv_spinner_create(ui_wifiScreen, 400, 60);
    lv_obj_add_flag(ui_wifispinner, LV_OBJ_FLAG_HIDDEN);
    lv_obj_set_size(ui_wifispinner, 25, 25);
    lv_obj_set_x(ui_wifispinner, -100);
    lv_obj_set_y(ui_wifispinner, 70);
    lv_obj_set_style_arc_width(ui_wifispinner, 5, LV_PART_INDICATOR);
    lv_obj_set_style_arc_width(ui_wifispinner, 5, LV_PART_MAIN);
    lv_obj_set_align(ui_wifispinner,LV_ALIGN_TOP_RIGHT);

    listwifi = lv_list_create(ui_wifiScreen);
    lv_obj_set_size(listwifi, 320, 260);
    lv_obj_set_x(listwifi, 0);
    lv_obj_set_y(listwifi, 220);
    lv_obj_set_align(listwifi, LV_ALIGN_TOP_MID);

    lv_obj_set_style_pad_top(listwifi, 30, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(listwifi, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(listwifi, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(listwifi, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(listwifi, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(listwifi, 100, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_wifimboxlist = create_mbox(ui_wifiScreen, " ", 20, 130, 280, 240);
    lv_obj_add_flag(ui_wifimboxlist, LV_OBJ_FLAG_HIDDEN);
    ui_wifimboxlistbtnOK = create_mboxbtn(ui_wifimboxlist, ui_wifiScreen_event, true);
    ui_wifimboxlistbtnCL = create_mboxbtn(ui_wifimboxlist, ui_wifiScreen_event, false);
    
    ui_wifimboxlistbtnTR = lv_btn_create(ui_wifimboxlist);
    lv_obj_set_width(ui_wifimboxlistbtnTR, 40);  /// 1
    lv_obj_set_height(ui_wifimboxlistbtnTR, 40); /// 1
    lv_obj_set_x(ui_wifimboxlistbtnTR, 0);
    lv_obj_set_y(ui_wifimboxlistbtnTR, 0);
    lv_obj_set_align(ui_wifimboxlistbtnTR, LV_ALIGN_TOP_RIGHT);
    lv_obj_set_style_radius(ui_wifimboxlistbtnTR, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_wifimboxlistbtnTR, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_wifimboxlistbtnTR, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_all(ui_wifimboxlistbtnTR, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_event_cb(ui_wifimboxlistbtnTR, ui_wifiScreen_event, LV_EVENT_ALL, NULL);

    lv_obj_t *label = lv_label_create(ui_wifimboxlistbtnTR);
    lv_obj_set_x(label, 0);
    lv_obj_set_y(label, 0);
    lv_obj_set_align(label, LV_ALIGN_CENTER);
    lv_label_set_text(label, LV_SYMBOL_TRASH);
    // lv_obj_set_style_text_font(label, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_wifimboxssid = create_label(ui_wifimboxlist,0, "SSID" , 20, 10, 20);

    ui_wifimboxPassword = lv_textarea_create(ui_wifimboxlist);
    lv_obj_set_size(ui_wifimboxPassword, 240, 40);
    lv_obj_set_x(ui_wifimboxPassword, 0);
    lv_obj_set_y(ui_wifimboxPassword, -10);
    lv_obj_set_align(ui_wifimboxPassword, LV_ALIGN_LEFT_MID);
    lv_textarea_set_placeholder_text(ui_wifimboxPassword, "Password?");
    lv_obj_add_event_cb(ui_wifimboxPassword, event_text_input, LV_EVENT_ALL, NULL); //ui_systemKeyboard

    keyboard(ui_wifiScreen);

    // uint32_t parm_id;
    if ( wifictl_get_autoon() ) {
        lv_wifi_auto_on();
    } else {
        lv_wifi_auto_on();
    }

    if ( wifictl_get_enable_on_standby() ) {
        lv_obj_add_state(ui_wifiswitch_standby, LV_STATE_CHECKED);
        log_i("STANBY GET TRUE");
    } else {
        lv_obj_clear_state(ui_wifiswitch_standby, LV_STATE_CHECKED); 
        log_i("STANBY GET TRUE");     
    }
    wifictl_register_cb( WIFICTL_AUTOON, wifi_setup_autoon_event_cb, "wifi setup");
    wifictl_register_cb( WIFICTL_ON | WIFICTL_OFF | WIFICTL_SCAN , wifi_setup_wifictl_event_cb, "wifi network scan" );
    
}

bool wifi_setup_wifictl_event_cb( EventBits_t event, void *arg ) {
    switch( event ) {
        case    WIFICTL_ON:
            log_i("WIFION SET ON");
            lv_obj_clear_flag(ui_wifispinner, LV_OBJ_FLAG_HIDDEN);
            lv_wifi_on();
            break;
        case    WIFICTL_OFF:
            log_i("WIFION SET OFF");
            lv_obj_add_flag(ui_wifispinner, LV_OBJ_FLAG_HIDDEN);
            lv_wifi_off();
            lv_obj_clean(listwifi);
            break;
        case    WIFICTL_SCAN:
            log_i("WIFION SET SCAN");
            lv_obj_clean(listwifi); 
            int len = WiFi.scanComplete();
            for( int i = 0 ; i < len ; i++ ) {
                if ( wifiset_is_known( WiFi.SSID(i).c_str() ) ) {
                add_wifi_name_list_btn(listwifi, WiFi.SSID(i).c_str(), LV_SYMBOL_EYE_OPEN);
                } else {
                add_wifi_name_list_btn(listwifi, WiFi.SSID(i).c_str(), LV_SYMBOL_EYE_CLOSE);
                }
            } 
            lv_obj_add_flag(ui_wifispinner, LV_OBJ_FLAG_HIDDEN);          
            break;
    }
    return( true );
}

bool wifi_setup_autoon_event_cb( EventBits_t event, void *arg ) {
    switch( event ) {
        case WIFICTL_AUTOON:
            if ( *(bool*)arg ) {
                lv_wifi_auto_on();
            }
            else {
                lv_wifi_auto_off();
            }
            break;
    }
    return( true );
}

void lv_wifi_on(void){
    log_i("WIFION SET TRUE");
    uint32_t parm_id = 1;
    lv_event_send(ui_wifiswitch_onoff, LV_EVENT_VALUE_CHANGED, &parm_id);
}

void lv_wifi_off(void){
    log_i("WIFION RES TRUE");
    uint32_t parm_id = 0;
    lv_event_send(ui_wifiswitch_onoff, LV_EVENT_VALUE_CHANGED, &parm_id);
}

void lv_wifi_auto_on(void){
    log_i("AUTOON SET TRUE");
    lv_label_set_text(ui_wifilabel_autoon, "WiFI auto ON o");
    lv_obj_add_state(ui_wifiswitch_autoon, LV_STATE_CHECKED);
}

void lv_wifi_auto_off(void){
    log_i("AUTOON RES TRUE");
    lv_label_set_text(ui_wifilabel_autoon, "WiFI auto Of o");
    lv_obj_clear_state(ui_wifiswitch_autoon, LV_STATE_CHECKED);
}

// void lv_wifi_stanby_on(void){
//     uint32_t parm_id = 1;
//     // lv_obj_add_state(ui_wifiswitch_standby, LV_STATE_CHECKED); //LY_KEY_ENTER 
//     lv_event_send(ui_wifiswitch_standby, LV_EVENT_VALUE_CHANGED, &parm_id);
// }

// void lv_wifi_stanby_off(void){
//     // lv_obj_clear_state(ui_wifiswitch_standby, LV_STATE_CHECKED);

//     uint32_t parm_id = 0;
//     lv_event_send(ui_wifiswitch_standby, LV_EVENT_VALUE_CHANGED, &parm_id);
// }

void event_mbox(lv_event_t *e){
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj = lv_event_get_target(e);
    
    if (code == LV_EVENT_CLICKED){
        wifiobj =  lv_list_get_btn_text(listwifi, obj); 
        lv_label_set_text(ui_wifimboxssid, wifiobj);    
        lv_obj_clear_flag(ui_wifimboxlist, LV_OBJ_FLAG_HIDDEN);
    }
}


void add_wifi_name_list_btn(lv_obj_t *parent, const char *name, const char *src)
{
    lv_obj_t *obj;
    lv_obj_t *icon;
    lv_obj_t *label;

    obj = lv_obj_create(parent);
    lv_obj_set_width(obj,280);  /// 1
    lv_obj_set_height(obj, 40); /// 1
    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(obj, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(obj, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_opa(obj, 50, LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_event_cb(obj, event_mbox, LV_EVENT_ALL, &name);

    label = lv_label_create(obj);
    lv_label_set_text(label, name);
    lv_obj_center(label);
    lv_obj_set_align(label, LV_ALIGN_LEFT_MID);
    lv_obj_set_x(label, 30);
    // lv_obj_set_style_text_font(label, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);

    icon = lv_label_create(obj);
    lv_label_set_text(icon, src);
    lv_obj_center(icon);
    lv_obj_set_align(icon, LV_ALIGN_LEFT_MID);
    lv_obj_set_x(icon, 0);
    // lv_obj_set_style_text_font(icon, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);

}



