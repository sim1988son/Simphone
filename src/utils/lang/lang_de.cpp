#include <Arduino.h>
#include "config.h"
#include "utils/lang.h"
#include "lang_de.h"


String update_string_lang_de(int _status){
    String lang_settings = "TxT";
    switch (_status)
    {
        case STR_SETTINGS:  lang_settings="Ustawienia";  return lang_settings; break;
        case STR_SYSTEM:    lang_settings="System";  return lang_settings; break;
        case STR_DISPLAY:   lang_settings="Wyswietlacz";  return lang_settings; break;
        case STR_BATTERY:   lang_settings="Bateria";  return lang_settings; break;
        case STR_STORAGE:   lang_settings="Pamiec";  return lang_settings; break;
        case STR_ABOUT:     lang_settings="Informacje";  return lang_settings; break;
        case STR_WIRELESS:  lang_settings="Siec i internet";  return lang_settings; break;
        case STR_SIM:       lang_settings="Sim";  return lang_settings; break;
        case STR_WIFI:      lang_settings="Wifi";  return lang_settings; break;
        case STR_BLUETOOTH: lang_settings="Blutooth";  return lang_settings; break;
        case STR_PERSONALIZE: lang_settings="Personalizacja";  return lang_settings; break;
        case STR_STRAT:     lang_settings="Tlo";  return lang_settings; break;
        case STR_COLORS:    lang_settings="Kolory";  return lang_settings; break;
        case STR_SOUND:     lang_settings="Dzwieki";  return lang_settings; break;
        case STR_LOCKSCREEN: lang_settings="Ekran blokady";  return lang_settings; break;
        case STR_TIMELANG:  lang_settings="Czas i jezyk";  return lang_settings; break;
        case STR_DATETIME:  lang_settings="Data i czas";  return lang_settings; break;
        case STR_LANGUAGE:  lang_settings="Jezyk";  return lang_settings; break;
        case STR_UPDATE:    lang_settings="Aktualizacja";  return lang_settings; break;
        
    default: return lang_settings; break;
    }
}