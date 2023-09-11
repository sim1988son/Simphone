#ifndef _MAIN_H
#define _MAIN_H

#include <lvgl.h>

#define LVGL_LOOP 1

#define TFT_RST 4
#define TFT_CS -1
#define TFT_BL 45

#define I2C_SDA 6
#define I2C_SCL 5
#define RST_N_PIN -1
#define INT_N_PIN 7

#define MOTOR -1
#define SCR 30

#define WAKE_PIN GPIO_NUM_7

#define MUSIC_PLAYER 1

#define FORMAT_SPIFFS_IF_FAILED true

#define SD_CS 41
#define SDMMC_CMD 40
#define SDMMC_CLK 39
#define SDMMC_D0 38

#define I2S_DOUT 37
#define I2S_BCLK 36
#define I2S_LRC 35

/* Change to your screen resolution */
static const uint32_t screenWidth = 320;
static const uint32_t screenHeight = 480;

// create buffer for display 
static lv_color_t disp_draw_buf[screenWidth * SCR];
static lv_color_t disp_draw_buf2[screenWidth * SCR];
static lv_disp_draw_buf_t draw_buf;
static lv_disp_drv_t disp_drv;

void init_WT32( void );

#endif