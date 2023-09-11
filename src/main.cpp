#define TWATCH_USE_PSRAM_ALLOC_LVGL

#include <Arduino.h>
#include "config.h"

#include <lvgl.h>
#include "ui/ui.h"

// #include <ESP32Time.h>
#include <WiFi.h>

#include "Audio.h"

#include <lgfx.h>
LGFX tft;


#include "littleFS.h"

#include "hardware/hardware.h"
#include "hardware/wifiset.h"
#include "hardware/displayset.h"
#include "hardware/powerset.h"
#include "hardware/touchset.h"
// Audio audio;

// bool playing;

// Display callback to flush the buffer to screen
void display_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p){
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);

    tft.startWrite();
    tft.setAddrWindow(area->x1, area->y1, w, h);
    tft.pushPixels((uint16_t *)&color_p->full, w * h, true);
    tft.endWrite();

    lv_disp_flush_ready(disp);
}

// Touchpad callback to read the touchpad
void touchpad_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data){
    uint16_t touchX, touchY;
    
    bool touched = tft.getTouch(&touchX, &touchY);

    if (!touched){
        data->state = LV_INDEV_STATE_REL;
    }else{
        data->state = LV_INDEV_STATE_PR;
        data->point.x = touchX;
        data->point.y = touchY;
    }
}

void init_WT32( void ){
}

void setup() {
  Serial.begin(115200);

  LittleFS.begin(FORMAT_SPIFFS_IF_FAILED);

  tft.init();        // Initialize LovyanGFX
  tft.setRotation(2);
  tft.setBrightness(10);
  tft.fillScreen(TFT_RED);

  display_setup();

  //AUDIO
  // audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
  //audio.setVolume(10); // 0...21
  // audio.forceMono(true);

 

  lv_init();         // Initialize lvgl

  if (!disp_draw_buf) {
    log_i("LVGL disp_draw_buf allocate failed!");
  } else {
    log_i("Display buffer size: ");

    /* LVGL : Setting up buffer to use for display ( disp_draw_buf2 )*/
    lv_disp_draw_buf_init(&draw_buf, disp_draw_buf, disp_draw_buf2, screenWidth * SCR);

    /*** LVGL : Setup & Initialize the display device driver ***/
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = screenWidth;
    disp_drv.ver_res = screenHeight;
    disp_drv.flush_cb = display_flush;
    disp_drv.draw_buf = &draw_buf;
    lv_disp_drv_register(&disp_drv);

    //*** LVGL : Setup & Initialize the input device driver ***
    static lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = touchpad_read;
    lv_indev_drv_register(&indev_drv);  
  }
  
  hardware_setup();
  
  uint16_t currentMillis = millis();
  while (currentMillis + 500 > millis()){
    lv_timer_handler();
  }

  ui_init();
  openStart();

  currentMillis = millis();
  while (currentMillis + 500 > millis()){
    lv_timer_handler();
  }

  if(wifictl_get_autoon()) wifictl_on();
// const char* ssid = "Mi Note 10";
// const char* password = "da6a527f264c";
display_set_brightness((int)display_get_display_brig());

}

void loop() {
  // lv_timer_handler(); /* let the GUI do its work */
  
  power_loop();

}