#include "stem_isr.h"

uint16 wake_count;

CY_ISR(wakeISR) {
    //get state of Sleep Timer's status register and clear interrupt flag
    int status = SleepTimer_GetStatus();
    //blinkLED(3,100,0);
    ++wake_count;
    if (wake_count % 4 == 0) {
        //Wake device up
        //onWakeup();
        
        
    }
    else {
        //Device needs to go back to sleep
        //sleepPsoc();
    }
    
}

/*
    Assigns correct vector address to isr component
*/
void initWakeISR(void) {
    isr_wakeup_Start();
    isr_wakeup_SetVector(wakeISR);
}