/**************************** PSoC4 Stuff *****************************/
// Counters 0 and 1 used in combination as watchdog timers to reset the device if the counters are not reset
// Counter0 gets a match every 2 seconds (match on 0xFFFF with 32khz clock)
CySysWdtWriteMode(CY_SYS_WDT_COUNTER0,CY_SYS_WDT_MODE_NONE);
CySysWdtWriteMatch(CY_SYS_WDT_COUNTER0,0xFFFF);    
CySysWdtWriteClearOnMatch(CY_SYS_WDT_COUNTER0, 1u);
// Cascade counter0 and counter1
CySysWdtWriteCascade(CY_SYS_WDT_CASCADE_01);  
// Counter1 gets match when counter0 matches 16 times, resets device when this occurs
CySysWdtWriteMode(CY_SYS_WDT_COUNTER1,CY_SYS_WDT_MODE_RESET);  
CySysWdtWriteMatch(CY_SYS_WDT_COUNTER1,0x000F); 
CySysWdtWriteClearOnMatch(CY_SYS_WDT_COUNTER1, 1u);
// Reset the counters in an ISR (which must occur more frequently than the counter that's in RESET mode)
CySysWdtResetCounters(CY_SYS_WDT_COUNTER0_RESET|CY_SYS_WDT_COUNTER1_RESET);
// In this example, the aforementioned ISR comes from counter2
    
// Counter2 used to wake up from sleep, interrupts every 4 seconds
CySysWdtWriteMode(CY_SYS_WDT_COUNTER2,CY_SYS_WDT_MODE_INT);
CySysWdtWriteToggleBit(17);
// Clear the interrupt in this ISR
CySysWdtClearInterrupt(CY_SYS_WDT_COUNTER2_INT);

// Enable the counters
CySysWdtEnable(CY_SYS_WDT_COUNTER0_MASK | CY_SYS_WDT_COUNTER1_MASK | CY_SYS_WDT_COUNTER2_MASK);

// Enter deep sleep
// NOTE: Wake-up source must be a WDT interrupt (counter2 in this example)
CySysPmDeepSleep();