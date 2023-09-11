#include <Arduino.h>
#include "callback.h"
#include "json_psram_allocator.h"
#include "alloc.h"
#include "main.h"
#include "hardware/touchset.h"
#include "hardware/powerset.h"
#include "hardware/wifiset.h"


EventGroupHandle_t power_status = NULL;
portMUX_TYPE DRAM_ATTR powerMux = portMUX_INITIALIZER_UNLOCKED;

callback_t *power_callback = NULL;
callback_t *power_loop_callback = NULL;

// esp_pm_config_esp32_t pm_config;

bool power_send_event_cb( EventBits_t event );
bool power_send_loop_event_cb( EventBits_t event );

void power_setup( void ) {

    power_status = xEventGroupCreate();

    // pmu_setup();
    wifiset_setup();
    touch_setup();
    // timesync_setup();
    // rtcctl_setup();
    // blectl_read_config();
    // sound_read_config();
    
    power_set_event( POWERMGM_WAKEUP );
}

void power_loop( void ){

        // check if a button or doubleclick was release
    if( power_get_event( POWERMGM_PMU_BUTTON | POWERMGM_BMA_DOUBLECLICK | POWERMGM_BMA_TILT | POWERMGM_RTC_ALARM ) ) {
        if ( power_get_event( POWERMGM_STANDBY ) || power_get_event( POWERMGM_SILENCE_WAKEUP ) ) {
            power_set_event( POWERMGM_WAKEUP_REQUEST );
        }
        // else {
        //     if ( power_get_event( POWERMGM_PMU_BUTTON | POWERMGM_BMA_DOUBLECLICK ) ) {
        //         power_set_event( POWERMGM_STANDBY_REQUEST );
        //     }
        // }
        power_clear_event( POWERMGM_PMU_BUTTON | POWERMGM_BMA_DOUBLECLICK  | POWERMGM_BMA_TILT | POWERMGM_RTC_ALARM );
    }

    if ( power_get_event( POWERMGM_WAKEUP_REQUEST ) && power_get_event( POWERMGM_WAKEUP ) ) {
        lv_disp_trig_activity( NULL );
        power_clear_event( POWERMGM_WAKEUP_REQUEST );
    }

    // drive into
    if ( power_get_event( POWERMGM_SILENCE_WAKEUP_REQUEST | POWERMGM_WAKEUP_REQUEST ) ) {
        power_clear_event( POWERMGM_STANDBY | POWERMGM_SILENCE_WAKEUP | POWERMGM_WAKEUP );

        //Network transfer times are likely a greater time consumer than actual computational time
        if (power_get_event( POWERMGM_SILENCE_WAKEUP_REQUEST ) ) {
            log_i("go silence wakeup");
            #if CONFIG_PM_ENABLE
                pm_config.max_freq_mhz = 240;
                pm_config.min_freq_mhz = 40;
                pm_config.light_sleep_enable = true;
                ESP_ERROR_CHECK( esp_pm_configure(&pm_config) );
                log_i("custom arduino-esp32 framework detected, enable PM/DFS support");
            #else
                setCpuFrequencyMhz(80);
            #endif
            power_set_event( POWERMGM_SILENCE_WAKEUP );
            power_send_event_cb( POWERMGM_SILENCE_WAKEUP );
        }
        else {
            log_i("go wakeup");
            #if CONFIG_PM_ENABLE
                pm_config.max_freq_mhz = 240;
                pm_config.min_freq_mhz = 240;
                pm_config.light_sleep_enable = false;
                ESP_ERROR_CHECK( esp_pm_configure(&pm_config) );
                log_i("custom arduino-esp32 framework detected, enable PM/DFS support");
            #else
                setCpuFrequencyMhz(240);
            #endif
            power_set_event( POWERMGM_WAKEUP );
            power_send_event_cb( POWERMGM_WAKEUP );
            // motor_vibe(3);
        }

        log_i("Free heap: %d", ESP.getFreeHeap());
        log_i("Free PSRAM heap: %d", ESP.getFreePsram());
        log_i("uptime: %d", millis() / 1000 );

    }        
    else if( power_get_event( POWERMGM_STANDBY_REQUEST ) ) { 
        
        //Save info to avoid buzz when standby after silent wake
        bool noBuzz = power_get_event( POWERMGM_SILENCE_WAKEUP | POWERMGM_SILENCE_WAKEUP_REQUEST );
        
        // send standby event
        power_clear_event( POWERMGM_STANDBY | POWERMGM_SILENCE_WAKEUP | POWERMGM_WAKEUP );
        power_set_event( POWERMGM_STANDBY );

//        adc_power_off();

        if ( power_send_event_cb( POWERMGM_STANDBY ) ) {
            // if (!noBuzz) motor_vibe(3);  //Only buzz if a non silent wake was performed
            log_i("Free heap: %d", ESP.getFreeHeap());
            log_i("Free PSRAM heap: %d", ESP.getFreePsram());
            log_i("uptime: %d", millis() / 1000 );
            log_i("go standby");
            delay( 100 );
            setCpuFrequencyMhz( 80 );
            esp_light_sleep_start();
            // from here, the consumption is round about 2.5mA
            // total standby time is 152h (6days) without use?
        }
        else {
            log_i("Free heap: %d", ESP.getFreeHeap());
            log_i("Free PSRAM heap: %d", ESP.getFreePsram());
            log_i("uptime: %d", millis() / 1000 );
            log_i("go standby blocked");
            #if CONFIG_PM_ENABLE
                pm_config.max_freq_mhz = 80;
                pm_config.min_freq_mhz = 40;
                pm_config.light_sleep_enable = true;
                ESP_ERROR_CHECK( esp_pm_configure(&pm_config) );
                log_i("custom arduino-esp32 framework detected, enable PM/DFS support");
                // from here, the consumption is round about 14mA
                // total standby time is 30h without use?
            #else
                setCpuFrequencyMhz(80);
                // from here, the consumption is round about 23mA
                // total standby time is 19h without use?
            #endif
        }
    }
    power_clear_event( POWERMGM_SILENCE_WAKEUP_REQUEST | POWERMGM_WAKEUP_REQUEST | POWERMGM_STANDBY_REQUEST );

    // send loop event depending on powermem state
    if ( power_get_event( POWERMGM_STANDBY ) ) {
        vTaskDelay( 250 );
        power_send_loop_event_cb( POWERMGM_STANDBY );
    }
    else if ( power_get_event( POWERMGM_WAKEUP ) ) {
        power_send_loop_event_cb( POWERMGM_WAKEUP );
    }
    else if ( power_get_event( POWERMGM_SILENCE_WAKEUP ) ) {
        power_send_loop_event_cb( POWERMGM_SILENCE_WAKEUP );
    }
}

void power_shutdown( void ) {
    power_send_event_cb( POWERMGM_SHUTDOWN );
    // pmu_shutdown();
}

void power_reset( void ) {
    power_send_event_cb( POWERMGM_RESET );
    ESP.restart();
}

void power_set_event( EventBits_t bits ) {
    portENTER_CRITICAL(&powerMux);
    xEventGroupSetBits( power_status, bits );
    portEXIT_CRITICAL(&powerMux);
}

void power_clear_event( EventBits_t bits ) {
    portENTER_CRITICAL(&powerMux);
    xEventGroupClearBits( power_status, bits );
    portEXIT_CRITICAL(&powerMux);
}

EventBits_t power_get_event( EventBits_t bits ) {
    portENTER_CRITICAL(&powerMux);
    EventBits_t temp = xEventGroupGetBits( power_status ) & bits;
    portEXIT_CRITICAL(&powerMux);
    return( temp );
}

bool power_register_cb( EventBits_t event, CALLBACK_FUNC callback_func, const char *id ) {
    if ( power_callback == NULL ) {
        power_callback = callback_init( "power" );
        if ( power_callback == NULL ) {
            log_e("power callback alloc failed");
            while(true);
        }
    }    
    return( callback_register( power_callback, event, callback_func, id ) );
}

bool power_register_loop_cb( EventBits_t event, CALLBACK_FUNC callback_func, const char *id ) {
    if ( power_loop_callback == NULL ) {
        power_loop_callback = callback_init( "power loop" );
        if ( power_loop_callback == NULL ) {
            log_e("power loop callback alloc failed");
            while(true);
        }
    }    
    return( callback_register( power_loop_callback, event, callback_func, id ) );
}

bool power_send_event_cb( EventBits_t event ) {
    return( callback_send( power_callback, event, (void*)NULL ) );
}

bool power_send_loop_event_cb( EventBits_t event ) {
    return( callback_send_no_log( power_loop_callback, event, (void*)NULL ) );
}

void power_disable_interrupts( void ) {
    power_send_event_cb( POWERMGM_DISABLE_INTERRUPTS );
}

void power_enable_interrupts( void ) {
    power_send_event_cb( POWERMGM_ENABLE_INTERRUPTS );
}
