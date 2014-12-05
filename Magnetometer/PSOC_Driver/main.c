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
#include <project.h>
//#include "my_command_isr.h"
#include "my_command_isr.h"

//#include "DriverMAG3110.h"

//uint8 DataReady = 0xFF; /* Signal from PSOC to MATLAB when INT1 goes high */
uint8 Command_Received;
uint8 ReadyForCommand_Flag; /*If ready to receive new CMD, set to 1. else, set to 0.*/
uint8 IncomingData_Flag;
uint8 StatusError_Flag; /* Maybe unecessary since if status!=0, readyforcmd flag will never be raised and will prevent further cmds*/
uint8 MAG_DataRdy_Flag;
uint8 Global_ReadBuffer[8] = {0};
uint8 *Global_ReadPtr = Global_ReadBuffer;
uint8 DataInBuffer_Global[8] = {0};
uint8 *DataInPtr_Global = DataInBuffer_Global;
//uint8 WaitForDataRead_Flag;
uint8 Command_Buffer;
//#define CMD_I_RM_MAGDATA_D		(0x34u)


int main()
{    
	CyGlobalIntEnable; /* Uncomment this line to enable global interrupts. */
	XBee_UART_Start();
	I2C_Start();
	LCD_Start();
	
	//	INITIALIZE VALUES
	Command_Received = 0;
	MAG_DataRdy_Flag = 0;
	ReadyForCommand_Flag = 1;
	IncomingData_Flag = 0;
	StatusError_Flag = 0;
	//WaitForDataRead_Flag = 0;
	Command_Buffer = 0;
	
	// INITIALIZE ISRs
	InitXBee_Isr();
	//InitINT1_Isr(); // IF INT1 TRIGGERS WHEN NOT IN ACTIVE MODE, MOVE INITIATION CODE FOR SETTING ACTIVE.
	//INT1_isr_Start();
	
	XBee_UART_ClearRxBuffer();
	XBee_UART_ClearTxBuffer();
	CyDelay(2000);
	LCD_ClearDisplay();
	LCD_Position(0,0);
	LCD_PrintString("MAG DRIVER:");
	LCD_Position(1,0);
	LCD_PrintString("[ERR]");
	LCD_Position(1,7);
	LCD_PrintString("[I2C]");

	uint8 status = 0;
	status = SetCtrlReg1Default();
	status |= SetCtrlReg2Default();
	if(status !=0)
	{
		LCD_ClearDisplay();
		LCD_Position(0,0);
		LCD_PrintInt8(status);
		status = 0;
	}
	uint8 pin_status = 0;
	    
    for(;;)
    {
		pin_status = INT1_Pin_Read();
		pin_status |= MAG_DataRdy_Flag;
		if(pin_status)
		{
			MAG_DataRdy_Flag = pin_status;
			if(ReadyForCommand_Flag!=0)
			{
				Command_Received = CMD_I_RM_MAGDATA;
				ReadyForCommand_Flag = 0;
			}	
		}

		if(Command_Received != 0)
		{
			LCD_ClearDisplay();
			//XBee_UART_ClearTxBuffer();
			//XBee_UART_ClearRxBuffer();			
			switch (Command_Received){
				case CMD_I_RM_MAGDATA: // CMD_I_RM_MAGDATA = 34
				{
					status = ReadMagData(Global_ReadPtr);
					if(status == 0)
					{
						CyDelay(500);
						XBee_UART_PutChar(Command_Received);
						//LED_out_Write(0);
						CyDelay(500);
						XBee_UART_PutArray(Global_ReadPtr,ARRAY_SIZE_MAG_DATA);
						CyDelay(1000);					
						//WaitForDataRead_Flag = 0;
						MAG_DataRdy_Flag = 0;
						if(Command_Buffer!=0)
						{
							Command_Received = Command_Buffer;
							Command_Buffer = 0;
							//ReadyForCommand_Flag = 0; //Dont toggle flag
						}
						else
						{
							Command_Received = 0;
							ReadyForCommand_Flag = 1;
							LED_out_Write(0);
						}
					}					
					else 
					{
						// will result is a halt of CMD processing and leaves LED lit (visual error signal)
						LCD_Position(1,0);
						LCD_PrintString("E:STA");
						CyDelay(1000);		
						StatusError_Flag = 1;						
					}
				}
				break;
				
				case CMD_I_WM_OFFSET_ALL:// CMD_I_WM_OFFSET_ALL = 1
				{
					if(IncomingData_Flag == 0) // IF THE OFFSET DATA TO WRITE HAS BEEN RECEIVED...
					{
						status = WriteOffsetCorrection(DataInPtr_Global);
						if(status == 0)
						{
							CyDelay(500);
							XBee_UART_PutChar(Command_Received); //Sends confirmation TWICE. Once after receiving CMD, and again after finishing WRITE
							LED_out_Write(0);
							Command_Received = 0;
							IncomingData_Flag = 0;
							ReadyForCommand_Flag = 1;
						}					
						else 
						{
							// will result is a halt of CMD processing and leaves LED lit (visual error signal)
							LCD_Position(1,0);
							LCD_PrintString("E:STA");
							CyDelay(1000);		
							StatusError_Flag = 1;
						}
					}
				}
				break;
				
				case CMD_I_RS_CTRL1://CMD_I_RS_CTRL1=35
				{
					status = ReadCtrlReg1(Global_ReadPtr);
					if(status == 0)
					{
						CyDelay(500);
						XBee_UART_PutChar(Command_Received);
						LED_out_Write(0);
						CyDelay(500);
						XBee_UART_PutChar(Global_ReadBuffer[0]); // Send Read Value
						Command_Received = 0;
						IncomingData_Flag = 0;
						ReadyForCommand_Flag = 1;
					}					
					else 
					{
						// will result is a halt of CMD processing and leaves LED lit (visual error signal)
						LCD_Position(1,0);
						LCD_PrintString("E:STA");
						CyDelay(1000);		
						StatusError_Flag = 1;
					}				
				}
				break;
				
				case CMD_I_RS_CTRL2://CMD_I_RS_CTRL2 = 36
				{
					status = ReadCtrlReg2(Global_ReadPtr);
					if(status == 0)
					{
						CyDelay(500);
						XBee_UART_PutChar(Command_Received);
						LED_out_Write(0);
						CyDelay(500);
						XBee_UART_PutChar(Global_ReadBuffer[0]); // Send Read Value
						Command_Received = 0;
						IncomingData_Flag = 0;
						ReadyForCommand_Flag = 1;
					}					
					else 
					{
						// will result is a halt of CMD processing and leaves LED lit (visual error signal)
						LCD_Position(1,0);
						LCD_PrintString("E:STA");
						CyDelay(1000);		
						StatusError_Flag = 1;
					}	
				}
				break;
				
				case CMD_I_RS_DRSTATUS://CMD_I_RS_DRSTATUS = 37
				{
					status = ReadDrStatus(Global_ReadPtr);
					if(status == 0)
					{
						CyDelay(500);
						XBee_UART_PutChar(Command_Received);
						LED_out_Write(0);
						CyDelay(500);
						XBee_UART_PutChar(Global_ReadBuffer[0]); // Send Read Value
						Command_Received = 0;
						IncomingData_Flag = 0;
						ReadyForCommand_Flag = 1;
					}					
					else 
					{
						// will result is a halt of CMD processing and leaves LED lit (visual error signal)
						LCD_Position(1,0);
						LCD_PrintString("E:STA");
						CyDelay(1000);		
						StatusError_Flag = 1;
					}	
				}
				break;
				
				case CMD_I_RS_SYSMOD:// CMD_I_RS_SYSMOD = 38
				{
					status = ReadSysMod(Global_ReadPtr);
					if(status == 0)
					{
						CyDelay(500);
						XBee_UART_PutChar(Command_Received);
						LED_out_Write(0);
						CyDelay(500);
						XBee_UART_PutChar(Global_ReadBuffer[0]);
						Command_Received = 0;
						IncomingData_Flag = 0;
						ReadyForCommand_Flag = 1;
					}					
					else 
					{
						// will result is a halt of CMD processing and leaves LED lit (visual error signal)
						LCD_Position(1,0);
						LCD_PrintString("E:STA");
						CyDelay(1000);		
						StatusError_Flag = 1;
					}
				}
				break;
				
				case CMD_I_RS_DIETEMP: // CMD_I_RS_DIETEMP = 39
				{
					status = ReadDieTemp(Global_ReadPtr);
					if(status == 0)
					{
						CyDelay(500);
						XBee_UART_PutChar(Command_Received);
						LED_out_Write(0);
						CyDelay(500);
						XBee_UART_PutChar(Global_ReadBuffer[0]);
						Command_Received = 0;
						IncomingData_Flag = 0;
						ReadyForCommand_Flag = 1;
					}					
					else 
					{
						// will result is a halt of CMD processing and leaves LED lit (visual error signal)
						LCD_Position(1,0);
						LCD_PrintString("E:STA");
						CyDelay(1000);		
						StatusError_Flag = 1;
					}
				}
				break;
				
				case CMD_I_RS_WHOAMI: // CMD_I_RS_WHOAMI = 40
				{
					status = ReadWhoAmI(Global_ReadPtr);
					if(status == 0)
					{
						CyDelay(500);
						XBee_UART_PutChar(Command_Received);
						LED_out_Write(0);
						CyDelay(500);
						XBee_UART_PutChar(Global_ReadBuffer[0]);
						Command_Received = 0;
						IncomingData_Flag = 0;
						ReadyForCommand_Flag = 1;
					}					
					else 
					{
						// will result is a halt of CMD processing and leaves LED lit (visual error signal)
						LCD_Position(1,0);
						LCD_PrintString("E:STA");
						CyDelay(1000);		
						StatusError_Flag = 1;
					}
				}
				break;
				
				case CMD_I_WS_CTRL1_DEFAULT: // CMD_I_WS_CTRL1_DEFAULT = 41
				{
					status = SetCtrlReg1Default();
					if(status == 0) //if the write was a success
					{
						CyDelay(500);
						XBee_UART_PutChar(Command_Received);//Returns command received from MATLAB as confirmation
						LED_out_Write(0); //Turns off LED
						Command_Received = 0; //Clears the Command
						IncomingData_Flag = 0;
						ReadyForCommand_Flag = 1; // Sets state as READY for next command
					}					
					else 
					{
						// will result is a halt of CMD processing and leaves LED lit (visual error signal)
						LCD_Position(1,0);
						LCD_PrintString("E:STA");
						CyDelay(1000);		
						StatusError_Flag = 1;
					}
				}
				break;
				
				case CMD_I_WS_CTRL1_MODSTANDBY: // CMD_I_WS_CTRL1_MODSTANDBY = 42
				{
					status = SetStandbyMode();
					if(status == 0) //if the write was a success
					{
						CyDelay(500);
						XBee_UART_PutChar(Command_Received);//Returns command received from MATLAB as confirmation
						LED_out_Write(0); //Turns off LED
						Command_Received = 0; //Clears the Command
						IncomingData_Flag = 0;
						ReadyForCommand_Flag = 1; // Sets state as READY for next command
					}					
					else 
					{
						// will result is a halt of CMD processing and leaves LED lit (visual error signal)
						LCD_Position(1,0);
						LCD_PrintString("E:STA");
						CyDelay(1000);		
						StatusError_Flag = 1;
					}
				}
				break;
				
				case CMD_I_WS_CTRL1_MODSINGLE: // CMD_I_WS_CTRL1_MODSINGLE = 43
				{
					status = SetSingleMeasurmentMode();
					if(status == 0) //if the write was a success
					{
						CyDelay(500);
						XBee_UART_PutChar(Command_Received);//Returns command received from MATLAB as confirmation
						LED_out_Write(0); //Turns off LED
						Command_Received = 0; //Clears the Command
						IncomingData_Flag = 0;
						ReadyForCommand_Flag = 1; // Sets state as READY for next command
					}					
					else 
					{
						// will result is a halt of CMD processing and leaves LED lit (visual error signal)
						LCD_Position(1,0);
						LCD_PrintString("E:STA");
						CyDelay(1000);		
						StatusError_Flag = 1;
					}
				}
				break;
				
				//IF INT1 CONTINUES TO TRIGGER WHEN NOT IT ACTIVE MODE, DEACTIVATE/ACTIVATE ISR WHEN CHANGING MODES				
				case CMD_I_WS_CTRL1_MODACTIVE: // CMD_I_WS_CTRL1_MODACTIVE = 44
				{
					status = SetContinuousMode();
					if(status == 0) //if the write was a success
					{
						CyDelay(500);
						XBee_UART_PutChar(Command_Received);//Returns command received from MATLAB as confirmation
						LED_out_Write(0); //Turns off LED
						Command_Received = 0; //Clears the Command
						IncomingData_Flag = 0;
						ReadyForCommand_Flag = 1; // Sets state as READY for next command
					}					
					else 
					{
						// will result is a halt of CMD processing and leaves LED lit (visual error signal)
						LCD_Position(1,0);
						LCD_PrintString("E:STA");
						CyDelay(1000);		
						StatusError_Flag = 1;
					}
				}
				break;
				
				case CMD_I_WS_CTRL1_MODTRIGGER: // CMD_I_WS_CTRL1_MODTRIGGER = 45
				{
					status = SetTriggerMeasurmentMode();
					if(status == 0) //if the write was a success
					{
						CyDelay(500);
						XBee_UART_PutChar(Command_Received);//Returns command received from MATLAB as confirmation
						LED_out_Write(0); //Turns off LED
						Command_Received = 0; //Clears the Command
						IncomingData_Flag = 0;
						ReadyForCommand_Flag = 1; // Sets state as READY for next command
					}					
					else 
					{
						// will result is a halt of CMD processing and leaves LED lit (visual error signal)
						LCD_Position(1,0);
						LCD_PrintString("E:STA");
						CyDelay(1000);		
						StatusError_Flag = 1;
					}
				}
				break;
				
				case CMD_I_WS_CTRL1_ENFAST: // CMD_I_WS_CTRL1_ENFAST = 46
				{
					status = SetFastReadOn();
					if(status == 0) //if the write was a success
					{
						CyDelay(500);
						XBee_UART_PutChar(Command_Received);//Returns command received from MATLAB as confirmation
						LED_out_Write(0); //Turns off LED
						Command_Received = 0; //Clears the Command
						IncomingData_Flag = 0;
						ReadyForCommand_Flag = 1; // Sets state as READY for next command
					}					
					else 
					{
						// will result is a halt of CMD processing and leaves LED lit (visual error signal)
						LCD_Position(1,0);
						LCD_PrintString("E:STA");
						CyDelay(1000);		
						StatusError_Flag = 1;
					}
				}
				break;
				
				case CMD_I_WS_CTRL1_NENFAST: //CMD_I_WS_CTRL1_NENFAST = 47
				{
					status = SetFastReadOff();
					if(status == 0) //if the write was a success
					{
						CyDelay(500);
						XBee_UART_PutChar(Command_Received);//Returns command received from MATLAB as confirmation
						LED_out_Write(0); //Turns off LED
						Command_Received = 0; //Clears the Command
						IncomingData_Flag = 0;
						ReadyForCommand_Flag = 1; // Sets state as READY for next command
					}					
					else 
					{
						// will result is a halt of CMD processing and leaves LED lit (visual error signal)
						LCD_Position(1,0);
						LCD_PrintString("E:STA");
						CyDelay(1000);		
						StatusError_Flag = 1;
					}
				}
				break;
				
				case CMD_I_WS_CTRL2_DEFAULT: //CMD_I_WS_CTRL2_DEFAULT = 48
				{
					status = SetCtrlReg2Default();
					if(status == 0) //if the write was a success
					{
						CyDelay(500);
						XBee_UART_PutChar(Command_Received);//Returns command received from MATLAB as confirmation
						LED_out_Write(0); //Turns off LED
						Command_Received = 0; //Clears the Command
						IncomingData_Flag = 0;
						ReadyForCommand_Flag = 1; // Sets state as READY for next command
					}					
					else 
					{
						// will result is a halt of CMD processing and leaves LED lit (visual error signal)
						LCD_Position(1,0);
						LCD_PrintString("E:STA");
						CyDelay(1000);		
						StatusError_Flag = 1;
					}
				}
				break;
				
				case CMD_I_WS_CTRL2_ENAUTORESET: // CMD_I_WS_CTRL2_ENAUTORESET = 49
				{
					status = SetAutoResetOn();
					if(status == 0) //if the write was a success
					{
						CyDelay(500);
						XBee_UART_PutChar(Command_Received);//Returns command received from MATLAB as confirmation
						LED_out_Write(0); //Turns off LED
						Command_Received = 0; //Clears the Command
						IncomingData_Flag = 0;
						ReadyForCommand_Flag = 1; // Sets state as READY for next command
					}					
					else 
					{
						// will result is a halt of CMD processing and leaves LED lit (visual error signal)
						LCD_Position(1,0);
						LCD_PrintString("E:STA");
						CyDelay(1000);		
						StatusError_Flag = 1;
					}
				}
				break;
				
				case CMD_I_WS_CTRL2_NENAUTORESET: // CMD_I_WS_CTRL2_NENAUTORESET = 50
				{
					status = SetAutoResetOff();
					if(status == 0) //if the write was a success
					{
						CyDelay(500);
						XBee_UART_PutChar(Command_Received);//Returns command received from MATLAB as confirmation
						LED_out_Write(0); //Turns off LED
						Command_Received = 0; //Clears the Command
						IncomingData_Flag = 0;
						ReadyForCommand_Flag = 1; // Sets state as READY for next command
					}					
					else 
					{
						// will result is a halt of CMD processing and leaves LED lit (visual error signal)
						LCD_Position(1,0);
						LCD_PrintString("E:STA");
						CyDelay(1000);		
						StatusError_Flag = 1;
					}
				}
				break;
				
				case CMD_I_WS_CTRL2_ENUSEROFFSET: // CMD_I_WS_CTRL2_ENUSEROFFSET = 51
				{
					status = SetUserCorrectedData();
					if(status == 0) //if the write was a success
					{
						CyDelay(500);
						XBee_UART_PutChar(Command_Received);//Returns command received from MATLAB as confirmation
						LED_out_Write(0); //Turns off LED
						Command_Received = 0; //Clears the Command
						IncomingData_Flag = 0;
						ReadyForCommand_Flag = 1; // Sets state as READY for next command
					}					
					else 
					{
						// will result is a halt of CMD processing and leaves LED lit (visual error signal)
						LCD_Position(1,0);
						LCD_PrintString("E:STA");
						CyDelay(1000);		
						StatusError_Flag = 1;						
					}
				}
				break;
				
				case CMD_I_WS_CTRL2_NENUSEROFFSET: // CMD_I_WS_CTRL2_NENUSEROFFSET = 52
				{
					status = SetRawData();
					if(status == 0) //if the write was a success
					{
						CyDelay(500);
						XBee_UART_PutChar(Command_Received);//Returns command received from MATLAB as confirmation
						LED_out_Write(0); //Turns off LED
						Command_Received = 0; //Clears the Command
						IncomingData_Flag = 0;
						ReadyForCommand_Flag = 1; // Sets state as READY for next command
					}					
					else 
					{
						// will result is a halt of CMD processing and leaves LED lit (visual error signal)
						StatusError_Flag = 1;
						LCD_Position(1,0);
						LCD_PrintString("E:STA");
						CyDelay(1000);		
						StatusError_Flag = 1;						
					}
				}
				break;				
				
				case CMD_I_WS_CTRL2_RESETMAG: // CMD_I_WS_CTRL2_RESETMAG = 53
				{
					status = ResetMag();
					if(status == 0) //if the write was a success
					{
						CyDelay(1000); //LET MAGNETOMETER RESET PROCEDURE FINISH
						XBee_UART_PutChar(Command_Received);//Returns command received from MATLAB as confirmation
						LED_out_Write(0); //Turns off LED
						Command_Received = 0; //Clears the Command
						IncomingData_Flag = 0;
						ReadyForCommand_Flag = 1; // Sets state as READY for next command
					}					
					else 
					{
						// will result is a halt of CMD processing and leaves LED lit (visual error signal)
						//LCD_ClearDisplay();
						LCD_Position(1,0);
						LCD_PrintString("E:STA");
						CyDelay(1000);		
						StatusError_Flag = 1;
					}
				}
				break;
				
				case CMD_I_RESET_ALL://54
				{
					// RESET I2C
					uint8 i2c_status = I2C_MasterClearStatus();
					//LCD_ClearDisplay();
					LCD_Position(1,7);
					LCD_PrintInt8(i2c_status);
					
					I2C_MasterClearReadBuf();
					I2C_MasterClearWriteBuf();
					
					// Reset MAG CTRL REGISTERS
					status = SetCtrlReg1Default();
					status |= SetCtrlReg2Default();
					if(status == 0) //if the write was a success
					{
						CyDelay(500);
						XBee_UART_PutChar(Command_Received);//Returns command received from MATLAB as confirmation
						LED_out_Write(0); //Turns off LED
						Command_Received = 0; //Clears the Command
						IncomingData_Flag = 0;
						ReadyForCommand_Flag = 1; // Sets state as READY for next command
					}
					else 
					{
						// will result is a halt of CMD processing and leaves LED lit (visual error signal)
						//LCD_ClearDisplay();
						LCD_Position(1,0);
						LCD_PrintString("E:STA");
						CyDelay(1000);		
						StatusError_Flag = 1;
					}					
					
				}
				break;
				
				default: //handles Set Sampling/Data rate CMDs, and Out of range errors
				{
					if((Command_Received >= (STARTOFRANGE_SET_SAMPLING_AND_RATE))&&(Command_Received < (STARTOFRANGE_SET_SAMPLING_AND_RATE + RANGESIZE_SET_SAMPLING_AND_RATE)))
					{
						uint8 offset = Command_Received-STARTOFRANGE_SET_SAMPLING_AND_RATE;
						offset *=DELTAVALS_SET_SAMP_AND_RATE;
						status = SetOverSampleAndDataRate(offset);
						if(status == 0) //if the write was a success
						{
							CyDelay(500);
							XBee_UART_PutChar(Command_Received);//Returns command received from MATLAB as confirmation
							LED_out_Write(0); //Turns off LED
							Command_Received = 0; //Clears the Command
							IncomingData_Flag = 0;
							ReadyForCommand_Flag = 1; // Sets state as READY for next command
						}		
						else 
						{
							// will result is a halt of CMD processing and leaves LED lit (visual error signal)
							StatusError_Flag = 1;
							//LCD_ClearDisplay();
							LCD_Position(1,0);
							LCD_PrintString("E:STA");
							CyDelay(1000);
						}
					}
					else // ERROR: CMD VALUE OUT OF RANGE (executed if not a CMD to set sampling/data rate)
					{
						//CLEAR EVERYTHING
						XBee_UART_ClearTxBuffer();
						XBee_UART_ClearRxBuffer();
						I2C_MasterClearReadBuf();
						I2C_MasterClearWriteBuf();

						Command_Received = 0; //Clears the Command
						IncomingData_Flag = 0;
						ReadyForCommand_Flag = 1; // Sets state as READY for next command						
						XBee_UART_PutChar(CMD_O_CMDVALUEOUTOFRANGE); //Send error msg
						//LCD_ClearDisplay();
						LCD_Position(1,0);
						LCD_PrintString("E:RAN");
						CyDelay(1000);
						LED_out_Write(0); //Turns off LED
					}
				}			
			} //END OF SWITCH-CASE
		}//end of IF statement encasing switch-case
		
		/*else // if(Command_Received == 0)
		{
			if((MAG_DataRdy_Flag==1) && (WaitForDataRead_Flag==0))
			{
				//May change CMD_O_MAGDATARDY to simply be the same as CMD_I_RM_MAGDATA.
				//XBee_UART_ClearTxBuffer();
				//LCD_ClearDisplay();
				LCD_Position(1,0);
				LCD_PrintString("DRDY2");
				XBee_UART_PutChar(CMD_O_MAGDATARDY);// CMD_O_MAGDATARDY = 55
				WaitForDataRead_Flag = 1; //Prevents constant resending of notification to MATLAB
				INT1_isr_Disable();
			}
		}*/
	}//END OF FOR LOOP	
		
}//END OF MAIN

/* [] END OF FILE */
