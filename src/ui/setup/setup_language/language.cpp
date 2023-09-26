#include <Arduino.h>
#include <string.h>
#include "config.h"
#include "utils/lang.h"
#include "ui/ui.h"
#include "ui/setup/setup.h"
#include "ui/setup/setup_language/language.h"
#include "hardware/displayset.h"



lv_obj_t * ui_languageScreen;
lv_obj_t * ui_languageddd;
lv_obj_t * ui_languagelabel;


static void event_ui_language(lv_event_t *e){
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj = lv_event_get_target(e);
    
    if (code == LV_EVENT_VALUE_CHANGED){
        uint16_t data = lv_dropdown_get_selected(obj); 
        // uint16_t data = lv_dropdown_get_option_cnt(obj); 
        log_i("%d",data);
        display_set_display_lang( data );
        lv_label_set_text(ui_languagelabel, str_txt(STR_LANGUAGES).c_str());
        // ui_init();
        display_save_config();
    }
}

void openAppLanguage(){
    closeApp();
    AppLanguage();
    launchApp(str_txt(STR_LANGUAGE).c_str(), true);
}

void AppLanguage(){
    lv_obj_t *canvas = app_canvas();
    ui_languageScreen = create_obj(canvas);

    ui_languagelabel = create_label(ui_languageScreen, 0, str_txt(STR_LANGUAGES).c_str(), 20, 100, 20);
    ui_languageddd = lv_dropdown_create(ui_languageScreen);
    lv_dropdown_set_options(ui_languageddd, str_txt(STR_LANGDROP).c_str());
    lv_obj_align(ui_languageddd, LV_ALIGN_TOP_RIGHT, -20, 100-8);
    lv_obj_set_width(ui_languageddd, 140); //100
    lv_dropdown_set_selected(ui_languageddd,display_get_display_lang());
    lv_obj_set_style_border_color(ui_languageddd, lv_palette_main(LV_PALETTE_GREY), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_event_cb(ui_languageddd, event_ui_language, LV_EVENT_ALL, NULL);
    // lv_dropdown_set_selected(ui_languageddd, display_get_display_bg());
    
}