#include <Arduino.h>
#include "config.h"
#include "utils/lang.h"


String str_txt(uint16_t _status){
    String txt_settings="";
    // log_i(%s,txt_settings.c_str());
    switch (get_language())
    {
    case UI_LANG_PL:txt_settings = update_string_lang_pl(_status);
        return txt_settings;// = update_string_lang_pl(_status);
        break;
    // case UI_LANG_DE:
    //     return update_string_lang_de(_status);
    //     break;
    // case UI_LANG_EN:
    //     return update_string_lang_en(_status);
    //     break;
    
    default:
        break;
    }
}

uint16_t get_language(void){
    return UI_LANG_PL;
}