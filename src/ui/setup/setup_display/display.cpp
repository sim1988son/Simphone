#include <Arduino.h>
#include "config.h"
#include "hardware/displayset.h"

#include "ui/ui.h"
#include "ui/setup/setup.h"
#include "ui/setup/setup_display/display.h"


lv_obj_t * ui_displayScreen;
lv_obj_t * ui_displaybtn;
lv_obj_t * ui_displaylabel;
lv_obj_t * ui_displayswitch;
lv_obj_t * ui_displayslider;

static void ui_displayScreen_event(lv_event_t *e){
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj = lv_event_get_target(e);
    lv_disp_t *display = lv_disp_get_default();
    lv_obj_t *actScr = lv_disp_get_scr_act(display);

    if (code == LV_EVENT_DELETE){
        display_save_config();
    }
    if (code == LV_EVENT_CLICKED){
        
    }
    if (code == LV_EVENT_VALUE_CHANGED){

    }
}

static void ui_displaySlider_event(lv_event_t *e){
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj = lv_event_get_target(e);
    
    if (code == LV_EVENT_VALUE_CHANGED){
        display_set_display_brig((int)lv_slider_get_value(obj));
        display_set_brightness((int)lv_slider_get_value(obj));
        
    }
}
 
void openAppDisplay(){

    ui_displayScreen = create_header(ui_app_settingsScreen(), "Display");
    lv_obj_add_event_cb(ui_displayScreen, ui_displayScreen_event, LV_EVENT_DELETE, NULL);
    ui_displaybtn = create_btn_header(ui_displayScreen, LV_SYMBOL_LEFT, 0, 0, 70, 50, event_setup_back, ui_displayScreen);

    ui_displaylabel = create_label(ui_displayScreen,0, "Adjust brightness", 20, 100, 20);
    ui_displayslider = create_slider(ui_displayScreen, 20, 150, 280, 10, ui_displaySlider_event);
    lv_slider_set_range(ui_displayslider, DISPLAY_MIN_BRIGHTNESS , DISPLAY_MAX_BRIGHTNESS);
    lv_slider_set_value(ui_displayslider, display_get_display_brig(), LV_ANIM_OFF);

    ui_displaylabel = create_label(ui_displayScreen,0, "Automatically" , 20, 200, 20);
    ui_displayswitch = create_switch(ui_displayScreen, 200, ui_displayScreen_event);
    lv_obj_add_state(ui_displayswitch, LV_STATE_DISABLED);
    
    
}

