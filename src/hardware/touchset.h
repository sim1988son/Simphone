#ifndef _TOUCH_H

    #define _TOUCH_H

    // #include "TTGO.h"
    /**
     * @brief setup touch
     */
    void touch_setup( void );
    bool toucheded( void );
    bool touch_lock_take( void );
    void touch_lock_give( void );
    void touch_loop( void ) ;
#endif // _TOUCH_H