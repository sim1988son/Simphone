#include <Arduino.h>
#include <string.h>
#include "config.h"
#include "hardware/displayset.h"

#include "ui/ui.h"
#include "ui/setup/setup.h"
#include "ui/setup/setup_color/color.h"
// #include "ui/setup/setup_start/start.h"
// #include "hardware/json_psram_allocator.h"
// #include "hardware/alloc.h"
// #include "littleFS.h"

// colorsetup_config_t colorsetup_config;

lv_obj_t * ui_colorsScreen;
lv_obj_t * ui_colorsbtn;
lv_obj_t * ui_colorslabel;
lv_obj_t * ui_colorobjlighten;
lv_obj_t * ui_colorobjlighten1;
lv_obj_t * ui_colorobjlighten2;
lv_obj_t * ui_colorobjdarken;
lv_obj_t * ui_colorobjdarken1;
lv_obj_t * ui_colorobjdarken2;
lv_obj_t * ui_colorobjcb1;
lv_obj_t * ui_colorobjcb2;
lv_obj_t * ui_colorobj_blue;
lv_obj_t * ui_colorobj_red;
lv_obj_t * ui_colorobj_yelow;
lv_obj_t * ui_colorobj_green;

bool t;

static void ui_colorobjcb_event(lv_event_t *e){
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj = lv_event_get_target(e);

    
    if (code == LV_EVENT_CLICKED){
        if (obj == ui_colorobjlighten){
            display_set_display_darkon(false);
        }
        if (obj == ui_colorobjdarken){
            display_set_display_darkon( true);
        }
        if (obj == ui_colorobj_blue){
            // colorsetup_config.cprimary = 0x005fff;//lv_color_hex(LV_PALETTE_RED);
        }
        if (obj == ui_colorobj_red){
            // colorsetup_config.cprimary = 0xcd0000;//lv_color_hex(LV_PALETTE_RED);
        }
        if (obj == ui_colorobj_yelow){
            // colorsetup_config.cprimary = 0xcdcd00;//lv_color_hex(LV_PALETTE_RED);
        }
        if (obj == ui_colorobj_green){
            // colorsetup_config.cprimary = 0x00cd00;//lv_color_hex(LV_PALETTE_RED);
        }
        // lv_obj_set_style_bg_color(ui_colorobjlighten1, lv_color_hex(colorsetup_config.cprimary), LV_PART_MAIN | LV_STATE_DEFAULT);
        // lv_obj_set_style_bg_color(ui_colorobjdarken1, lv_color_hex(colorsetup_config.cprimary), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_set_theme_color();
        // statusBar_show();
        statusbar_refresh_plus();
        // set_obj_color_bg();
        // start_app_bg_color();
        display_save_config();
    }
}

void openAppColors(){
    closeApp();
    AppColors();
    launchApp("Colors", true);
}

void AppColors(){
    lv_obj_t *canvas = app_canvas();
    ui_colorsScreen = create_obj(canvas);
    // ui_colorsScreen = create_header(ui_app_settingsScreen(), "Colors");
    // ui_colorsbtn = create_btn_header(ui_colorsScreen, LV_SYMBOL_LEFT, 0, 0, 70, 50, event_setup_back, ui_colorsScreen);


    lv_color_t c;
    lv_color_t d;
    c = lv_theme_get_color_primary(NULL);
    d = lv_theme_get_color_secondary(NULL);

    ui_colorobjlighten = create_obj(ui_colorsScreen, -70, 80, 90, 120, lv_color_white());
    lv_obj_add_event_cb(ui_colorobjlighten, ui_colorobjcb_event, LV_EVENT_ALL , NULL);
    ui_colorobjlighten1 = create_obj(ui_colorobjlighten, -10, 65, 60, 20, c);
    ui_colorobjlighten2 = create_obj(ui_colorobjlighten, 10, 90, 60, 20, d);
    ui_colorslabel = create_label(ui_colorsScreen,0, "Light", 60, 210, 20);

    ui_colorobjdarken = create_obj(ui_colorsScreen, 70, 80, 90, 120, lv_color_hex(0x070709));
    lv_obj_add_event_cb(ui_colorobjdarken, ui_colorobjcb_event, LV_EVENT_ALL , NULL);
    ui_colorobjdarken1 = create_obj(ui_colorobjdarken, -10, 65, 60, 20, c);
    ui_colorobjdarken2 = create_obj(ui_colorobjdarken, 10, 90, 60, 20, d);
    ui_colorslabel = create_label(ui_colorsScreen,0, "Dark", 210, 200, 20);

    ui_colorslabel = create_label(ui_colorsScreen,0, "Color primary", 20, 270, 20);
    ui_colorobj_blue = create_obj_color(ui_colorsScreen, 20, 300, 50, 0x005fff, ui_colorobjcb_event);
    ui_colorobj_red = create_obj_color(ui_colorsScreen, 95, 300, 50, 0xcd0000, ui_colorobjcb_event);
    ui_colorobj_yelow = create_obj_color(ui_colorsScreen, 175, 300, 50, 0xcdcd00, ui_colorobjcb_event);
    ui_colorobj_green = create_obj_color(ui_colorsScreen, 250, 300, 50, 0x00cd00, ui_colorobjcb_event);
   

}

void lv_set_theme_color(void){ 
           
        lv_disp_t *display = lv_disp_get_default();
        lv_theme_t *theme = lv_theme_default_init(display, 
                            lv_color_hex(0x055fff), //15171A
                            lv_palette_main(LV_PALETTE_GREY),
                            display_get_display_darkon(), 
                            LV_FONT_DEFAULT);
        lv_disp_set_theme(display, theme);
}
