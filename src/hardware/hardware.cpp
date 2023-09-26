#include "config.h"

#include "hardware/hardware.h"
#include "hardware/wifiset.h"
#include "hardware/displayset.h"
#include "hardware/sdset.h"
#include "hardware/powerset.h"
#include "ui/ui.h"

void hardware_setup( void ){

    sdset_setup();
    
    power_setup();

}



void hardware_post_setup( void ){
    
}