#include <Arduino.h>
#include <string.h>
#include "config.h"
#include "ui/ui.h"
#include "ui/setup/setup.h"
#include "ui/setup/setup_update/update.h"



lv_obj_t * ui_updateScreen;
lv_obj_t * ui_updatebtn;

static void ui_updateScreen_event(lv_event_t *e){
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj = lv_event_get_target(e);
    lv_disp_t *display = lv_disp_get_default();
    lv_obj_t *actScr = lv_disp_get_scr_act(display);

    if (code == LV_EVENT_CLICKED){
        
    }
}

void openAppUpdate(){
    closeApp();
    AppUpdate();
    launchApp("Update", true);
}

void AppUpdate(){
    lv_obj_t *canvas = app_canvas();
    ui_updateScreen = create_obj(canvas);
    // ui_updateScreen = create_header(ui_app_settingsScreen(), "Update");
    // ui_updatebtn = create_btn_header(ui_updateScreen, LV_SYMBOL_LEFT, 0, 0, 70, 50, event_setup_back, ui_updateScreen);


}
