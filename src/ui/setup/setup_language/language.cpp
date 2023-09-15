#include <Arduino.h>
#include <string.h>
#include "config.h"
#include "ui/ui.h"
#include "ui/setup/setup.h"
#include "ui/setup/setup_language/language.h"



lv_obj_t * ui_languageScreen;
lv_obj_t * ui_languagebtn;


void openAppLanguage(){
    closeApp();
    AppLanguage();
    launchApp("Language", true);
}

void AppLanguage(){
    lv_obj_t *canvas = app_canvas();
    ui_languageScreen = create_obj(canvas);
    // ui_languageScreen = create_header(ui_app_settingsScreen(), "Language");
    // ui_languagebtn = create_btn_header(ui_languageScreen, LV_SYMBOL_LEFT, 0, 0, 70, 50, event_setup_back, ui_languageScreen);


}