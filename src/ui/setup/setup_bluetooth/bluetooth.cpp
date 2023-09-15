#include <Arduino.h>
#include <string.h>
#include "config.h"
#include "ui/ui.h"
#include "ui/setup/setup.h"
#include "ui/setup/setup_bluetooth/bluetooth.h"


lv_obj_t * ui_bluetoothScreen;
lv_obj_t * ui_bluetoothbtn;
lv_obj_t * ui_bllabel;
lv_obj_t * ui_blswitch;

static void ui_bluetoothScreen_event(lv_event_t *e){
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj = lv_event_get_target(e);
    lv_disp_t *display = lv_disp_get_default();
    lv_obj_t *actScr = lv_disp_get_scr_act(display);

    if (code == LV_EVENT_VALUE_CHANGED){
        if (lv_obj_has_state(ui_blswitch, LV_STATE_CHECKED)){
            statusBar_show_icon(ui_bleIcon);

        }else{
            statusBar_hide_icon(ui_bleIcon);

        }
    }
}

void openAppBluetooth(){
    closeApp();
    AppBluetooth();
    launchApp("Bluetoth", true);
}

void AppBluetooth(){
    lv_obj_t *canvas = app_canvas();
    ui_bluetoothScreen = create_obj(canvas);
    // ui_bluetoothScreen = create_header(ui_app_settingsScreen(), "Bluetoth");
    // ui_bluetoothbtn = create_btn_header(ui_bluetoothScreen, LV_SYMBOL_LEFT, 0, 0, 70, 50, event_setup_back, ui_bluetoothScreen);


    ui_bllabel = create_label(ui_bluetoothScreen,0, "Bluetoth" , 20, 100, 20);
    ui_blswitch = create_switch(ui_bluetoothScreen, 100, ui_bluetoothScreen_event);

    
}