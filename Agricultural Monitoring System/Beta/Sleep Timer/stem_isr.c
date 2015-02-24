#include "stem_isr.h"

uint16 wake_count;

CY_ISR(wakeISR) {
    //get state of Sleep Timer's status register and clear interrupt flag
    int status = SleepTimer_GetStatus();
 
    ++wake_count;
    
}

CY_ISR(XbeeRx) {
    //Clear XBee interrupt flag
    UART_Xbee_ReadRxStatus();
    //CommandReceived = XBee_UART_GetByte();
}

void initAllISR() {
    //wakeup ISR
    initWakeISR();
    //XBee Receive ISR
    initXbeeRxISR();
}
    

void initWakeISR(void) {
    isr_wakeup_Start();
    isr_wakeup_SetVector(wakeISR);
}

void initXbeeRxISR() {
    isr_xbee_rx_Start();
    isr_xbee_rx_SetVector(XbeeRx);
}
