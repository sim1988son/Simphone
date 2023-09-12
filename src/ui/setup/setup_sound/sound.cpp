#include <Arduino.h>
#include <string.h>
#include "config.h"
#include "ui/ui.h"
#include "ui/setup/setup.h"
#include "ui/setup/setup_sound/sound.h"



lv_obj_t * ui_soundScreen;
lv_obj_t * ui_soundbtn;

static void ui_soundScreen_event(lv_event_t *e){
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj = lv_event_get_target(e);
    lv_disp_t *display = lv_disp_get_default();
    lv_obj_t *actScr = lv_disp_get_scr_act(display);

    if (code == LV_EVENT_CLICKED){
        
    }
}

void openAppSound(){

    ui_soundScreen = create_header(ui_app_settingsScreen(), "Sound");
    ui_soundbtn = create_btn_header(ui_soundScreen, LV_SYMBOL_LEFT, 0, 0, 70, 50, event_setup_back, ui_soundScreen);


}