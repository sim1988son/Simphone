#include <Arduino.h>
#include "config.h"
#include "utils/lang.h"
#include "lang_en.h"

String update_string_lang_en(int _status){
    String lang_settings = "TxT";
    switch (_status)
    {
        case STR_SETTINGS:  lang_settings="Stettings";  return lang_settings; break;
        case STR_SYSTEM:    lang_settings="System";  return lang_settings; break;
        case STR_DISPLAY:   lang_settings="Display";  return lang_settings; break;
        case STR_BATTERY:   lang_settings="Battery";  return lang_settings; break;
        case STR_STORAGE:   lang_settings="Storage";  return lang_settings; break;
        case STR_ABOUT:     lang_settings="About";  return lang_settings; break;
        case STR_WIRELESS:  lang_settings="Net & wireless";  return lang_settings; break;
        case STR_SIM:       lang_settings="Sim";  return lang_settings; break;
        case STR_WIFI:      lang_settings="Wifi";  return lang_settings; break;
        case STR_BLUETOOTH: lang_settings="Blutooth";  return lang_settings; break;
        case STR_PERSONALIZE: lang_settings="Personalize";  return lang_settings; break;
        case STR_STRAT:     lang_settings="Start";  return lang_settings; break;
        case STR_COLORS:    lang_settings="Colors";  return lang_settings; break;
        case STR_SOUND:     lang_settings="Sound";  return lang_settings; break;
        case STR_LOCKSCREEN: lang_settings="Lock screen";  return lang_settings; break;
        case STR_TIMELANG:  lang_settings="Time & lang";  return lang_settings; break;
        case STR_DATETIME:  lang_settings="Date & time";  return lang_settings; break;
        case STR_LANGUAGE:  lang_settings="Language";  return lang_settings; break;
        case STR_UPDATE:    lang_settings="Update";  return lang_settings; break;
        case STR_BRIGHTNESS: lang_settings="Adjust brightness";  return lang_settings; break;
        
    default: return lang_settings; break;
    }
}