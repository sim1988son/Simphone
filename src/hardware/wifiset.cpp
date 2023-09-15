#include <Arduino.h>
#include "config.h"
#include <WiFi.h>
#include <esp_wifi.h>
#include <esp_wps.h>
#include "hardware/powerset.h"
#include "hardware/sdset.h"
#include "hardware/wifiset.h"
#include "callback.h"
#include "json_psram_allocator.h"
#include "alloc.h"
#include "littleFS.h"

bool wifi_init = false;

EventGroupHandle_t wifictl_status = NULL;
portMUX_TYPE DRAM_ATTR wifictlMux = portMUX_INITIALIZER_UNLOCKED;
callback_t *wifictl_callback = NULL;

// void wifictl_send_event_cb( EventBits_t event, char *msg );
bool wifictl_power_event_cb( EventBits_t event, void *arg );

// void wifictl_StartTask( void );
void wifictl_Task( void * pvParameters );
TaskHandle_t _wifictl_Task;

char * wifiname=NULL;
char * wifipassword=NULL;


static networklist * wifiset_networklist = NULL;
wifiset_config_t wifiset_config;


bool wifictl_send_event_cb( EventBits_t event, void *arg );
// void wifictl_set_event( EventBits_t bits );
// bool wifictl_get_event( EventBits_t bits );
// void wifictl_clear_event( EventBits_t bits );
// void wifictl_save_network( void );
// void wifictl_load_network( void );
void wifiset_save_config( void );
void wifiset_load_config( void );
void wifictl_Task( void * pvParameters );

void wifiset_setup(void){
    if ( wifi_init == true )
        return;

    wifictl_status = xEventGroupCreate();

    wifi_init = true;
    
    wifiset_networklist = (networklist*)CALLOC( sizeof( networklist ) * NETWORKLIST_ENTRYS, 1 );

    if( !wifiset_networklist ) {
      log_e("wifiset_networklist calloc faild");
      while(true);
    }

    // clean network list table
    for ( int entry = 0 ; entry < NETWORKLIST_ENTRYS ; entry++ ) {
      wifiset_networklist[ entry ].ssid[ 0 ] = '\0';
      wifiset_networklist[ entry ].password[ 0 ] = '\0';
    }

    // load config from spiff
    wifiset_load_config();

    esp_wifi_set_bandwidth( WIFI_IF_STA, WIFI_BW_HT20 );

    // register WiFi events
    WiFi.onEvent([](WiFiEvent_t event, WiFiEventInfo_t info) {
        wifictl_set_event( WIFICTL_ACTIVE );
        wifictl_clear_event( WIFICTL_OFF_REQUEST | WIFICTL_ON_REQUEST | WIFICTL_SCAN | WIFICTL_CONNECT );
        if ( wifictl_get_event( WIFICTL_WPS_REQUEST ) )
          wifictl_send_event_cb( WIFICTL_DISCONNECT, (void *)"wait for WPS" );
        else {
          wifictl_set_event( WIFICTL_SCAN );
          wifictl_send_event_cb( WIFICTL_DISCONNECT, (void *)"scan ..." );
          WiFi.scanNetworks();
        }
    }, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_DISCONNECTED);// SYSTEM_EVENT_STA_DISCONNECTED);

    WiFi.onEvent([](WiFiEvent_t event, WiFiEventInfo_t info) {
        wifictl_set_event( WIFICTL_ACTIVE );
        wifictl_clear_event( WIFICTL_OFF_REQUEST | WIFICTL_ON_REQUEST | WIFICTL_SCAN | WIFICTL_CONNECT | WIFICTL_WPS_REQUEST );
        int len = WiFi.scanComplete();
        for( int i = 0 ; i < len ; i++ ) {
          for ( int entry = 0 ; entry < NETWORKLIST_ENTRYS ; entry++ ) {
            if ( !strcmp( wifiset_networklist[ entry ].ssid,  WiFi.SSID(i).c_str() ) ) {
              wifiname = wifiset_networklist[ entry ].ssid;
              wifipassword = wifiset_networklist[ entry ].password;
              wifictl_send_event_cb( WIFICTL_SCAN, (void *)"connecting ..." );
              WiFi.begin( wifiname, wifipassword );
              return;
            }
          }
        }
        wifictl_send_event_cb( WIFICTL_SCAN, (void *)"scan done" );
    }, WiFiEvent_t::ARDUINO_EVENT_WIFI_SCAN_DONE);// SYSTEM_EVENT_SCAN_DONE );

    WiFi.onEvent([](WiFiEvent_t event, WiFiEventInfo_t info) {
        wifictl_set_event( WIFICTL_CONNECT | WIFICTL_ACTIVE );
        if ( wifictl_get_event( WIFICTL_WPS_REQUEST ) ) {
          log_i("store new SSID and psk from WPS");
          wifiset_insert_network( WiFi.SSID().c_str(), WiFi.psk().c_str() );
          wifiset_save_config();
        }
        wifictl_clear_event( WIFICTL_OFF_REQUEST | WIFICTL_ON_REQUEST | WIFICTL_SCAN | WIFICTL_WPS_REQUEST  );
        wifictl_send_event_cb( WIFICTL_CONNECT, (void *)WiFi.SSID().c_str() );
        wifictl_send_event_cb( WIFICTL_CONNECT_IP, (void *)WiFi.localIP().toString().c_str() );
        #ifdef ENABLE_WEBSERVER
        if ( wifictl_config.webserver ) {
          asyncwebserver_start();
        }
        #endif
        #ifdef ENABLE_FTPSERVER
        if ( wifictl_config.ftpserver ) {
          ftpserver_start( wifictl_config.ftpuser , wifictl_config.ftppass );
        }
        # endif
    }, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_GOT_IP);// SYSTEM_EVENT_STA_GOT_IP );

    WiFi.onEvent([](WiFiEvent_t event, WiFiEventInfo_t info) {
        wifictl_set_event( WIFICTL_ACTIVE );
        wifictl_clear_event( WIFICTL_CONNECT | WIFICTL_OFF_REQUEST | WIFICTL_ON_REQUEST );
        if ( wifictl_get_event( WIFICTL_WPS_REQUEST ) )
          wifictl_send_event_cb( WIFICTL_ON, (void *)"wait for WPS" );
        else {
          wifictl_set_event( WIFICTL_SCAN );
          wifictl_send_event_cb( WIFICTL_ON, (void *)"scan ..." );
          WiFi.scanNetworks();
        }
    }, WiFiEvent_t::ARDUINO_EVENT_WIFI_READY);// SYSTEM_EVENT_WIFI_READY );

    WiFi.onEvent([](WiFiEvent_t event, WiFiEventInfo_t info) {
        #ifdef ENABLE_WEBSERVER
        asyncwebserver_end();
        #endif
        wifictl_clear_event( WIFICTL_ACTIVE | WIFICTL_CONNECT | WIFICTL_OFF_REQUEST | WIFICTL_ON_REQUEST | WIFICTL_SCAN | WIFICTL_WPS_REQUEST );
        wifictl_send_event_cb( WIFICTL_OFF, (void *)"" );
    }, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_STOP);// SYSTEM_EVENT_STA_STOP );

    WiFi.onEvent([](WiFiEvent_t event, WiFiEventInfo_t info) {
      esp_wifi_wps_disable();
      WiFi.begin();
      wifictl_send_event_cb( WIFICTL_WPS_SUCCESS, (void *)"wps success" );
    }, WiFiEvent_t::ARDUINO_EVENT_WPS_ER_SUCCESS);// SYSTEM_EVENT_STA_WPS_ER_SUCCESS );

    WiFi.onEvent([](WiFiEvent_t event, WiFiEventInfo_t info) {
      esp_wifi_wps_disable();
      wifictl_send_event_cb( WIFICTL_WPS_SUCCESS, (void *)"wps failed" );
    }, WiFiEvent_t::ARDUINO_EVENT_WPS_ER_FAILED);// SYSTEM_EVENT_STA_WPS_ER_FAILED );

    WiFi.onEvent([](WiFiEvent_t event, WiFiEventInfo_t info) {
      esp_wifi_wps_disable();
      wifictl_send_event_cb( WIFICTL_WPS_SUCCESS, (void *)"wps timeout" );
    }, WiFiEvent_t::ARDUINO_EVENT_WPS_ER_TIMEOUT);// arduino_EVENT_STA_WPS_ER_TIMEOUT );
  
    // log_i("Init wifi");
    xTaskCreatePinnedToCore(  wifictl_Task,     /* Function to implement the task */
                              "wifictl Task",   /* Name of the task */
                              3000,             /* Stack size in words */
                              NULL,             /* Task input parameter */
                              1,                /* Priority of the task */
                              &_wifictl_Task,   /* Task handle. */
                              0 );
    vTaskSuspend( _wifictl_Task );

    power_register_cb( POWERMGM_SILENCE_WAKEUP | POWERMGM_STANDBY | POWERMGM_WAKEUP, wifictl_power_event_cb, "wifictl" );

    wifictl_set_event( WIFICTL_OFF );
    // log_e("timer start wifi");

}


bool wifictl_power_event_cb( EventBits_t event, void *arg ) {
    bool retval = true;
    
    switch( event ) {
        case POWERMGM_STANDBY:          
            if ( !wifiset_config.enable_on_standby || wifictl_get_event( WIFICTL_OFF ) ) {
                wifictl_standby();
                retval = true;
            }
            else {
              log_w("standby blocked by \"enable on standby\" option");
              retval = false;
            }
                                        break;
        case POWERMGM_WAKEUP:           wifictl_wakeup();
                                        retval = true;
                                        break;
        case POWERMGM_SILENCE_WAKEUP:   wifictl_wakeup();
                                        retval = true;
                                        break;
    }
    return( retval );
}

bool wifi_status(){
  return (WiFi.status() == WL_CONNECTED);
}

void wifiset_save_config( void ) {
    fs::File file = LittleFS.open( WIFISETUP_JSON_CONFIG_FILE, FILE_WRITE );

    if (!file) {
        log_e("Can't open file: %s!", WIFISETUP_JSON_CONFIG_FILE );
    }
    else {
        SpiRamJsonDocument doc( 10000 );

        doc["autoon"] = wifiset_config.autoon;
        doc["enable_on_standby"] = wifiset_config.enable_on_standby;
        for ( int i = 0 ; i < NETWORKLIST_ENTRYS ; i++ ) {
            doc["networklist"][ i ]["ssid"] = wifiset_networklist[ i ].ssid;
            doc["networklist"][ i ]["psk"] = wifiset_networklist[ i ].password;
        }

        if ( serializeJsonPretty( doc, file ) == 0) {
            log_e("Failed to write config file");
        }
        doc.clear();
    }
    file.close();
}

void wifiset_load_config( void ) {
    fs::File file = LittleFS.open( WIFISETUP_JSON_CONFIG_FILE, FILE_READ );
    if (!file) {
        log_e("Can't open file: %s!", WIFISETUP_JSON_CONFIG_FILE );
    }
    else {
        int filesize = file.size();
        SpiRamJsonDocument doc( filesize * 2 );

        DeserializationError error = deserializeJson( doc, file );
        if ( error ) {
            log_e("update check deserializeJson() failed: %s", error.c_str() );
        }
        else {
            wifiset_config.autoon = doc["autoon"] | true;
            wifiset_config.enable_on_standby = doc["enable_on_standby"] | false;
            for ( int i = 0 ; i < NETWORKLIST_ENTRYS ; i++ ) {
                if ( doc["networklist"][ i ]["ssid"] && doc["networklist"][ i ]["psk"] ) {
                    strlcpy( wifiset_networklist[ i ].ssid    , doc["networklist"][ i ]["ssid"], sizeof( wifiset_networklist[ i ].ssid ) );
                    strlcpy( wifiset_networklist[ i ].password, doc["networklist"][ i ]["psk"], sizeof( wifiset_networklist[ i ].password ) );
                }
            }
        }        
        doc.clear();
    }
    file.close();
}

bool wifictl_get_autoon( void ) {
  return( wifiset_config.autoon );
}

bool wifictl_get_enable_on_standby( void ) {
  return( wifiset_config.enable_on_standby );
}

void wifictl_set_autoon( bool autoon ) {
  wifiset_config.autoon = autoon;
  wifictl_send_event_cb( WIFICTL_AUTOON, (void*)&autoon );
  wifiset_save_config();
}

void wifictl_set_enable_on_standby( bool enable ) {
  wifiset_config.enable_on_standby = enable;
  wifiset_save_config();
}


void wifictl_set_event( EventBits_t bits ) {
    portENTER_CRITICAL(&wifictlMux);
    xEventGroupSetBits( wifictl_status, bits );
    portEXIT_CRITICAL(&wifictlMux);
}

void wifictl_clear_event( EventBits_t bits ) {
    portENTER_CRITICAL(&wifictlMux);
    xEventGroupClearBits( wifictl_status, bits );
    portEXIT_CRITICAL(&wifictlMux);
}

bool wifictl_get_event( EventBits_t bits ) {
    portENTER_CRITICAL(&wifictlMux);
    EventBits_t temp = xEventGroupGetBits( wifictl_status ) & bits;
    portEXIT_CRITICAL(&wifictlMux);
    if ( temp )
        return( true );

    return( false );
}

bool wifictl_register_cb( EventBits_t event, CALLBACK_FUNC callback_func, const char *id ) {
    if ( wifictl_callback == NULL ) {
        wifictl_callback = callback_init( "wifictl" );
        if ( wifictl_callback == NULL ) {
            log_e("wifictl callback alloc failed");
            while(true);
        }
    }    
    return( callback_register( wifictl_callback, event, callback_func, id ) );
}

bool wifictl_send_event_cb( EventBits_t event, void *arg ) {
    return( callback_send( wifictl_callback, event, arg ) );
}

void wifictl_on( void ) {
  if ( wifi_init == false )
    return;

  log_i("request wifictl on");
  while( wifictl_get_event( WIFICTL_OFF_REQUEST | WIFICTL_ON_REQUEST ) ) { 
    yield();
  }
  wifictl_set_event( WIFICTL_ON_REQUEST | WIFICTL_FIRST_RUN );
  vTaskResume( _wifictl_Task );
}

void wifictl_off( void ) {
  if ( wifi_init == false )
    return;
  
  log_i("request wifictl off");
  while( wifictl_get_event( WIFICTL_OFF_REQUEST | WIFICTL_ON_REQUEST ) ) { 
    yield();
  }

  if ( !wifictl_get_event( WIFICTL_FIRST_RUN ) ) {
    log_i("wifictl not active, prevent first run crash");
    return;
  }

  wifictl_set_event( WIFICTL_OFF_REQUEST );
  vTaskResume( _wifictl_Task );
}

void wifictl_standby( void ) {
  log_i("request wifictl standby");
  wifictl_off();
  while( wifictl_get_event( WIFICTL_ACTIVE | WIFICTL_CONNECT | WIFICTL_OFF_REQUEST | WIFICTL_ON_REQUEST | WIFICTL_SCAN | WIFICTL_WPS_REQUEST ) ) { 
    yield();
  }
  log_i("request wifictl standby done");
}

void wifictl_wakeup( void ) {
  if ( wifiset_config.autoon ) {
    log_i("request wifictl wakeup");
    wifictl_on();
    log_i("request wifictl wakeup done");
  }
}

void wifictl_Task( void * pvParameters ) {
  if ( wifi_init == false )
    return;

  log_i("start wifictl task, heap: %d", ESP.getFreeHeap() );

  while ( true ) {
    vTaskDelay( 500 );
    if ( wifictl_get_event( WIFICTL_OFF_REQUEST ) && wifictl_get_event( WIFICTL_ON_REQUEST ) ) {
      log_w("confused by wifictl on/off at the same time. off request accept");
    }

    if ( wifictl_get_event( WIFICTL_OFF_REQUEST ) ) {
      WiFi.mode( WIFI_OFF );
      esp_wifi_stop();
      log_i("request wifictl off done");
      wifictl_set_event( WIFICTL_OFF );
      wifictl_clear_event( WIFICTL_ON );
    }
    else if ( wifictl_get_event( WIFICTL_ON_REQUEST ) ) {
      esp_wifi_start();
      WiFi.mode( WIFI_STA );
      log_i("request wifictl on done");
      wifictl_set_event( WIFICTL_ON );
      wifictl_clear_event( WIFICTL_OFF );
    }
    wifictl_clear_event( WIFICTL_OFF_REQUEST | WIFICTL_ACTIVE | WIFICTL_CONNECT | WIFICTL_SCAN | WIFICTL_ON_REQUEST );
    vTaskSuspend( _wifictl_Task );
  }
}

bool wifiset_is_known( const char* networkname ) {
  if ( wifi_init == false )
    return( false );

  for( int entry = 0 ; entry < NETWORKLIST_ENTRYS; entry++ ) {
    if( !strcmp( networkname, wifiset_networklist[ entry ].ssid ) ) {
      return( true );
    }
  }
  return( false );
}


bool wifiset_delete_network( const char *ssid ) {
  if ( wifi_init == false )
    return( false );

  for( int entry = 0 ; entry < NETWORKLIST_ENTRYS; entry++ ) {
    if( !strcmp( ssid, wifiset_networklist[ entry ].ssid ) ) {
      wifiset_networklist[ entry ].ssid[ 0 ] = '\0';
      wifiset_networklist[ entry ].password[ 0 ] = '\0';
      wifiset_save_config();
      return( true );
    }
  }
  return( false );
}

bool wifiset_insert_network( const char *ssid, const char *password ) {
  if ( wifi_init == false )
    return( false );

  // check if existin
  for( int entry = 0 ; entry < NETWORKLIST_ENTRYS; entry++ ) {
    if( !strcmp( ssid, wifiset_networklist[ entry ].ssid ) ) {
      strlcpy( wifiset_networklist[ entry ].password, password, sizeof( wifiset_networklist[ entry ].password ) );
      wifiset_save_config();
      WiFi.scanNetworks();
      wifictl_set_event( WIFICTL_SCAN );
      return( true );
    }
  }
  // check for an emty entry
  for( int entry = 0 ; entry < NETWORKLIST_ENTRYS; entry++ ) {
    if( strlen( wifiset_networklist[ entry ].ssid ) == 0 ) {
      strlcpy( wifiset_networklist[ entry ].ssid, ssid, sizeof( wifiset_networklist[ entry ].ssid ) );
      strlcpy( wifiset_networklist[ entry ].password, password, sizeof( wifiset_networklist[ entry ].password ) );
      wifiset_save_config();
      WiFi.scanNetworks();
      wifictl_set_event( WIFICTL_SCAN );
      return( true );
    }
  }
  return( false ); 
} 

