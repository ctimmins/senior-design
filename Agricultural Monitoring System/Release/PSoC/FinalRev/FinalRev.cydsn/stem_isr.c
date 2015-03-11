#include "stem_isr.h"

uint16 wake_count;

CY_ISR(wakeISR) {
    //get state of Sleep Timer's status register and clear interrupt flag
    int status = SleepTimer_GetStatus();
 
    ++wake_count;
    
}

/* 
Interrupt is generated whenever a byte is received on the PSOC
*/
CY_ISR(XbeeRx) {
    //Clear XBee interrupt flag
    Xbee_UART_ReadRxStatus();
    int num = Xbee_UART_GetTxBufferSize();
    //CommandReceived = XBee_UART_GetByte();
}

/* 
Interrupt is generated whenever a byte is sent from the PSOC
*/
CY_ISR(XbeeTx) {
    
}

void initAllISR() {
    //wakeup ISR
    initWakeISR();
    //XBee Receive ISR
    initXbeeRxISR();
    //XBee Send ISR
    initXbeeTxISR();
}
    
void stopAllISR(void) {
    isr_wakeup_Stop();
    isr_xbee_rx_Stop();
    isr_xbee_tx_Stop();
}

void initWakeISR(void) {
    isr_wakeup_Start();
    isr_wakeup_SetVector(wakeISR);
}

void initXbeeRxISR() {
    isr_xbee_rx_Start();
    isr_xbee_rx_SetVector(XbeeRx);
}

void initXbeeTxISR() {
    isr_xbee_tx_Start();
    isr_xbee_tx_SetVector(XbeeTx);
}
