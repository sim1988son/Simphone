#include "config.h"

#include "hardware/hardware.h"
#include "hardware/wifiset.h"
#include "hardware/displayset.h"
#include "hardware/sdset.h"
#include "hardware/powerset.h"
#include "ui/ui.h"

void hardware_setup( void ){

    splash_screen_stage_boot();
    // splash_screen_stage_update( "SIMCOM", 100 );

    //splash_screen_stage_update( "init SPIFF & SD", 20 );
    sdset_setup();
    
    // splash_screen_stage_update( "init display", 40 );
    // display_setup();
    
    //splash_screen_stage_update( "init power", 60 );
    power_setup();


    //splash_screen_stage_update( "init gui", 80 );
    //splash_screen_stage_update( "init gui", 100 );
    // splash_screen_stage_finish();
}



void hardware_post_setup( void ){
    
}