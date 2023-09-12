#include <Arduino.h>
#include <string.h>
#include "config.h"
#include "hardware/displayset.h"

#include "ui/ui.h"
#include "ui/setup/setup.h"
#include "ui/setup/setup_start/start.h"
// #include "hardware/json_psram_allocator.h"
// #include "hardware/alloc.h"
// #include "littleFS.h"

// LV_IMG_DECLARE( bg1_320x480px );
// const lv_img_dsc_t bg1 = bg1_320x480px;

// start_config_t start_config;


lv_obj_t * ui_StartScreen;
lv_obj_t * ui_startbtn;
lv_obj_t * ui_backgrobj;
lv_obj_t * ui_backgrtileView;
lv_obj_t * ui_backgrimg_bin = NULL;
lv_obj_t * ui_backgrobj2[4];
lv_obj_t * cont2;
lv_obj_t * ui_backgrlabel;
lv_obj_t * ui_backgrslider;
lv_obj_t * ui_backgrddd;

static void ui_StartScreen_event(lv_event_t *e){
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj = lv_event_get_target(e);
    lv_disp_t *display = lv_disp_get_default();
    lv_obj_t *actScr = lv_disp_get_scr_act(display);

    if (code == LV_EVENT_CLICKED){
        
    }
}

static void ui_startSlider_event(lv_event_t *e){
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj = lv_event_get_target(e);
    
    if (code == LV_EVENT_VALUE_CHANGED){
        uint16_t data = lv_dropdown_get_selected(obj);
        display_set_display_bg( data );
    }
}

void openAppStart(){

    // start_load_config();

    ui_StartScreen = create_header(ui_app_settingsScreen(), "Start");
    ui_startbtn = create_btn_header(ui_StartScreen, LV_SYMBOL_LEFT, 0, 0, 70, 50, event_setup_back, ui_StartScreen);



    ui_backgrobj = lv_obj_create(ui_StartScreen);
    lv_obj_set_width(ui_backgrobj, 320/3);  /// 1
    lv_obj_set_height(ui_backgrobj, 480/3); /// 1
    lv_obj_set_x(ui_backgrobj, 20);
    lv_obj_set_y(ui_backgrobj, 80);
    lv_obj_set_align(ui_backgrobj, LV_ALIGN_TOP_LEFT);
    lv_obj_set_style_radius(ui_backgrobj, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
    // lv_obj_set_style_bg_color(obj, color, LV_PART_MAIN | LV_STATE_DEFAULT);
    // lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_backgrobj, lv_palette_main(LV_PALETTE_GREY), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui_backgrobj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui_backgrobj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui_backgrobj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui_backgrobj, 0, LV_PART_MAIN | LV_STATE_DEFAULT); 
    lv_obj_clear_flag(ui_backgrobj, LV_OBJ_FLAG_SCROLLABLE);

    static lv_coord_t col_dsc[] = {40, 40, LV_GRID_TEMPLATE_LAST};
    static lv_coord_t row_dsc[] = {40, 40, 40, LV_GRID_TEMPLATE_LAST};

    ui_backgrimg_bin = lv_img_create( ui_backgrobj);
    lv_obj_set_width( ui_backgrimg_bin, 320);
    lv_obj_set_height( ui_backgrimg_bin, 480);
    lv_obj_set_align( ui_backgrimg_bin, LV_ALIGN_CENTER );
    // lv_img_set_src( ui_backgrimg_bin, &bg1 );
    lv_img_set_zoom(ui_backgrimg_bin, 256/3);

    cont2 = lv_obj_create(ui_backgrobj);
    lv_obj_set_style_grid_column_dsc_array(cont2, col_dsc, 0);
    lv_obj_set_style_grid_row_dsc_array(cont2, row_dsc, 0);
    lv_obj_set_size(cont2, 320/3, 480/3);
    lv_obj_center(cont2);
    lv_obj_set_layout(cont2, LV_LAYOUT_GRID);
    lv_obj_set_style_bg_opa(cont2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(cont2, 7, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(cont2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(cont2, 7, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(cont2, 0, LV_PART_MAIN | LV_STATE_DEFAULT); 
    lv_obj_set_style_border_width(cont2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_scrollbar_mode(cont2, LV_SCROLLBAR_MODE_OFF);


    ui_backgrobj2[0] = create_obj(cont2, 0, 0);
    ui_backgrobj2[1] = create_obj(cont2, 0, 1);
    ui_backgrobj2[2] = create_obj(cont2, 1, 1);
    ui_backgrobj2[3] = create_obj(cont2, 1, 2);
    set_obj_color_bg();
    // set_opa(start_config.start_trans);
    // start_app_opa(start_config.start_trans);

    ui_backgrlabel = create_label(ui_StartScreen, 0, "Background", 20, 250, 20);
    ui_backgrddd = lv_dropdown_create(ui_StartScreen);
    lv_dropdown_set_options(ui_backgrddd,   "bg1\n" 
                                            "bg2\n"
                                            "bg3\n"
                                            "bg4");
    lv_obj_align(ui_backgrddd, LV_ALIGN_TOP_RIGHT, -20, 250-8);
    lv_obj_set_width(ui_backgrddd, 100);
    lv_obj_set_style_border_color(ui_backgrddd, lv_palette_main(LV_PALETTE_GREY), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_dropdown_set_selected(ui_backgrddd, display_get_display_bg());
    lv_obj_add_event_cb(ui_backgrddd, ui_startSlider_event, LV_EVENT_ALL, NULL);

    ui_backgrlabel = create_label(ui_StartScreen, 0, "Tile transparency", 20, 300, 20);
    ui_backgrslider = create_slider(ui_StartScreen, 20, 350, 280, 10, ui_startSlider_event);
    lv_slider_set_range(ui_backgrslider, 50 , 255);
    // lv_slider_set_value(ui_backgrslider, start_config.start_trans, LV_ANIM_OFF);
}

void set_obj_color_bg(void){
    uint32_t i;
    for(i = 0; i < 4; i++){
        lv_obj_set_style_bg_color(ui_backgrobj2[i], lv_theme_get_color_primary(NULL), 0);
    }
}

lv_obj_t * create_obj(lv_obj_t *parent, int col, int row){
    lv_obj_t * obj = lv_obj_create(parent);
    lv_obj_set_style_radius(obj, 5, 0);
    lv_obj_set_grid_cell(obj, LV_GRID_ALIGN_STRETCH, col, 1, LV_GRID_ALIGN_STRETCH, row, 1);
    // lv_obj_set_style_bg_color(obj, lv_theme_get_color_primary(NULL), 0);
    lv_obj_set_style_border_opa(obj, 0, 0);
    return obj;
}

void set_opa(int value){
    uint32_t i;
    for(i = 0; i < 4; i++){
        lv_obj_set_style_bg_opa(ui_backgrobj2[i], value, 0);
    }
}
