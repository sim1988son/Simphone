#include <Arduino.h>
#include <string.h>
#include "littleFS.h"
#include "main.h"
#include "ui/ui.h"
#include "ui/setup/setup.h"
#include "ui/setup/setup_about/about.h"
#include <WiFi.h>

lv_obj_t * ui_aboutScreen;
lv_obj_t * ui_aboutbtn;
lv_obj_t * ui_aboutText;

void openAppAbout(){

    ui_aboutScreen = create_header(ui_app_settingsScreen(), "About"); 
    ui_aboutbtn = create_btn_header(ui_aboutScreen, LV_SYMBOL_LEFT, 0, 00, 70, 50, event_setup_back, ui_aboutScreen);

    uint64_t mac = ESP.getEfuseMac();

    String info = "";
    info += "\nName Sketch: WT32_plus "; //+ String(__FILE__);
    info += "\nChip model: " + String(ESP.getChipModel());
    info += "\nChip cores: " + String(ESP.getChipCores());
    info += "\nChip frequency: " + String(ESP.getCpuFreqMHz()) + "Mhz";
    info += "\nChip version: " + String(ESP.getChipRevision());

    info += "\nRAM size: \t\t\t\t" + String((ESP.getHeapSize() / 1024.0), 0) + "kB";
    info += "\nPSRAM size: \t\t" + String((ESP.getPsramSize() / (1024.0)), 0) + "kB";

    info += "\nFlash size: \t\t\t\t" + String((ESP.getFlashChipSize() / (1024.0)), 0) + "kB";
    info += "\nFlash speed: \t\t" + String((ESP.getFlashChipSpeed() / 1000000.0), 0) + "Mhz";

    info += "\nSDK version: \t\t" + String(ESP.getSdkVersion());
    info += "\nFirmware size: \t" + String((ESP.getSketchSize() / (1024.0)), 0) + "kB";
    info += "\nStorage space: \t" + String((LittleFS.totalBytes() / (1024.0)), 0) + "kB";
    info += "\nMAC address: \t";

    for (int i = 0; i < 6; i++){
        if (i > 0){
            info += "-";
        }
        info += String(byte(mac >> (i * 8) & 0xFF), HEX);
    }
    info += "\nIP address: \t\t\t";// + String((WiFi.localIP().toString().c_str()), 0);
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
    lv_obj_set_width(ui_aboutText, 280);  /*Set smaller width to make the lines wrap*/
    lv_obj_set_style_text_align(ui_aboutText, LV_TEXT_ALIGN_LEFT, 0);
    lv_obj_align(ui_aboutText, LV_ALIGN_TOP_MID, 0, 60);
}