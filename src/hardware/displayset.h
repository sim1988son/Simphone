#ifndef _DISPLAY_H
    #define _DISPLAY_H

    #include <Arduino.h>
    #include "callback.h"

    #define DISPLAYCTL_BRIGHTNESS       _BV(0)          /** @brief event mask display brightness, callback arg is (uint32_t*) */
    #define DISPLAYCTL_TIMEOUT          _BV(1)          /** @brief event mask display timeout, callback arg is (bool*) */
    #define DISPLAYCTL_SCREENSHOT       _BV(2)          /** @brief event mask display screenshot, callback arg is (bool*) */

    #define DISPLAYSETUP_JSON_CONFIG_FILE    "/display_setup.json"


    #define DISPLAY_MIN_TIMEOUT         15              /** @brief min display timeout */
    #define DISPLAY_MAX_TIMEOUT         300             /** @brief max display timeout */
    #define DISPLAY_MIN_BRIGHTNESS      8               /** @brief min display brightness */
    #define DISPLAY_MAX_BRIGHTNESS      255             /** @brief max display brightness */
    #define DISPLAY_MIN_ROTATE          0               /** @brief min display rotation */
    #define DISPLAY_MAX_ROTATE          270             /** @brief max display rotation */


    typedef struct {
        bool        display_auto    = false;
        uint16_t    display_brig    = DISPLAY_MAX_BRIGHTNESS; 
        uint16_t    display_times   = DISPLAY_MIN_TIMEOUT;
        uint16_t    display_bg      = 1; 
        uint16_t    display_look    = 1; 
        bool        display_darkon  = false;
        // uint32_t brightness = DISPLAY_MAX_BRIGHTNESS;   /** @brief display brightness */
        // uint32_t timeout = DISPLAY_MIN_TIMEOUT;         /** @brief display time out */
        // uint32_t rotation = 0;                          /** @brief display rotation */
        // bool block_return_maintile = false;             /** @brief block back to main tile on standby */
        // bool long_press_take_screenshot = false;        /** @brief long press take a screenshoot, otherwise back to maintile */
        // uint32_t background_image = 2;                  /** @brief background image */
    } display_config_t;

    void display_setup( void );
    void display_loop( void );

    void display_save_config( void );
    void display_load_config( void );

    bool display_get_display_auto( void );
    void display_set_display_auto( bool display_auto );

    uint16_t display_get_display_brig( void );
    void display_set_display_brig( uint16_t display_brig );

    void display_set_brightness(int brig);
    
    uint16_t display_get_display_times( void );
    void display_set_display_times( uint16_t display_times );

    uint16_t display_get_display_bg( void );
    void display_set_display_bg( uint16_t display_bg );

    uint16_t display_get_display_look( void );
    void display_set_display_look( uint16_t display_look );

    bool display_get_display_darkon( void );
    void display_set_display_darkon( bool display_darkon );
    

    // uint32_t display_get_rotation( void );

    // void display_set_rotation( uint32_t rotation );

    // uint32_t display_get_background_image( void );

    // void display_set_background_image( uint32_t background_image );

    void display_standby( void );
    /**
     * @brief set display into normal mode or leave it in standby if a silence wakeup occur
     */
    void display_wakeup( bool silence );
    /**
     * @brief registers a callback function which is called on a corresponding event
     * 
     * @param   event           possible values: DISPLAYCTL_BRIGHTNESS and DISPLAYCTL_TIMEOUT
     * @param   callback_func   pointer to the callback function
     * @param   id              program id
     * 
     * @return  true if success, false if failed
     */
    bool display_register_cb( EventBits_t event, CALLBACK_FUNC callback_func, const char *id );

#endif // _DISPLAY_H