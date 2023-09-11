#include <Arduino.h>

#include "main.h"
#include "hardware/sdset.h"
#include "hardware/hardware.h"
#include "callback.h"
#include "json_psram_allocator.h"
#include "alloc.h"
#include "littleFS.h"

#include "SD.h"

EventGroupHandle_t sdset_status = NULL;
portMUX_TYPE DRAM_ATTR sdsetMux = portMUX_INITIALIZER_UNLOCKED;
callback_t * sdset_callback = NULL;

// void sdset_set_event( EventBits_t bits );
// bool sdset_get_event( EventBits_t bits );
// void sdset_clear_event( EventBits_t bits );

void sdset_setup( void ){

    sdset_status = xEventGroupCreate();


    if (!LittleFS.begin(FORMAT_SPIFFS_IF_FAILED)){
        log_e("LittleFS Mount Failed");
        delay(2000);
        ESP.restart();
    }
    log_i("Init LittleFS");
    // SD
    pinMode(SD_CS, OUTPUT);
    digitalWrite(SD_CS, HIGH);
    SPI.begin(SDMMC_CLK, SDMMC_D0, SDMMC_CMD); 
    sdset_set_status(SD.begin(SD_CS));
    if(sdset_get_status()){
        log_i("Init SC CARD");
        sdset_set_event(SDCARD_ON); 
    } else {
        sdset_clear_event(SDCARD_ON);
    }
}

bool sdset_get_status( void ){
    return statusSdCard;
}

void sdset_set_status( bool _status ){
    statusSdCard = _status;
}

void sdset_set_event( EventBits_t bits ) {
    portENTER_CRITICAL(&sdsetMux);
    xEventGroupSetBits( sdset_status, bits );
    portEXIT_CRITICAL(&sdsetMux);
}

void sdset_clear_event( EventBits_t bits ) {
    portENTER_CRITICAL(&sdsetMux);
    xEventGroupClearBits( sdset_status, bits );
    portEXIT_CRITICAL(&sdsetMux);
}

EventBits_t sdset_get_event( EventBits_t bits ) { 
    portENTER_CRITICAL(&sdsetMux);
    EventBits_t temp = xEventGroupGetBits( sdset_status ) & bits;
    portEXIT_CRITICAL(&sdsetMux);
    return( temp );
}

bool sdset_register_cb( EventBits_t event, CALLBACK_FUNC callback_func, const char *id ) {
    if ( sdset_callback == NULL ) {
        sdset_callback = callback_init( "sdset" );
        if ( sdset_callback == NULL ) {
            log_e("sdset callback alloc failed");
            while(true);
        }
    }    
    return( callback_register( sdset_callback, event, callback_func, id ) );
}

bool sdset_send_event_cb( EventBits_t event, void *arg ) {
    return( callback_send( sdset_callback, event, arg ) );
}