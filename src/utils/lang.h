#ifndef _UI_LANG_H
#define _UI_LANG_H

#include <Arduino.h>
// #include "config.h"
#include "lang/lang_pl.h"
// #include "lang/lang_en.h"
// #include "lang/lang_de.h"
// #define NUMLANG 3

typedef enum {
        UI_LANG_PL = 0,
        UI_LANG_DE ,
        UI_LANG_EN,

    } status;

typedef enum {
        STR_SETTINGS = 0,
        STR_WIFI,

    } txt_string;



// void update_string_lang(uint16_t _status){
//     switch (_status)
//     {
//     case UI_LANG_PL:
//         update_string_lang_pl();
//         break;
//     case UI_LANG_DE:
//         update_string_lang_de();
//         break;
//     case UI_LANG_EN:
//         update_string_lang_en();
//         break;
    
//     default:
//         break;
//     }
//     txt_settings = lang_settings;
// }


uint16_t get_language(void);

String str_txt(uint16_t _status);

#endif