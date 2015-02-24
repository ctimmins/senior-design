#include <project.h>
#include "utils.h"
#include "stem_isr.h"

//global wake count variable
uint16 wake_count = 0;

int main()
{
    // initialize wakeup isr
    initWakeISR();
    
    //Initialize LCD
    initLCD();
    CyDelay(2000);
    
    //Start Sleep Timer 
    SleepTimer_Start();
    
    //LED blinking parameters
    int numBlinks = 2;
    int interval = 250;
    int delay = 1000;
    
    CyGlobalIntEnable;
    for(;;)
    {
        blinkLED(1, 300, 0);
        printWakeCount(wake_count);
        sleepPsoc();
    }
}

/* [] END OF FILE */
