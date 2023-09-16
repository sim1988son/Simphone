#include <Arduino.h>
#include <string.h>
#include "config.h"
#include "ui/ui.h"
#include "ui/setup/setup.h"
#include "ui/setup/setup_language/language.h"



lv_obj_t * ui_languageScreen;
lv_obj_t * ui_languageddd;

static void ui_startSlider_event(lv_event_t *e){
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj = lv_event_get_target(e);
    
    if (code == LV_EVENT_VALUE_CHANGED){
        uint16_t data = lv_dropdown_get_selected(obj);
        // display_set_display_bg( data );
    }
}

void openAppLanguage(){
    closeApp();
    AppLanguage();
    launchApp("Language", true);
}

void AppLanguage(){
    lv_obj_t *canvas = app_canvas();
    ui_languageScreen = create_obj(canvas);

    lv_obj_t * ui_languagelabel = create_label(ui_languageScreen, 0, "Language", 20, 100, 20);
    ui_languageddd = lv_dropdown_create(ui_languageScreen);
    lv_dropdown_set_options(ui_languageddd, "Polski\n" 
                                            "Angielski\n"
                                            "Niemiecki");
    lv_obj_align(ui_languageddd, LV_ALIGN_TOP_RIGHT, -20, 100-8);
    lv_obj_set_width(ui_languageddd, 120); //100
    lv_obj_set_style_border_color(ui_languageddd, lv_palette_main(LV_PALETTE_GREY), LV_PART_MAIN | LV_STATE_DEFAULT);
    // lv_dropdown_set_selected(ui_languageddd, display_get_display_bg());
    
}