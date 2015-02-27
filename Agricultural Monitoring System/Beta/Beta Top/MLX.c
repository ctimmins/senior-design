/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
/* This file is the source code for the IR Sensor. Do not change.*/
#include <project.h>
#include <Xbee_UART.h>
#include "utils.h"

uint8 EEPROM_data[256] = {0};


uint16 IRDataConcat[64] = {0};
//const uint16 *arrayPointer = &IRDataConcat;
uint16 const *IRLowPointer = IRDataConcat;
uint16 const *IRHighPointer = IRDataConcat + 32;





void readEEPROM()
{
    I2C_1_MasterSendStart(0x50,0);
    I2C_1_MasterWriteByte(0x00);
    I2C_1_MasterSendRestart(0x50,1);
    int i;
    for(i = 0;i < 256; i++){
    EEPROM_data[i] = I2C_1_MasterReadByte(1);   
    }
    I2C_1_MasterSendStop();
}

void writeOscTrimValue()
{
    uint8 EEPROM_extraction = EEPROM_data[0xF7];
    I2C_1_MasterSendStart(0x60, 0);
	I2C_1_MasterWriteByte(0x04);
	I2C_1_MasterWriteByte(EEPROM_extraction - 0xAA);
	I2C_1_MasterWriteByte(EEPROM_extraction);
	I2C_1_MasterWriteByte(0x56);
	I2C_1_MasterWriteByte(0x00);
	I2C_1_MasterSendStop();    
}

void writeConfigValue(){
    uint8 configMS = EEPROM_data[0xF6];
    uint8 configLS = EEPROM_data[0xF5];
    I2C_1_MasterSendStart(0x60,0);
    I2C_1_MasterWriteByte(0x03);
    I2C_1_MasterWriteByte(configLS-0x55);
    I2C_1_MasterWriteByte(configLS);
    I2C_1_MasterWriteByte(configMS-0x55);
    I2C_1_MasterWriteByte(configMS);
    I2C_1_MasterSendStop();
}

uint16 readPTAT(){
            uint8 PTAT[2] = {0};                      
            I2C_1_MasterSendStart(0x60,0);
            I2C_1_MasterWriteByte(0x02);
            I2C_1_MasterWriteByte(0x90);
            I2C_1_MasterWriteByte(0x00);
            I2C_1_MasterWriteByte(0x01);
            I2C_1_MasterSendRestart(0x60,1);
            PTAT[0] = I2C_1_MasterReadByte(1);//LS
            PTAT[1] = I2C_1_MasterReadByte(1);//MS
            I2C_1_MasterSendStop();
            return PTAT[0] | (PTAT[1] << 8);
}

uint16 readCompensation(){
            uint8 Vcp[2] = {0};   
            I2C_1_MasterSendStart(0x60,0);
        	I2C_1_MasterWriteByte(0x02);
        	I2C_1_MasterWriteByte(0x91);
        	I2C_1_MasterWriteByte(0x00);
        	I2C_1_MasterWriteByte(0x01);
        	I2C_1_MasterSendRestart(0x60,1);
        	Vcp[0] = I2C_1_MasterReadByte(1);
        	Vcp[1] = I2C_1_MasterReadByte(1);
        	I2C_1_MasterSendStop();   
            return Vcp[0] | (Vcp[1] << 8);
}

void readFrame(){
             uint8 IRData[0x80] = {0};
            uint16 Concat[64] = {0};
            I2C_1_MasterSendStart(0x60,0);
            I2C_1_MasterWriteByte(0x02);
            I2C_1_MasterWriteByte(0x00);
            I2C_1_MasterWriteByte(0x01);
            I2C_1_MasterWriteByte(0x40);
            I2C_1_MasterSendRestart(0x60,1);
            int y;
            for(y = 0; y < 0x80;y++){
            IRData[y] = I2C_1_MasterReadByte(1);
            }
            I2C_1_MasterSendStop();
            //concatenating
            for(y = 0; y < 0x40;y++){    
            Concat[y] = IRData[y+y] | (IRData[y+y+1] << 8);    
            }   
            
            for(y = 0; y < 0x40;y++){    
            IRDataConcat[y] = Concat[y];  
            }  
}

void XBeeTxPTATparam(){
            int i;
            char outData[2];
            Xbee_UART_PutString("PP,");

            for(i = 0xDA; i < (0xDF + 1);i++){
                sprintf(outData, "%X", EEPROM_data[i]);
                Xbee_UART_PutString(outData);

            }

            CyDelay(200);
            //Xbee_UART_ClearTxBuffer();
}

void XBeeTxPTAT(){
            char PTATData[2];
            Xbee_UART_PutString("Pt,");
            sprintf(PTATData, "%X", readPTAT());
            Xbee_UART_PutString(PTATData);
            Xbee_UART_PutChar(',');
            Xbee_UART_PutChar(calculateCheckSum(PTATData));
            CyDelay(200);
            //Xbee_UART_ClearTxBuffer();   
}

void XBeeTxCompensation(){
            char CompData[2];
            Xbee_UART_PutString("MC,");
            sprintf(CompData, "%X", readCompensation());
            Xbee_UART_PutString(CompData);
            Xbee_UART_PutChar(',');
            Xbee_UART_PutChar(calculateCheckSum(CompData));
            

            CyDelay(200);
            //Xbee_UART_ClearTxBuffer();
}

void XBeeTxPixelOffsets(){
            int i;
            char outData[2];
            Xbee_UART_PutString("PO,");
           
            for(i = 0; i < (0x3F + 1);i++){
                sprintf(outData, "%X", EEPROM_data[i]);
                Xbee_UART_PutChar(0x27);
                Xbee_UART_PutString(outData);
                Xbee_UART_PutChar(0x27);

            }            
             
            CyDelay(200);
            //Xbee_UART_ClearTxBuffer();   
}

void XBeeTxPixelSlope(){
            int i;
            char outData[2];
            Xbee_UART_PutString("Sl,");

            for(i = 0x40; i < (0x7F + 1);i++){

                sprintf(outData, "%X", EEPROM_data[i]);
                Xbee_UART_PutChar(0x27);
                Xbee_UART_PutString(outData);
                                Xbee_UART_PutChar(0x27);

            }
            Xbee_UART_PutChar(',');
            Xbee_UART_PutChar(calculateCheckSum(outData));
            CyDelay(200);
            //Xbee_UART_ClearTxBuffer();
    
}

void XBeeTxPixelSensitivity(){
            int i;
            char outData[2];
            Xbee_UART_PutString("Se,");
            for(i = 0x80; i < (0xBF + 1);i++){

                sprintf(outData, "%X", EEPROM_data[i]);
                Xbee_UART_PutChar(0x27);
                Xbee_UART_PutString(outData);
                Xbee_UART_PutChar(0x27);

            }
            CyDelay(200);
            //Xbee_UART_ClearTxBuffer();
}

void XBeeTxParameters(){
            int i;
            char outData[2];
            Xbee_UART_PutString("MP,");
            for(i = 0xD4; i < (0xD9 + 1);i++){
                sprintf(outData, "%X", EEPROM_data[i]);
                Xbee_UART_PutString(outData);
                Xbee_UART_PutChar(' ');
            }
            for(i = 0xE0; i < (0xE5 + 1);i++){
                sprintf(outData, "%X", EEPROM_data[i]);
                Xbee_UART_PutString(outData);
            }

            CyDelay(200);
            //Xbee_UART_ClearTxBuffer();
}

void XBeeTxFrame(){
            int i;
            //char outData[4];
            
            //send first 64 bytes of IR data
            Xbee_UART_PutString("IL,");
            Xbee_UART_PutArray(IRLowPointer, 64);
            Xbee_UART_PutChar(',');
            Xbee_UART_PutChar(calculateCheckSum((char *)IRLowPointer));
            CyDelay(200);
            
            //send last 64 bytes of IR data
            Xbee_UART_PutString("IH,");
            Xbee_UART_PutArray(IRHighPointer, 64);
            Xbee_UART_PutChar(',');
            Xbee_UART_PutChar(calculateCheckSum((char *)IRHighPointer));
             CyDelay(200);
}




    
    
    
    

/* [] END OF FILE */
