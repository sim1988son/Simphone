#include <Arduino.h>
#include "config.h"
#include "utils/lang.h"
#include "ui/ui.h"
#include "hardware/displayset.h"
#include "ui/setup/setup.h"
#include "ui/setup/setup_storage/storage.h"
#include "littleFS.h"
#include "SD.h"



lv_obj_t * ui_storageScreen;
lv_obj_t * ui_storagebtn;
lv_obj_t * ui_reboot_btn;
lv_obj_t * ui_format_spiffs_btn;
lv_obj_t * SpiffsWarningBox;
lv_obj_t * SpiffsWarningBoxOK;
lv_obj_t * SpiffsWarningBoxCL;
lv_obj_t * SpinBoxPanel;

static void SpiffsWarningBox_event_handler( lv_event_t *e );
// static void format_SPIFFS_utilities_event_cb( lv_event_t *e );
static void format_SPIFFS(void);

static void reboot_utilities_event_cb( lv_event_t *e );
// static void poweroff_utilities_event_cb( lv_obj_t * obj, lv_event_t event );


static void ui_storageScreen_event(lv_event_t *e){
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj = lv_event_get_target(e);
    // lv_disp_t *display = lv_disp_get_default();
    // lv_obj_t *actScr = lv_disp_get_scr_act(display);

    if (code == LV_EVENT_CLICKED){
        
    }
}

void openAppStorage(){
    closeApp();
    AppStorage();
    launchApp(str_txt(STR_STORAGE).c_str(), true);
}

void AppStorage(){
    lv_obj_t *canvas = app_canvas();
    ui_storageScreen = create_obj(canvas);
    // ui_storageScreen = create_header(ui_app_settingsScreen(), "Storage");
    // ui_storagebtn = create_btn_header(ui_storageScreen, LV_SYMBOL_LEFT, 0, 0, 70, 50, event_setup_back, ui_storageScreen);

    
    lv_obj_t * ui_storagelabel;
    char temp[80]="";
    ui_storagelabel = create_label(ui_storageScreen, 0, "Flash", 20, 70, 20);
    uint64_t total = ESP.getFlashChipSize()/1024;
    uint64_t free = LittleFS.totalBytes()/1024;
    uint64_t ussd = LittleFS.usedBytes()/1024;
    uint64_t used = total - free - ussd;
    lv_snprintf( temp, sizeof( temp ), "%llu kB / %llu kB", used, total );
    ui_storagelabel = create_label(ui_storageScreen, 0, temp, 40, 100, 14);

    lv_obj_t * ui_storagebar;
    ui_storagebar = lv_bar_create(ui_storageScreen);
    lv_obj_set_size(ui_storagebar, 280, 20);
    lv_obj_align(ui_storagebar,LV_ALIGN_TOP_LEFT,20,120);
    int32_t val = map(used, 0, total, 0, 100);
    lv_bar_set_value(ui_storagebar, val, LV_ANIM_OFF);
    lv_obj_set_style_radius(ui_storagebar, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui_storagebar, 5, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    
    ui_storagelabel = create_label(ui_storageScreen, 0, "SD Card", 20, 170, 20);
    uint64_t cardSize = SD.cardSize() / (1024 * 1024);
    uint64_t usedSize = SD.usedBytes() / (1024 * 1024);
    lv_snprintf( temp, sizeof( temp ), "%llu MB / %llu MB", usedSize, cardSize );
    ui_storagelabel = create_label(ui_storageScreen, 0, temp, 40, 200, 14);
    
    ui_storagebar = lv_bar_create(ui_storageScreen);
    lv_obj_set_size(ui_storagebar, 280, 20);
    lv_obj_align(ui_storagebar,LV_ALIGN_TOP_LEFT,20,220);
    int32_t valsd = map(usedSize, 0, cardSize, 0, 100);
    lv_bar_set_value(ui_storagebar, valsd, LV_ANIM_OFF);
    lv_obj_set_style_radius(ui_storagebar, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui_storagebar, 5, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    
    lv_obj_t * label;

    ui_reboot_btn = lv_btn_create(ui_storageScreen);
    lv_obj_add_event_cb(ui_reboot_btn, reboot_utilities_event_cb, LV_EVENT_ALL, NULL);
    lv_obj_align(ui_reboot_btn, LV_ALIGN_TOP_LEFT, 20, 270);
    lv_obj_set_size(ui_reboot_btn, 160, 30);
    label = lv_label_create(ui_reboot_btn);
    lv_label_set_text(label, "REBOOT");
    lv_obj_center(label);
    

    ui_format_spiffs_btn = lv_btn_create(ui_storageScreen);
    lv_obj_add_event_cb(ui_format_spiffs_btn, SpiffsWarningBox_event_handler, LV_EVENT_ALL, NULL);
    lv_obj_align(ui_format_spiffs_btn, LV_ALIGN_TOP_LEFT, 20, 320);
    lv_obj_set_size(ui_format_spiffs_btn, 160, 30);
    label = lv_label_create(ui_format_spiffs_btn);
    lv_label_set_text(label, "RESET LITTLEFS");
    lv_obj_center(label);
    
    SpinBoxPanel= lv_obj_create(ui_storageScreen);
    // lv_obj_set_style_bg_color(SpinBoxPanel, lv_color_hex(0x202020), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(SpinBoxPanel, LV_OPA_80, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_align( SpinBoxPanel, LV_ALIGN_CENTER);
    lv_obj_set_size( SpinBoxPanel, 320, 450);
    lv_obj_add_flag(SpinBoxPanel, LV_OBJ_FLAG_HIDDEN);
    
    // char temp[78]=""; //65 characters - 2 (%d) + 8 (16mb in bytes=16777216) + 1 = 72 characters
    snprintf( temp, sizeof( temp ), "Confirm reformat of LittleFS,\n and reset settings?\n(Used bytes: %d)", LittleFS.usedBytes() );

    SpiffsWarningBox = create_mbox(SpinBoxPanel, "RESET LittleFS:", 0, 100, 280, 240);
    // lv_obj_add_flag(SpiffsWarningBox, LV_OBJ_FLAG_HIDDEN);
    label = lv_label_create(SpiffsWarningBox);
    lv_label_set_text(label, temp);
    lv_obj_set_style_text_align(label, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_center(label);
    SpiffsWarningBoxOK = create_mboxbtn(SpiffsWarningBox, SpiffsWarningBox_event_handler, true);
    SpiffsWarningBoxCL = create_mboxbtn(SpiffsWarningBox, SpiffsWarningBox_event_handler, false);

    // ui_reboot_btn  = create_btn_header(ui_storageScreen, LV_SYMBOL_LEFT, 0, 0, 70, 50, reboot_utilities_event_cb , ui_storageScreen);


}

//********************************SPIFFS stuff

static void SpiffsWarningBox_event_handler( lv_event_t *e ){
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * obj = lv_event_get_target(e);
    if (code == LV_EVENT_CLICKED){
        if (obj == ui_format_spiffs_btn){
            lv_obj_clear_flag(SpinBoxPanel, LV_OBJ_FLAG_HIDDEN);
        }
        if (obj == SpiffsWarningBoxOK){
            format_SPIFFS();
            lv_obj_add_flag(SpinBoxPanel, LV_OBJ_FLAG_HIDDEN);
        }
        if (obj == SpiffsWarningBoxCL){
            lv_obj_add_flag(SpinBoxPanel, LV_OBJ_FLAG_HIDDEN);
        }
    }
}

static void format_SPIFFS(void){
    log_i("LittleFS Format by User");
    // motor_vibe(20);
    delay(20);
    LittleFS.end();
    log_i("LittleFS unmounted!");
    delay(100);
    LittleFS.format();
    log_i("LittleFS format complete!");
    // motor_vibe(20);
    delay(100);
    bool newmount_attempt = LittleFS.begin();
    if (!newmount_attempt){
        log_e("LittleFS New Mount failed, rebooting");
        delay(1000);
        ESP.restart();
    }
}
//********************************Power stuff
static void reboot_utilities_event_cb( lv_event_t *e ) {
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj = lv_event_get_target(e);
    switch( code ) {
        case( LV_EVENT_CLICKED ):       
            log_i("System reboot by user");
            // motor_vibe(20);
            delay(20);
            display_standby();
            LittleFS.end();
            log_i("LittleFS unmounted!");
            delay(500);
            ESP.restart();
            break;
    }
}

