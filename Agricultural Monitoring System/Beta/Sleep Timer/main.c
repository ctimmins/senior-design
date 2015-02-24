#include <project.h>
#include "utils.h"
#include "stem_isr.h"

//global wake count variable
uint16 wake_count = 0;

int main()
{   
    //Initialize LCD
    initLCD();
    CyDelay(2000);
    
    //Start Sleep Timer 
    SleepTimer_Start();
    
    //XBee UART block
    UART_Xbee_Start();
 
    // initialize wakeup and xbee isr's
    initAllISR();
    
    //wakeup multiplier. Read every (4 x wMul) seconds
    int wMul = 2;
    
    CyGlobalIntEnable;
    
    for(;;)
    {
        blinkLED(1, 300, 0);
        printWakeCount(wake_count);
        if (wake_count % wMul == 0) {
            onWakeup();
        }
        sleepPsoc();
        
    }
}

/* [] END OF FILE */
