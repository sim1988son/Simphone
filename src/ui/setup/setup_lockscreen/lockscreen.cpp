#include <Arduino.h>
#include <string.h>
#include "config.h"
#include "hardware/displayset.h"
#include "utils/lang.h"
#include "ui/ui.h"
#include "ui/setup/setup.h"
#include "ui/setup/setup_lockscreen/lockscreen.h"

// LV_IMG_DECLARE( ui_img_wallpaper_png );
// const lv_img_dsc_t lock1 = ui_img_wallpaper_png;


lv_obj_t * ui_lockScreen;
lv_obj_t * ui_lockbtn;
lv_obj_t * ui_locklabel;
lv_obj_t * ui_lockddbg;
lv_obj_t * ui_lockbgobj;
lv_obj_t * ui_lockimg_bin;
lv_obj_t * ui_lockslider;
lv_obj_t * ui_lockliderlabel;

static void ui_lockScreen_event(lv_event_t *e){
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj = lv_event_get_target(e);

    if (code == LV_EVENT_DELETE){
        display_save_config();
    }
    if (code == LV_EVENT_CLICKED){
        
    }
}

static void event_ui_lockddbg(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * obj = lv_event_get_target(e);
    if(code == LV_EVENT_VALUE_CHANGED) {

        uint16_t data = lv_dropdown_get_selected(obj);
        display_set_display_look( data );

    }

}

static void ui_lockslider_event(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * obj = lv_event_get_target(e);
    if(code == LV_EVENT_VALUE_CHANGED) {
        display_set_display_times( lv_slider_get_value( obj ) );
        lockscreen_set_slider_time();
        // char temp[16]="";
        // if ( lv_slider_get_value( ui_lockslider ) == DISPLAY_MAX_TIMEOUT ) {
        //     snprintf( temp, sizeof( temp ), "newer" );
        // } else {
        //     snprintf( temp, sizeof( temp ), "%d sec", lv_slider_get_value( ui_lockslider ) );
        // }
        // lv_label_set_text( ui_lockliderlabel, temp );
    }
}

void openAppLockScreen(){
    closeApp();
    AppLockScreen();
    launchApp(str_txt(STR_LOCKSCREEN).c_str(), true);
}

void AppLockScreen(){
    lv_obj_t *canvas = app_canvas();
    ui_lockScreen = create_obj(canvas);
    lv_obj_add_event_cb(ui_lockScreen, ui_lockScreen_event, LV_EVENT_DELETE, NULL);
    
    
    ui_lockbgobj = lv_obj_create(ui_lockScreen);
    lv_obj_set_width(ui_lockbgobj, 320/3);  /// 1
    lv_obj_set_height(ui_lockbgobj, 480/3); /// 1
    lv_obj_set_x(ui_lockbgobj, 20);
    lv_obj_set_y(ui_lockbgobj, 80);
    lv_obj_set_align(ui_lockbgobj, LV_ALIGN_TOP_LEFT);
    lv_obj_set_style_radius(ui_lockbgobj, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
    // lv_obj_set_style_bg_color(obj, color, LV_PART_MAIN | LV_STATE_DEFAULT);
    // lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_lockbgobj, lv_palette_main(LV_PALETTE_GREY), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui_lockbgobj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui_lockbgobj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui_lockbgobj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui_lockbgobj, 0, LV_PART_MAIN | LV_STATE_DEFAULT); 
    lv_obj_clear_flag(ui_lockbgobj, LV_OBJ_FLAG_SCROLLABLE);

    ui_lockimg_bin = lv_img_create( ui_lockbgobj);
    lv_obj_set_width( ui_lockimg_bin, 320);
    lv_obj_set_height( ui_lockimg_bin, 480);
    lv_obj_set_align( ui_lockimg_bin, LV_ALIGN_CENTER );
    // lv_img_set_src( ui_lockimg_bin, &lock1 );
    lv_img_set_zoom(ui_lockimg_bin, 256/3);

    ui_locklabel = create_label(ui_lockScreen, 0, str_txt(STR_BACKGROUND).c_str(), 20, 250, 20);
    ui_lockddbg = lv_dropdown_create(ui_lockScreen);
    lv_dropdown_set_options(ui_lockddbg, "lock1\n" 
                                        "lock2\n"
                                        "lock3\n"
                                        "lock4");
    lv_obj_align(ui_lockddbg, LV_ALIGN_TOP_RIGHT, -20, 250-8);
    lv_dropdown_set_selected(ui_lockddbg, display_get_display_look());
    lv_obj_set_width(ui_lockddbg, 100);
    lv_obj_set_style_border_color(ui_lockddbg, lv_palette_main(LV_PALETTE_GREY), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_event_cb(ui_lockddbg, event_ui_lockddbg, LV_EVENT_ALL, NULL);


    ui_locklabel = create_label(ui_lockScreen, 0, str_txt(STR_SCREENTIMEOUT).c_str(), 20, 300, 20);
    ui_lockliderlabel = create_label(ui_lockScreen, 3, " ", -20, 300, 16);
    
    ui_lockslider = create_slider(ui_lockScreen, 20, 350, 280, 10, ui_lockslider_event);
    lv_slider_set_range(ui_lockslider, DISPLAY_MIN_TIMEOUT , DISPLAY_MAX_TIMEOUT);
    lv_slider_set_value(ui_lockslider, display_get_display_times(), LV_ANIM_OFF);

    lockscreen_set_slider_time();
    // char temp[16]="";
    // if ( lv_slider_get_value( ui_lockslider ) == DISPLAY_MAX_TIMEOUT ) {
    //     snprintf( temp, sizeof( temp ), str_txt(STR_TIMENEVER).c_str() );
    // } else {
    //     snprintf( temp, sizeof( temp ), "%d sec", lv_slider_get_value( ui_lockslider ) );
    // }
    // lv_label_set_text( ui_lockliderlabel, temp );

}

void lockscreen_set_slider_time(){
    char temp[16]="";
    if ( lv_slider_get_value( ui_lockslider ) == DISPLAY_MAX_TIMEOUT ) {
        snprintf( temp, sizeof( temp ), str_txt(STR_TIMENEVER).c_str() );
    } else {
        snprintf( temp, sizeof( temp ), "%d sec", lv_slider_get_value( ui_lockslider ) );
    }
    lv_label_set_text( ui_lockliderlabel, temp );
}