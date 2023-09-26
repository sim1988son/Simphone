#include <Arduino.h>
#include <lgfx.h>
#include "config.h"
#include "hardware/powerset.h"
#include "hardware/sdset.h"
#include "hardware/displayset.h"
#include "callback.h"
#include "json_psram_allocator.h"
#include "alloc.h"
#include "littleFS.h"

static uint8_t dest_brightness = 0;
static uint8_t brightness = 0;

LGFX disp;

display_config_t display_config;
callback_t *display_callback = NULL;

bool display_power_event_cb( EventBits_t event, void *arg );
bool display_power_loop_cb( EventBits_t event, void *arg );
bool display_send_event_cb( EventBits_t event, void *arg );


void display_setup( void ){

    display_load_config();
    

    // display_set_brightness((int)display_get_display_brig());
    log_i("Init Display %d", (int)display_get_display_brig());

    // log_i("%d",display_get_display_lang());

    power_register_cb( POWERMGM_SILENCE_WAKEUP | POWERMGM_STANDBY | POWERMGM_WAKEUP, display_power_event_cb, "display" );
    power_register_loop_cb( POWERMGM_WAKEUP, display_power_loop_cb, "display loop" );
}

bool display_power_event_cb( EventBits_t event, void *arg ) {
    switch( event ) {
        case POWERMGM_STANDBY:          display_standby();
                                        break;
        case POWERMGM_WAKEUP:           display_wakeup( false );
                                        break;
        case POWERMGM_SILENCE_WAKEUP:   display_wakeup( true );
                                        break;
    }
    return( true );
}

bool display_power_loop_cb( EventBits_t event, void *arg ) {
    display_loop();
    return( true );
}

void display_loop( void ){
    if ( dest_brightness != brightness ) {
        if ( brightness < dest_brightness ) {
            brightness++;
            display_set_brightness( brightness );
        } else {
            brightness--;
            display_set_brightness( brightness );
        }
    }
    if ( display_get_display_times() != DISPLAY_MAX_TIMEOUT ) {
        if ( lv_disp_get_inactive_time(NULL) > ( ( display_get_display_times() * 1000 ) - display_get_display_brig() * 8 ) ) {
            dest_brightness = ( ( display_get_display_times() * 1000 ) - lv_disp_get_inactive_time( NULL ) ) / 8 ;
        } else {
            dest_brightness = display_get_display_brig();
        }
    }
}

bool display_register_cb( EventBits_t event, CALLBACK_FUNC callback_func, const char *id ) {
    if ( display_callback == NULL ) {
        display_callback = callback_init( "display" );
        if ( display_callback == NULL ) {
            log_e("display_callback_callback alloc failed");
            while(true);
        }
    }
    return( callback_register( display_callback, event, callback_func, id ) );
}

bool display_send_event_cb( EventBits_t event, void *arg ) {
    return( callback_send( display_callback, event, arg ) );
}

void display_standby( void ) {
  log_i("go standby");
    disp.setBrightness(0);
    brightness = 0;
    dest_brightness = 0;
}

void display_wakeup( bool silence ) {
  // wakeup without display
  if ( silence ) {
    log_i("go silence wakeup");
    disp.setBrightness(0);
    brightness = 0;
    dest_brightness = 0;
  }
  // wakeup with display
  else {
    log_i("go wakeup");
    disp.setBrightness(0);
    brightness = 0;
    dest_brightness = display_get_display_brig();
  }
}

void display_save_config( void ) {
    fs::File file = LittleFS.open( DISPLAYSETUP_JSON_CONFIG_FILE, FILE_WRITE );
    if (!file) {
        log_e("Can't open file: %s!", DISPLAYSETUP_JSON_CONFIG_FILE );
    }else {
        SpiRamJsonDocument doc( 1000 );
        doc["display_auto"  ] = display_config.display_auto;
        doc["display_brig"  ] = display_config.display_brig;
        doc["display_times" ] = display_config.display_times;
        doc["display_bg"    ] = display_config.display_bg;
        doc["display_look"  ] = display_config.display_look;
        doc["display_darkon"] = display_config.display_darkon;
        doc["display_lang"  ] = display_config.display_lang;
        if ( serializeJsonPretty( doc, file ) == 0) {
            log_e("Failed to write config file");
        }
        doc.clear();
    }
    file.close();
}

void display_load_config( void ) {
    fs::File file = LittleFS.open( DISPLAYSETUP_JSON_CONFIG_FILE, FILE_READ );
    if (!file) {
        log_e("Can't open file: %s!", DISPLAYSETUP_JSON_CONFIG_FILE );
    }else {
        int filesize = file.size();
        SpiRamJsonDocument doc( filesize * 2 );
        DeserializationError error = deserializeJson( doc, file );
        if ( error ) {
            log_e("update check deserializeJson() failed: %s", error.c_str() );
        }
        else {
            display_config.display_auto     = doc["display_auto"]   | true;
            display_config.display_brig     = doc["display_brig"]   | DISPLAY_MAX_BRIGHTNESS / 2;
            display_config.display_times    = doc["display_times"]  | DISPLAY_MIN_TIMEOUT;
            display_config.display_bg       = doc["display_bg"]     | 1;
            display_config.display_look     = doc["display_look"]   | 1;
            display_config.display_darkon   = doc["display_darkon"] | false;
            display_config.display_lang     = doc["display_lang"]   | 0;
        }        
        doc.clear();
    }
    file.close();
}

bool display_get_display_auto( void ) {
    return( display_config.display_auto );
}

void display_set_display_auto( bool display_auto ) {
    display_config.display_auto = display_auto;
}

uint16_t display_get_display_brig( void ) {
    return( display_config.display_brig );
}

void display_set_display_brig( uint16_t display_brig ) {
    display_config.display_brig = display_brig;
    dest_brightness = display_brig;
}

void display_set_brightness(int brig){
  disp.setBrightness(brig);
}

uint16_t display_get_display_times( void ) {
    return( display_config.display_times );
}

void display_set_display_times( uint16_t display_times ) {
    display_config.display_times = display_times;
}

uint16_t display_get_display_bg( void ) {
    return( display_config.display_bg );
}

void display_set_display_bg( uint16_t display_bg ) {
    display_config.display_bg = display_bg;
}

uint16_t display_get_display_look( void ) {
    return( display_config.display_look );
}

void display_set_display_look( uint16_t display_look ) {
    display_config.display_look = display_look;
}

bool display_get_display_darkon( void ){
    return( display_config.display_darkon );
}

void display_set_display_darkon( bool display_darkon ){
    display_config.display_darkon = display_darkon;
}

uint16_t display_get_display_lang(){
    return( display_config.display_lang );
}
    
void display_set_display_lang( uint16_t display_lang ){
    display_config.display_lang = display_lang;
}
    
