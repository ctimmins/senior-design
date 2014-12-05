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
//#include <project.h>
#include "my_command_isr.h"

extern uint8 Command_Received;
extern uint8 MAG_DataRdy_Flag;
extern uint8 IncomingData_Flag;
extern uint8 ReadyForCommand_Flag; // Flag to see if a procedure is already in progress
extern uint8 Command_Buffer;
extern uint8 DataInBuffer_Global[];
extern uint8 *DataInPtr_Global;

CY_ISR(XBeeISR) //Interrupt for recieved data/commands
{
    XBee_UART_ReadRxStatus(); //Clear XBee interrupt flag (If done at start, can another interrupt be triggered??)

	if(ReadyForCommand_Flag)
	{
		ReadyForCommand_Flag = 0; /*Set flag indicating a MATLAB cmd has been received and its procedure is being Executed */
		//Command_Received = XBee_UART_GetByte();
		Command_Received = XBee_UART_GetChar();
		/* doesnt getbyte return uint16, and getchar returns uint8? if using getbyte, may need additional code*/
		
		LED_out_Write(1); // makes LED part of error detection (will stay lit while working on a procedure)
		
		//XBee_UART_ClearRxBuffer(); //Ensure no accumulation of random data
		if(Command_Received == CMD_I_WM_OFFSET_ALL) /* since the cmd is followed by a inbound data transmission... */
		{
			//CyDelay(500);
			//LED_out_Write(0);
			LCD_Position(1,0);
			LCD_PrintString("IDOFF");				
			IncomingData_Flag = 1;
			
			XBee_UART_PutChar(Command_Received);//Sends confirmation TWICE. Once after receiving CMD(in ISR), and again after finishing WRITE(in MAIN)
			// May modify to simply toggle IncomingData_Flag here. Matlab will delay after sending CMD, then send data w/o command confirmation.			
		}
	}
	


	else // If procedure is already in progress... (ReadyForCommand_Flag = 0)
	{
		if(IncomingData_Flag)
		{
			//LED_out_Write(1);
			uint8 i = 0;
			//uint8 timeout_count = 0;
			// WILL LOOP INFINITLY IF DATA NOT RECEIVED!! :/
			while(i < ARRAY_SIZE_MAG_DATA) // DO I NEED TO READ BYTE BY BYTE (LETTING THE ISR RETRIGGER EACH TIME?)
			{
				//if(((XBee_UART_ReadRxStatus())&(XBee_UART_RX_STS_FIFO_NOTEMPTY)) == XBee_UART_RX_STS_FIFO_NOTEMPTY)
				//if((XBee_UART_ReadRxStatus() & XBee_UART_RX_STS_FIFO_NOTEMPTY)==XBee_UART_RX_STS_FIFO_NOTEMPTY)
				uint8 temp_status = XBee_UART_ReadRxStatus();
				temp_status &= 0x20;
				if(temp_status)
				{
					*(DataInPtr_Global+i) = XBee_UART_GetChar(); /* GETCHAR OR GETBYTE??*/
					i +=1;
				}
				
				// if transmission ends up being too slow add the following else statement (needs modification!)
				/*else
				{
					CyDelay(5);
					timeout_count += 1;
					if(timeout_count >= 255)
					{
						//SEND ERROR MSG TO MATLAB
						//XBee_UART_GetByte(); //Clear the byte sent
						XBee_UART_ClearTxBuffer();
						XBee_UART_PutChar(CMD_O_ARRAY_TIMEOUT); //SEND ERROR MSG TO MATLAB
						//Add step to RESET SYSTEM?
						return;
					}
				}*/
			}
			IncomingData_Flag = 0;
		}
		if((MAG_DataRdy_Flag!=0)&&(Command_Buffer==0))
		{
			Command_Buffer = XBee_UART_GetChar();
		}
		else //ERROR: BUSY (If a procedure IS already in progress and NOT waiting for furthur data. [DENY or STORE?] and return BUSY msg to MATLAB)
		{
			// Possibly add a step that stores the CMD and executes after
			LCD_Position(1,0);
			LCD_PrintString("E:BSY");			
			//XBee_UART_ClearTxBuffer();
			XBee_UART_PutChar(CMD_O_BUSY); /* SEND ERROR MSG TO MATLAB*/
			
			//XBee_UART_ClearRxBuffer();
			//I2C_MasterClearReadBuf();
			//I2C_MasterClearWriteBuf();
			//Command_Received = 0;
			//IncomingData_Flag = 0;
			//ReadyForCommand_Flag = 1;
		}	
	}   
} /*End of XBeeISR Code*/


CY_ISR(INT1PinISR) // Only clears interrupt and sets flag. Allows any currently processing CMD to finish.
{
    //clear interrupt flag
    CY_GET_REG8(INT1_Pin__INTSTAT);

    //XBee_UART_PutChar(CMD_O_MAGDATARDY);// CMD_O_MAGDATARDY = 54
    //LED_out_Write(1);
   // CyDelay(1000);
   // LED_out_Write(0);
	
    //set INT1 Flag
	LCD_Position(1,0);
	LCD_PrintString("DRDY1");
	CyDelay(2000);
    MAG_DataRdy_Flag = 1;
}


void InitXBee_Isr(void)
{
    XBee_isr_Start();
    XBee_isr_SetVector(XBeeISR);
}

void InitINT1_Isr(void)
{
    INT1_isr_Start();
    INT1_isr_SetVector(INT1PinISR);
}

/* [] END OF FILE */
