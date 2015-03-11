#ifndef STEM_ISR
#define STEM_ISR
    
    #include <cytypes.h>
    #include <project.h>
    #include "utils.h"

    //GLOBAL VARIABLES
    extern uint16 wake_count;
    
    //ISR PROTOTYPES
    CY_ISR_PROTO(wakeISR);
    CY_ISR_PROTO(XbeeRx);
    CY_ISR_PROTO(XbeeTx);
    
    //UTILITY FUNCTIONS
    void initAllISR();
    void stopAllISR();
    void initWakeISR();
    void initXbeeRxISR();
    void initXbeeTxISR();
    
#endif
    