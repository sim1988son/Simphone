#include <Arduino.h>
#include "config.h"
#include "utils/lang.h"
#include "lang_pl.h"


String update_string_lang_pl(uint16_t _status){
    String lang_settings;
    switch (_status)
    {
    case STR_SETTINGS_PL: lang_settings = "Ustawienia";
    return lang_settings; break;

    default:
        break;
    // return lang_settings
    }
}