#include "utils.h"

void blinkLED(int count, int interval, int delay) {
    int i = 0;
    for (i; i < count; ++i) {
       CyDelay(interval);
       Pin_led_Write(1);
       CyDelay(interval);
       Pin_led_Write(0);
    }
    CyDelay(delay);
}

void initLCD() {
    LCD_Start();
    LCD_ClearDisplay();
    LCD_PrintString("status: ");
    LCD_Position(1,0);
    LCD_PrintString("Count: ");
    LCD_Position(1,7);
    LCD_PrintNumber(SleepTimer_initVar);
}

void printSleepTimerStatus(uint8 status) {
    LCD_Position(0,8);
    LCD_PrintString("       ");
    if (status == SleepTimer_PM_INT_SR_CTW) {
        LCD_Position(0,8);
        LCD_PrintString("CTW");
    }
    else {
        LCD_Position(0,8);
        LCD_PrintString("null");
    }
    CyDelay(1000);
    LCD_Position(0,8);
    LCD_PrintString("       ");
}

void printWakeCount(uint16 count) {
    LCD_Position(1, 7);
    LCD_PrintNumber(count);
}

void sleepPsoc() {
    //Prepare clock tree configuration
    CyPmSaveClocks();
    CyDelay(25);
    CyPmSleep(PM_SLEEP_TIME_NONE, PM_SLEEP_SRC_CTW);
    CyPmRestoreClocks();
}

void onWakeup() {
    //CyPmRestoreClocks();
    LCD_Position(0,0);
    LCD_PrintString("Im awake!");
    CyDelay(1000);
    LCD_Position(0,0);
    LCD_PrintString("         ");
}
    