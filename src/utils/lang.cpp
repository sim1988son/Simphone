#include <Arduino.h>
#include "config.h"
#include "utils/lang.h"
#include "hardware/displayset.h"


String str_txt(int _status){
    String txt_settings = "TXT";
    switch (display_get_display_lang())
    {
        case UI_LANG_PL: txt_settings = update_string_lang_pl(_status); return txt_settings; break;
        case UI_LANG_EN: txt_settings = update_string_lang_en(_status); return txt_settings; break;
        case UI_LANG_DE: txt_settings = update_string_lang_de(_status); return txt_settings; break;
        
        default: return txt_settings; break;
    }
}


uint16_t get_language(){
    return UI_LANG_PL;
}