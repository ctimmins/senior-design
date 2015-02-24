#ifndef UTILS_H
#define UTILS_H
    
    #include <cytypes.h>
    #include <project.h>
    
    //LED
    void blinkLED(int count, int interval, int delay);
    
    //LCD
    void initLCD();
    
    //Sleep Timer
    void printSleepTimerStatus(uint8 status);
    
    void printWakeCount(uint16 count);
    
    void sleepPsoc();
    
    void onWakeup();
    
    //XBee and UART
    void printXbeeRx(char ch);
    
    //Hardware Components
    void initAllComponents();
    
#endif
    