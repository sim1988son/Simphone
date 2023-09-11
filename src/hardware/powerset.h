#ifndef _POWERMGM_H
    #define _POWERMGM_H

    #include <Arduino.h>
    #include "callback.h"

    #define POWERMGM_STANDBY                    _BV(0)         /** @brief event mask for powermgm standby */
    #define POWERMGM_STANDBY_REQUEST            _BV(1)         /** @brief event mask for powermgm standby request */
    #define POWERMGM_SILENCE_WAKEUP             _BV(2)         /** @brief event mask for powermgm silence wakeup */
    #define POWERMGM_SILENCE_WAKEUP_REQUEST     _BV(3)         /** @brief event mask for powermgm wakeup silence request */
    #define POWERMGM_WAKEUP                     _BV(4)         /** @brief event mask for powermgm wakeup */
    #define POWERMGM_WAKEUP_REQUEST             _BV(5)         /** @brief event mask for powermgm wakeup request */
    #define POWERMGM_PMU_BUTTON                 _BV(6)         /** @brief event mask for powermgm pmu button is pressed */
    #define POWERMGM_BMA_DOUBLECLICK            _BV(9)         /** @brief event mask for powermgm bma soubleclick */
    #define POWERMGM_BMA_TILT                   _BV(10)        /** @brief event mask for powermgm bma tilt */
    #define POWERMGM_RTC_ALARM                  _BV(11)        /** @brief event mask for powermgm rtc alarm */
    #define POWERMGM_SHUTDOWN                   _BV(12)        /** @brief event mask for powermgm shutdown */
    #define POWERMGM_RESET                      _BV(13)        /** @brief event mask for powermgm reset */
    #define POWERMGM_PMU_LONG_BUTTON            _BV(14)        /** @brief event mask for powermgm pmu button is long pressed */
    #define POWERMGM_DISABLE_INTERRUPTS         _BV(15)        
    #define POWERMGM_ENABLE_INTERRUPTS          _BV(16)        
    
    void power_setup( void );

    void power_loop( void );

    void power_set_event( EventBits_t bits );
    /**
     * @brief clear a power managemt event
     * 
     * @param   bits    event to trigger, example: POWERMGM_WIFI_ON_REQUEST for switch an WiFi
     */
    void power_clear_event( EventBits_t bits );
    /**
     * @brief get a power managemt event state
     * 
     * @param   bits    event state, example: POWERMGM_STANDBY to evaluate if the system in standby
     * 
     * @return  EventBits_t    event state
     */
    EventBits_t power_get_event( EventBits_t bits );
    /**
     * @brief registers a callback function which is called on a corresponding event
     * 
     * @param   event               possible values: POWERMGM_STANDBY, POWERMGM_SILENCE_WAKEUP, POWERMGM_WAKEUP and POWERMGM_RTC_ALARM
     * @param   callback_func       pointer to the callback function 
     * @param   id                  pointer to an string
     */
    bool power_register_cb( EventBits_t event, CALLBACK_FUNC callback_func, const char *id );
    /**
     * @brief registers a callback function which is called on a corresponding loop event
     * 
     * @param   event               possible values: POWERMGM_STANDBY, POWERMGM_SILENCE_WAKEUP, POWERMGM_WAKEUP
     * @param   callback_func       pointer to the callback function 
     * @param   id                  pointer to an string
     */
    bool power_register_loop_cb( EventBits_t event, CALLBACK_FUNC callback_func, const char *id );
    /**
     * @brief send an interrupt disable request
     */
    void power_disable_interrupts( void );
    /**
     * @brief send an interrupt enable request
     */
    void power_enable_interrupts( void );
    
#endif
