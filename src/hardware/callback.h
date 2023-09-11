#ifndef _CALLBACK_H
    #define _CALLBACK_H

    #include <stdint.h>

    typedef bool ( * CALLBACK_FUNC ) ( EventBits_t event, void *arg );


    typedef struct {
        EventBits_t event;                  /** @brief event mask */
        CALLBACK_FUNC callback_func;        /** @brief pointer to a callback function */
        const char *id;                     /** @brief id for the callback */
        uint64_t counter;                   /** @brief callback function call counter thair returned true */
    } callback_table_t;


    typedef struct callback_t {
        uint32_t entrys;                    /** @brief count callback entrys */
        bool debug;
        callback_table_t *table;            /** @brief pointer to an callback table */
        const char *name;                   /** @brief id for the callback structure */
        callback_t *next_callback_t;
    } callback_t;

    /**
     * @brief init the callback structure
     * 
     * @param   name        pointer to an string thats contains the name for the callback table
     * 
     * @return  pointer to a callback_t structure if success, NULL if failed
     */
    callback_t *callback_init( const char *name );
    /**
     * @brief   register an callback function
     * 
     * @param   callback        pointer to a callback_t structure
     * @param   event           event filter mask
     * @param   callback_func   pointer to a callbackfunc
     * @param   id              pointer to an string thats contains the id aka name for the callback function
     * 
     * @return  true if success, false if failed
     */
    bool callback_register( callback_t *callback, EventBits_t event, CALLBACK_FUNC callback_func, const char *id );
    /**
     * @brief   call all callback function thats match with the event filter mask
     * 
     * @param   callback        pointer to a callback_t structure
     * @param   event           event filter mask
     * @param   arg             argument for the called callback function
     * 
     * @return  true if success, false if failed
     */
    bool callback_send( callback_t *callback, EventBits_t event, void *arg );
    /**
     * @brief   call all callback function thats match with the event filter mask without logging
     * 
     * @param   callback        pointer to a callback_t structure
     * @param   event           event filter mask
     * @param   arg             argument for the called callback function
     * 
     * @return  true if success, false if failed
     */
    bool callback_send_no_log( callback_t *callback, EventBits_t event, void *arg );
    /**
     * @brief enable/disable SPIFFS event logging
     * 
     * @param enable    true if logging enabled, false if logging disabled
     */
    void display_event_logging_enable( bool enable );
    /**
     * @brief enable/disable SPIFFS event logging
     * 
     * @param   callback        pointer to a callback_t structure
     * @param   debuging        true enable debuging, false disable debuging
     */
    void callback_enable_debuging( callback_t *callback, bool debuging );
    
    void callback_print( void );

#endif // _CALLBACK_H