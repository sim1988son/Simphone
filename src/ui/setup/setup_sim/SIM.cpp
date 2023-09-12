#include <Arduino.h>
#include <string.h>
#include "config.h"
#include "ui/ui.h"
#include "ui/setup/setup.h"
#include "ui/setup/setup_sim/SIM.h"



lv_obj_t * ui_simScreen;
lv_obj_t * ui_simbtn;
lv_obj_t * ui_simlabel;
lv_obj_t * ui_simswitch;

static void ui_simScreen_event(lv_event_t *e){
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj = lv_event_get_target(e);
    lv_disp_t *display = lv_disp_get_default();
    lv_obj_t *actScr = lv_disp_get_scr_act(display);

    if (code == LV_EVENT_CLICKED){
        
    }
    if (code == LV_EVENT_VALUE_CHANGED){
        if (lv_obj_has_state(ui_simswitch, LV_STATE_CHECKED)){
            //statusBar_show_icon(ui_simIcon);

        }else{
            //statusBar_hide_icon(ui_simIcon);

        }
    }
}

void openAppSIM(){

    ui_simScreen = create_header(ui_app_settingsScreen(), "SIM");
    ui_simbtn = create_btn_header(ui_simScreen, LV_SYMBOL_LEFT, 0, 0, 70, 50, event_setup_back, ui_simScreen);


    ui_simlabel = create_label(ui_simScreen,0, "SIM" , 20, 100, 20);
    ui_simswitch = create_switch(ui_simScreen, 100, ui_simScreen_event);

    
}
