#ifndef _UI_LANG_H
#define _UI_LANG_H

#include <Arduino.h>
// #include "config.h"
#include "lang/lang_pl.h"
#include "lang/lang_en.h"
#include "lang/lang_de.h"
// #define NUMLANG 3

enum 
{
    UI_LANG_PL = 0,
    UI_LANG_EN,
    UI_LANG_DE
};

enum 
{
    STR_SETTINGS = 0,
    STR_SYSTEM,
    STR_DISPLAY,
    STR_BATTERY,
    STR_STORAGE,
    STR_ABOUT,
    STR_WIRELESS,
    STR_SIM,
    STR_WIFI,
    STR_BLUETOOTH,
    STR_PERSONALIZE,
    STR_STRAT,
    STR_COLORS,
    STR_SOUND,
    STR_LOCKSCREEN,
    STR_TIMELANG,
    STR_DATETIME,
    STR_LANGUAGE,
    STR_UPDATE,
    STR_BRIGHTNESS,
    STR_AUTOMATIC,
    STR_BATTSAVER,
    STR_STORAGEINTERNAL,
    STR_MEMORYCARD,
    STR_BACKGROUND,
    STR_TILETRANSPA,
    STR_SYSTEMTHEME,
    STR_COLORPRIMARY,
    STR_COLORLIGHT,
    STR_COLORDARK,
    STR_SCREENTIMEOUT,
    STR_TIMENEVER,
    STR_LANGUAGES,
    STR_LANGDROP
    
};


uint16_t get_language();

String str_txt(int _status);

#endif