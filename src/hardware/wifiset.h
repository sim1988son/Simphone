#ifndef _UI_SET_WIFISET_H
#define _UI_SET_WIFISET_H

    #include <Arduino.h>
    #include "callback.h"

    #if __has_include("lvgl.h")
    #include "lvgl.h"
    #else
    #include "lvgl/lvgl.h"
    #endif


    #define WIFISETUP_DELAY               10
    #define NETWORKLIST_ENTRYS            20
    #define WIFISETUP_JSON_CONFIG_FILE    "/wifi_setup.json"

    typedef struct {
        char ssid[64]="";
        char password[64]="";
    } networklist;

    typedef struct {
        bool autoon = true;             /** @brief enable on auto on/off an wakeup and standby */
        bool enable_on_standby = false;
    } wifiset_config_t;
    

        enum wifictl_event_t {
            WIFICTL_CONNECT                = _BV(0),
            WIFICTL_CONNECT_IP             = _BV(1),
            WIFICTL_DISCONNECT             = _BV(2),
            WIFICTL_ON                     = _BV(3),
            WIFICTL_OFF                    = _BV(4),
            WIFICTL_ACTIVE                 = _BV(5),
            WIFICTL_ON_REQUEST             = _BV(6),
            WIFICTL_OFF_REQUEST            = _BV(7),
            WIFICTL_WPS_REQUEST            = _BV(8),
            WIFICTL_WPS_SUCCESS            = _BV(9),
            WIFICTL_WPS_FAILED             = _BV(10),
            WIFICTL_SCAN                   = _BV(11),
            WIFICTL_FIRST_RUN              = _BV(12),
            WIFICTL_AUTOON                 = _BV(13)
        };


    // void wifiset_set_networklist(Network_Status_t _status_net );
    void wifiset_setup( void );
    bool wifiset_is_known( const char* networkname );
    bool wifiset_delete_network( const char *ssid );
    bool wifiset_insert_network( const char *ssid, const char *password );
    void wifiset_save_config( void ) ;
    void wifiset_load_config( void );
    bool wifictl_get_autoon( void );
    void wifictl_set_autoon( bool autoon );
    bool wifi_status(void);
    // void loop_wifiset( lv_timer_t  * timer );
    void wifictl_on(void);
    void wifictl_off(void);

    void wifictl_set_event( EventBits_t bits );
    bool wifictl_get_event( EventBits_t bits );
    void wifictl_clear_event( EventBits_t bits );
    bool wifictl_send_event_cb( EventBits_t event, void *arg );
    void wifictl_standby( void );

    void wifictl_wakeup( void );

     /* @param   event  possible values: WIFICTL_CONNECT,
     *                                      WIFICTL_CONNECT_IP,
     *                                      WIFICTL_DISCONNECT,
     *                                      WIFICTL_ON,
     *                                      WIFICTL_OFF,       
     *                                      WIFICTL_ACTIVE,    
     *                                      WIFICTL_ON_REQUEST,
     *                                      WIFICTL_OFF_REQUEST,
     *                                      WIFICTL_WPS_REQUEST,
     *                                      WIFICTL_WPS_SUCCESS,
     *                                      WIFICTL_WPS_FAILED,
     *                                      WIFICTL_SCAN,      
     *                                      WIFICTL_FIRST_RUN
     * @param   wifictl_event_cb   pointer to the callback function */

    bool wifictl_register_cb( EventBits_t event, CALLBACK_FUNC callback_func, const char *id );
    
    void wifictl_set_enable_on_standby( bool enable );

    bool wifictl_get_enable_on_standby( void );
#endif