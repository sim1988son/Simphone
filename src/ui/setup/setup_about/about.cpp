#include <Arduino.h>
#include <string.h>
#include "littleFS.h"
#include "config.h"
#include "ui/ui.h"
#include "ui/setup/setup.h"
#include "ui/setup/setup_about/about.h"
#include <WiFi.h>

lv_obj_t * ui_aboutScreen;
lv_obj_t * ui_aboutbtn;
lv_obj_t * ui_aboutText;

void openAppAbout(){
    closeApp();
    AppAbout();
    launchApp("About", true);
}

void AppAbout(){
    lv_obj_t *canvas = app_canvas();
    ui_aboutScreen = create_obj(canvas);

    uint64_t mac = ESP.getEfuseMac();

    String info = "";
    info += "\nName Sketch:";
    info += "\nChip model: ";
    info += "\nChip cores: ";
    info += "\nChip frequency: ";
    info += "\nChip version: ";

    info += "\nRAM size: ";
    info += "\nPSRAM size: ";

    info += "\nFlash size: ";
    info += "\nFlash speed: ";

    info += "\nSDK version: ";
    info += "\nFirmware size: ";
    info += "\nStorage space: ";
    info += "\nMAC address: ";

    info += "\nIP address: ";// + String((WiFi.localIP().toString().c_str()), 0);

    ui_aboutText = lv_label_create(ui_aboutScreen);
    lv_label_set_text(ui_aboutText, info.c_str());
    lv_obj_set_width(ui_aboutText, 140);  /*Set smaller width to make the lines wrap*/
    lv_obj_set_style_text_align(ui_aboutText, LV_TEXT_ALIGN_LEFT, 0);
    lv_obj_align(ui_aboutText, LV_ALIGN_TOP_MID, -60, 60);

    info = "";
    info += "\nWT32_plus "; //+ String(__FILE__);
    info += "\n" + String(ESP.getChipModel());
    info += "\n" + String(ESP.getChipCores());
    info += "\n" + String(ESP.getCpuFreqMHz()) + "Mhz";
    info += "\n" + String(ESP.getChipRevision());

    info += "\n" + String((ESP.getHeapSize() / 1024.0), 0) + "kB";
    info += "\n" + String((ESP.getPsramSize() / (1024.0)), 0) + "kB";

    info += "\n" + String((ESP.getFlashChipSize() / (1024.0)), 0) + "kB";
    info += "\n" + String((ESP.getFlashChipSpeed() / 1000000.0), 0) + "Mhz";

    info += "\n" + String(ESP.getSdkVersion());
    info += "\n" + String((ESP.getSketchSize() / (1024.0)), 0) + "kB";
    info += "\n" + String((LittleFS.totalBytes() / (1024.0)), 0) + "kB";
    info += "\n";

    for (int i = 0; i < 6; i++){
        if (i > 0){
            info += "-";
        }
        info += String(byte(mac >> (i * 8) & 0xFF), HEX);
    }
    info += "\n";// + String((WiFi.localIP().toString().c_str()), 0);
    IPAddress ipa = WiFi.localIP();
    uint8_t IP_array[4]= {ipa[0],ipa[1],ipa[2],ipa[3]};
    String strIP=
        String(IP_array[0])+"."+
        String(IP_array[1])+"."+
        String(IP_array[2])+"."+
        String(IP_array[3]);
    info += strIP;

    ui_aboutText = lv_label_create(ui_aboutScreen);
    lv_label_set_text(ui_aboutText, info.c_str());
    lv_obj_set_width(ui_aboutText, 140);  /*Set smaller width to make the lines wrap*/
    lv_obj_set_style_text_align(ui_aboutText, LV_TEXT_ALIGN_RIGHT, 0);
    lv_obj_align(ui_aboutText, LV_ALIGN_TOP_MID, 60, 60);
}