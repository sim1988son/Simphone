#ifndef _UI_HARD_STORAGE_H
#define _UI_HARD_STORAGE_H

    #include <Arduino.h>
    #include "callback.h"

    #define SDCARD_ON           _BV(0)          /** @brief event mask display brightness, callback arg is (uint32_t*) */


    static bool statusSdCard = false;


    void sdset_setup( void );

    bool sdset_get_status( void );

    void sdset_set_status( bool _status );

    void sdset_set_event( EventBits_t bits );

    void sdset_clear_event( EventBits_t bits );

    EventBits_t sdset_get_event( EventBits_t bits );

    bool sdset_register_cb( EventBits_t event, CALLBACK_FUNC callback_func, const char *id );

    bool sdset_register_loop_cb( EventBits_t event, CALLBACK_FUNC callback_func, const char *id );
    
    bool sdset_send_event_cb( EventBits_t event, void *arg );

// void storage_save_config( void );
// void storage_load_config( void );
// bool storage_get_storage_auto( void );
// void storage_set_storage_auto( bool storage_auto );
// int32_t storage_get_storage_brig( void );
// void storage_set_storage_brig( int32_t battery_brig );


#endif