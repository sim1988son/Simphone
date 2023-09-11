#include <Arduino.h>
#include "callback.h"
#include "json_psram_allocator.h"
#include "alloc.h"
#include "main.h"
#include "hardware/touchset.h"
#include "hardware/powerset.h"


volatile bool DRAM_ATTR touch_irq_flag = false;
portMUX_TYPE DRAM_ATTR Touch_IRQ_Mux = portMUX_INITIALIZER_UNLOCKED;
void IRAM_ATTR touch_irq( void );

bool touched = false;

// lv_indev_t *touch_indev = NULL;
callback_t *touch_callback = NULL;

// static bool touch_read(lv_indev_drv_t * drv, lv_indev_data_t*data);
// static bool touch_getXY( int16_t &x, int16_t &y );
bool touch_send_event_cb( EventBits_t event, void *arg );
bool touch_power_event_cb( EventBits_t event, void *arg );
bool touch_power_loop_cb( EventBits_t event , void *arg );

static SemaphoreHandle_t xSemaphores = NULL;

void touch_setup( void ) {


    xSemaphores = xSemaphoreCreateMutex();
    // touch_indev = lv_indev_get_next( NULL );
    // touch_indev->driver.read_cb = touch_read;
    attachInterrupt( WAKE_PIN, &touch_irq, FALLING );

    esp_sleep_wakeup_cause_t wakeup_reason;
    wakeup_reason = esp_sleep_get_wakeup_cause();

    power_register_cb( POWERMGM_SILENCE_WAKEUP | POWERMGM_STANDBY | POWERMGM_WAKEUP | POWERMGM_ENABLE_INTERRUPTS | POWERMGM_DISABLE_INTERRUPTS , touch_power_event_cb, "touch" );
    power_register_loop_cb( POWERMGM_SILENCE_WAKEUP | POWERMGM_STANDBY | POWERMGM_WAKEUP, touch_power_loop_cb, "touch loop" );
}

bool touch_power_loop_cb( EventBits_t event , void *arg ) {
    touch_loop();
    return( true );
}

bool touch_lock_take( void ) {
    return xSemaphoreTake( xSemaphores, portMAX_DELAY ) ==  pdTRUE;
}
void touch_lock_give( void ) {
    xSemaphoreGive( xSemaphores );
}

bool touch_power_event_cb( EventBits_t event, void *arg ) {

    switch( event ) {
        case POWERMGM_STANDBY:          log_i("go standby");
                                        // gpio_wakeup_enable( (gpio_num_t)WAKE_PIN, GPIO_INTR_LOW_LEVEL );
                                        // esp_sleep_enable_gpio_wakeup ();
                                        esp_sleep_enable_ext0_wakeup(WAKE_PIN, 0);
                                        // if ( touch_lock_take() ) {
                                        //     // ttgo->touchToMonitor();
                                        //     touch_lock_give();
                                        // }
                                        break;
        case POWERMGM_WAKEUP:           log_i("go wakeup");
                                        // if ( touch_lock_take() ) {
                                        //     // ttgo->touchToMonitor();
                                        //     touch_lock_give();
                                        // }
                                        break;
        case POWERMGM_SILENCE_WAKEUP:   log_i("go silence wakeup");
                                        break;
        case POWERMGM_ENABLE_INTERRUPTS:
                                        attachInterrupt( WAKE_PIN, &touch_irq, FALLING );
                                        break;
        case POWERMGM_DISABLE_INTERRUPTS:
                                        detachInterrupt( WAKE_PIN );
                                        break;
    }
    return( true );
}

void IRAM_ATTR touch_irq( void ) {
    portENTER_CRITICAL_ISR(&Touch_IRQ_Mux);
    touch_irq_flag = true;
    portEXIT_CRITICAL_ISR(&Touch_IRQ_Mux);
    power_set_event( POWERMGM_PMU_BUTTON );

}

bool toucheded( void ) {
    // We use two flags, one changes in the interrupt handler
    // the other controls whether we poll the sensor,
    // and gets cleared when the level is no longer low,
    // meaning the touch has finished
    portENTER_CRITICAL( &Touch_IRQ_Mux );
    bool temp_touch_irq_flag = touch_irq_flag;
    touch_irq_flag = false;
    portEXIT_CRITICAL( &Touch_IRQ_Mux );
    touched |= temp_touch_irq_flag;
    power_set_event( POWERMGM_PMU_BUTTON );
    return touched;
}

void touch_loop( void ) {
    // TTGOClass *ttgo = TTGOClass::getWatch();

    /*
     * handle IRQ event
     */
    // portENTER_CRITICAL( &Touch_IRQ_Mux );
    // bool temp_touch_irq_flag = touch_irq_flag;
    // touch_irq_flag = false;
    // portEXIT_CRITICAL( &Touch_IRQ_Mux );

    // if ( temp_touch_irq_flag ) {                
        // while( !ttgo->bma->readInterrupt() );

        // if ( ttgo->bma->isDoubleClick() ) {
            // power_set_event( POWERMGM_PMU_BUTTON );
            // touch_send_event_cb( POWERMGM_PMU_BUTTON, (void *)"" );
        // }
    // }
}

bool touch_send_event_cb( EventBits_t event, void *arg ) {
    return( callback_send( touch_callback, event, arg ) );
}
