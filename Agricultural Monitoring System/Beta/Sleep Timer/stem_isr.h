#ifndef STEM_ISR
#define STEM_ISR
    
    #include <cytypes.h>
    #include <project.h>
    #include "utils.h"
    
    //GLOBAL VARIABLES
    extern uint16 wake_count;
    
    
    //FUNCTION PROTOTYPES
    CY_ISR_PROTO(wakeISR);
    CY_ISR_PROTO(XbeeRx);
    
    //UTILITY FUNCTIONS
    void initAllISR();
    void initWakeISR();
    void initXbeeRxISR();
    
    
    
    
#endif 