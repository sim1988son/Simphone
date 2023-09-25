#include <Arduino.h>
#include "config.h"
#include "utils/lang.h"
#include "lang_pl.h"


String update_string_lang_pl(int _status){
    String lang_settings = "TxT";
    switch (_status)
    {
        case STR_SETTINGS:      lang_settings="Ustawienia";  return lang_settings; break;
        case STR_SYSTEM:        lang_settings="System";  return lang_settings; break;
        case STR_DISPLAY:       lang_settings="Wyswietlacz";  return lang_settings; break;
        case STR_BATTERY:       lang_settings="Bateria";  return lang_settings; break;
        case STR_STORAGE:       lang_settings="Pamiec";  return lang_settings; break;
        case STR_ABOUT:         lang_settings="Informacje";  return lang_settings; break;
        case STR_WIRELESS:      lang_settings="Siec i internet";  return lang_settings; break;
        case STR_SIM:           lang_settings="Sim";  return lang_settings; break;
        case STR_WIFI:          lang_settings="Wifi";  return lang_settings; break;
        case STR_BLUETOOTH:     lang_settings="Blutooth";  return lang_settings; break;
        case STR_PERSONALIZE:   lang_settings="Personalizacja";  return lang_settings; break;
        case STR_STRAT:         lang_settings="Tlo";  return lang_settings; break;
        case STR_COLORS:        lang_settings="Kolory";  return lang_settings; break;
        case STR_SOUND:         lang_settings="Dzwieki";  return lang_settings; break;
        case STR_LOCKSCREEN:    lang_settings="Ekran blokady";  return lang_settings; break;
        case STR_TIMELANG:      lang_settings="Czas i jezyk";  return lang_settings; break;
        case STR_DATETIME:      lang_settings="Data i czas";  return lang_settings; break;
        case STR_LANGUAGE:      lang_settings="Jezyk";  return lang_settings; break;
        case STR_UPDATE:        lang_settings="Aktualizacja";  return lang_settings; break;
        case STR_BRIGHTNESS:    lang_settings="Regulacja jasnosci";  return lang_settings; break;
        case STR_AUTOMATIC:     lang_settings="Automatycznie";  return lang_settings; break;
        case STR_BATTSAVER:     lang_settings="Oszczedzanie bateri";  return lang_settings; break;
        case STR_STORAGEINTERNAL: lang_settings="Pamiec wewnetrzna";  return lang_settings; break;
        case STR_MEMORYCARD:    lang_settings="Karta pamieci";  return lang_settings; break;
        case STR_BACKGROUND:    lang_settings="Tlo";  return lang_settings; break;
        case STR_TILETRANSPA:   lang_settings="Kafelek przezroczystosc";  return lang_settings; break;
        case STR_SYSTEMTHEME:   lang_settings="Motyw systemu";  return lang_settings; break;
        case STR_COLORPRIMARY:  lang_settings="Color wiodacy";  return lang_settings; break;
        case STR_COLORLIGHT:    lang_settings="Jasny";  return lang_settings; break;
        case STR_COLORDARK:     lang_settings="Ciemny";  return lang_settings; break;
        case STR_SCREENTIMEOUT: lang_settings="Wygaszanie ekranu";  return lang_settings; break;
        case STR_TIMENEVER:     lang_settings="nigdy";  return lang_settings; break;
        case STR_LANGUAGES:     lang_settings="Jezyki";  return lang_settings; break;
        case STR_LANGDROP:      lang_settings="polski\nangielski\nniemiecki";  return lang_settings; break;
        case STR_ENGLISH:       lang_settings="angielsk\n";  return lang_settings; break;
        case STR_GERMAN:        lang_settings="niemiecki\n";  return lang_settings; break;
        
    default: return lang_settings; break;
    }
}