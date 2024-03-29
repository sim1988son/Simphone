#include <Arduino.h>
#include <string.h>
#include "config.h"
#include "ui/ui.h"
#include "utils/lang.h"
#include "ui/setup/setup.h"
#include "ui/setup/setup_about/about.h"
#include "ui/setup/setup_battery/battery.h"
#include "ui/setup/setup_bluetooth/bluetooth.h"
#include "ui/setup/setup_color/color.h"
#include "ui/setup/setup_start/start.h"
#include "ui/setup/setup_datetime/datetime.h"
#include "ui/setup/setup_display/display.h"
#include "ui/setup/setup_language/language.h"
#include "ui/setup/setup_lockscreen/lockscreen.h"
#include "ui/setup/setup_sim/SIM.h"
#include "ui/setup/setup_sound/sound.h"
#include "ui/setup/setup_storage/storage.h"
#include "ui/setup/setup_update/update.h"
#include "ui/setup/setup_wlan/wlan.h"



lv_obj_t * ui_settingsScreen;
// lv_obj_t * ui_setbtn;
lv_obj_t * list1;
lv_obj_t * ui_appScreen;
lv_obj_t * ui_appPanel;
lv_obj_t * ui_appHeader;
lv_obj_t * ui_appLabel;

//=====================================================
void event_setup_back(lv_event_t *e){
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj = lv_event_get_target(e);

    lv_obj_t * data = (lv_obj_t *)lv_event_get_user_data(e);
    if (code == LV_EVENT_CLICKED){
        log_i("close app  ");
        lv_obj_del(data); 
    }
}

//===== event =====
static void event_setup_open(lv_event_t *e){
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj = lv_event_get_target(e);
    if (code == LV_EVENT_CLICKED){
        // char buf[150];
        char *data = (char *)lv_event_get_user_data(e);
        // Settings list
        if (data == "Display"){
            openAppDisplay();
        }
        else if (data == "Battery"){
            openAppBattery();
        }
        else if (data == "Storage"){
            openAppStorage();
        }
        else if (data == "About"){
            openAppAbout();
        }
        // Settings list - Net 
        else if (data == "SIM"){
            openAppSIM();
        }
        else if (data == "WiFi"){
            openAppwifi();
        }
        else if (data == "Bluetooth"){
            openAppBluetooth();
        }
        // Settings list - Personalization
        else if (data == "Start"){
            openAppStart();
        }
        else if (data == "Colors"){
            openAppColors();
        }
        else if (data == "Sound"){
            openAppSound();
        }
        else if (data == "Lock screen"){
            openAppLockScreen();
        }
        // Settings list - Time
        else if (data == "Date & time"){
            openAppDateTime();
        }
        else if (data == "Language"){
            openAppLanguage();
        }
        // Settings list - Update
        else if (data == "Update"){
            openAppUpdate();
        }
    }
}

lv_obj_t *create_obj(lv_obj_t *parent, uint16_t _pad_top)
{
    lv_obj_t * panel = lv_obj_create(parent);
    lv_obj_set_width(panel, 320);
    lv_obj_set_height(panel, 480);
    lv_obj_set_align(panel, LV_ALIGN_TOP_MID);
    lv_obj_set_scrollbar_mode(panel, LV_SCROLLBAR_MODE_ACTIVE);
    lv_obj_clear_flag(panel, LV_OBJ_FLAG_SCROLLABLE); /// Flags
    lv_obj_set_style_radius(panel, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(panel, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(panel, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(panel, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(panel, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(panel, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(panel, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_width(panel, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_pad(panel, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
return panel;
}

lv_obj_t *create_header(lv_obj_t *parent, const char *text)
{
    lv_obj_t * header = lv_obj_create(parent);
    lv_obj_set_width(header, 320);
    lv_obj_set_height(header, 70);
    lv_obj_set_align(header, LV_ALIGN_TOP_MID);
    lv_obj_clear_flag(header, LV_OBJ_FLAG_SCROLLABLE); /// Flags
    lv_obj_set_style_radius(header, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    // lv_obj_set_style_bg_color(header, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(header, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(header, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t * label = lv_label_create(header);
    lv_obj_set_width(label, LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height(label, LV_SIZE_CONTENT); /// 1
    lv_obj_set_x(label, 0);
    lv_obj_set_y(label, 0);
    lv_obj_set_align(label, LV_ALIGN_BOTTOM_MID);
    lv_label_set_text(label, text);
    lv_obj_set_style_text_font(label, &lv_font_montserrat_22, LV_PART_MAIN | LV_STATE_DEFAULT);
    return header;
}

lv_obj_t *create_btn_header(lv_obj_t *parent, const char *text, uint16_t xPos, uint16_t yPos, uint16_t width, uint16_t height, lv_event_cb_t callback, lv_obj_t *data)
{
    lv_obj_t *button = lv_obj_create(parent);
    lv_obj_set_width(button, width);  /// 1
    lv_obj_set_height(button, height); /// 1
    lv_obj_set_x(button, xPos);
    lv_obj_set_y(button, yPos);
    lv_obj_set_align(button, LV_ALIGN_TOP_LEFT);
    lv_obj_clear_flag(button, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_radius(button, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(button, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(button, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_event_cb(button, callback, LV_EVENT_ALL, data);

    lv_obj_t *label = lv_label_create(button);
    lv_obj_set_align(label, LV_ALIGN_CENTER);
    lv_label_set_text(label, text);
    lv_obj_set_style_text_font(label, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
    return button;
}

void add_item(lv_obj_t *parent, char *name, const char *name2, char *src, lv_event_cb_t callback){

    lv_obj_t *obj;
    lv_obj_t *icon;
    lv_obj_t *label;
    lv_obj_t *icon2;

    obj = lv_obj_create(parent);
    // lv_obj_set_width(obj, lv_pct(30));
    lv_obj_set_width(obj, 280);
    lv_obj_set_height(obj, 35);
    lv_obj_set_style_radius(obj, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(obj, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(obj, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_event_cb(obj, callback, LV_EVENT_ALL, name);

    label = lv_label_create(obj);
    lv_label_set_text(label, name2);
    lv_obj_center(label);
    lv_obj_set_align(label, LV_ALIGN_LEFT_MID);
    lv_obj_set_x(label, 30);
    lv_obj_set_style_text_font(label, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);

    icon = lv_label_create(obj);
    lv_label_set_text(icon, src);
    lv_obj_center(icon);
    lv_obj_set_align(icon, LV_ALIGN_LEFT_MID);
    lv_obj_set_x(icon, 0);
    lv_obj_set_style_text_font(icon, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);

    icon2 = lv_label_create(obj);
    lv_label_set_text(icon2, LV_SYMBOL_RIGHT);
    lv_obj_center(icon2);
    lv_obj_set_align(icon2, LV_ALIGN_RIGHT_MID);
    lv_obj_set_x(icon2, 0);
    lv_obj_set_style_text_font(icon2, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
}

lv_obj_t * ui_app_settingsScreen(){
    return (ui_settingsScreen);
}

void ui_settingsScreen_init(void){

    ui_settingsScreen = create_obj(NULL);

    list1 = lv_list_create(ui_settingsScreen);
    lv_obj_set_size(list1, 320, 430);
    lv_obj_set_x(list1, 0);
    lv_obj_set_y(list1, 0);
    lv_obj_set_align(list1, LV_ALIGN_TOP_MID);

    // lv_obj_set_style_pad_top(list1, 30, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(list1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(list1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(list1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_scrollbar_mode(list1, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_style_pad_top(list1, 70, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(list1, 20, LV_PART_MAIN | LV_STATE_DEFAULT);

    /*Add apps to the list*/
    lv_list_add_text(list1, str_txt(STR_SYSTEM).c_str()); //"System");
    add_item(list1, "Display", str_txt(STR_DISPLAY).c_str(), LV_SYMBOL_IMAGE, &event_setup_open);
    add_item(list1, "Battery", str_txt(STR_BATTERY).c_str(), LV_SYMBOL_BATTERY_EMPTY, event_setup_open);
    add_item(list1, "Storage", str_txt(STR_STORAGE).c_str(), LV_SYMBOL_SD_CARD, event_setup_open);
    add_item(list1, "About", str_txt(STR_ABOUT).c_str(), LV_SYMBOL_HOME, event_setup_open);

    lv_list_add_text(list1, str_txt(STR_WIRELESS).c_str()); //"Net & wireless");
    add_item(list1, "SIM", str_txt(STR_SIM).c_str(), LV_SYMBOL_SETTINGS, event_setup_open);
    add_item(list1, "WiFi", str_txt(STR_WIFI).c_str(), LV_SYMBOL_WIFI, event_setup_open);
    add_item(list1, "Bluetooth", str_txt(STR_BLUETOOTH).c_str(), LV_SYMBOL_BLUETOOTH, event_setup_open);

    lv_list_add_text(list1, str_txt(STR_PERSONALIZE).c_str()); //"Personalization");
    add_item(list1, "Start", str_txt(STR_STRAT).c_str(), LV_SYMBOL_IMAGE, event_setup_open);
    add_item(list1, "Colors", str_txt(STR_COLORS).c_str(), LV_SYMBOL_SETTINGS, event_setup_open);
    add_item(list1, "Sound", str_txt(STR_SOUND).c_str(), LV_SYMBOL_VOLUME_MAX, event_setup_open);
    add_item(list1, "Lock screen", str_txt(STR_LOCKSCREEN).c_str(), LV_SYMBOL_IMAGE, event_setup_open);

    lv_list_add_text(list1, str_txt(STR_TIMELANG).c_str()); //"Time & language");
    add_item(list1, "Date & time", str_txt(STR_DATETIME).c_str(), LV_SYMBOL_BELL, event_setup_open);
    add_item(list1, "Language", str_txt(STR_LANGUAGE).c_str(), LV_SYMBOL_SETTINGS, event_setup_open);

    lv_list_add_text(list1, str_txt(STR_UPDATE).c_str()); //"Update");
    add_item(list1, "Update", str_txt(STR_UPDATE).c_str(), LV_SYMBOL_UPLOAD, event_setup_open);

    lv_obj_t * header = create_header(ui_settingsScreen, str_txt(STR_SETTINGS).c_str());
    // lv_obj_t * header = create_header(ui_settingsScreen, "Settings");
 
}

lv_obj_t * ui_app_settingsScreenApps(){
    return (ui_appScreen);
}

void ui_settingsScreenApps_init(void){
    ui_appScreen = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_appScreen, LV_OBJ_FLAG_SCROLLABLE); /// Flags
    
    ui_appPanel = create_obj(ui_appScreen);

    ui_appHeader = lv_obj_create(ui_appScreen);
    lv_obj_set_width(ui_appHeader, 320);//320
    lv_obj_set_height(ui_appHeader, 70);//72
    lv_obj_set_align(ui_appHeader, LV_ALIGN_TOP_MID);
    lv_obj_clear_flag(ui_appHeader, LV_OBJ_FLAG_SCROLLABLE); /// Flags
    lv_obj_set_style_radius(ui_appHeader, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    // lv_obj_set_style_bg_color(ui_appHeader, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_appHeader, 25, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_appHeader, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_appLabel = lv_label_create(ui_appHeader);
    lv_obj_set_width(ui_appLabel, LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height(ui_appLabel, LV_SIZE_CONTENT); /// 1
    lv_obj_set_x(ui_appLabel, 0);
    lv_obj_set_y(ui_appLabel, 0);
    lv_obj_set_align(ui_appLabel, LV_ALIGN_BOTTOM_MID);
    lv_label_set_text(ui_appLabel, "App");
    lv_obj_set_style_text_font(ui_appLabel, &lv_font_montserrat_22, LV_PART_MAIN | LV_STATE_DEFAULT);


    // AppAbout();
    // AppBattery();
    // AppBluetooth();
    // AppColors();
    // AppDateTime();
    // AppDisplay();
    // AppLanguage();
    // AppLockScreen();
    // AppSIM();
    // AppSound();
    // AppStart();
    // AppStorage();
    // AppUpdate();
    // Appwifi();
}

void launchApp(const char *name, bool header){
    openlaunch();
    if (header){
        lv_obj_clear_flag(ui_appHeader, LV_OBJ_FLAG_HIDDEN); // show the header
    }else{
        lv_obj_add_flag(ui_appHeader, LV_OBJ_FLAG_HIDDEN); // hide the header
    }
    lv_label_set_text(ui_appLabel, name);
    lv_disp_load_scr(ui_appScreen);
}

void closeApp()
{
    lv_obj_clean(ui_appPanel);
}

///////////////////// MODULES ////////////////////
lv_obj_t *app_canvas(){
    lv_obj_t *container = create_obj(ui_appPanel ,30);

    return container;
}