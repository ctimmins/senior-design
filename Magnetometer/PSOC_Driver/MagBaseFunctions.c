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
#include "MagBaseFunctions.h"
//#include "I2C.h"
//#include "my_command_isr.h"

extern uint8 Global_ReadBuffer[];
extern uint8 *Global_ReadPtr;

extern uint8 DataInBuffer_Global[];
extern uint8 *DataInPtr_Global;


/* 
 * @brief Write a byte to the MAG3110
 * @details Takes a register address that we are going to write to and a byte of datat that we will write to the register
 *
 * @param registerAddress Address of the register on the MAG3110
 * @param data Byte that will be written to the MAG3110
 *
 * @return Error status of the function
 */
uint8 MagWriteByte(uint8 registerAddress, uint8 value)
{
    //functioncomplete_flag = 0;/*set Flag for function Incomplete*/
	I2C_MasterClearReadBuf();
	I2C_MasterClearWriteBuf();
	uint8 i2c_status = I2C_MasterClearStatus();
	//LCD_ClearDisplay();
	LCD_Position(1,7);
	LCD_PrintInt8(i2c_status);
	
	/* Buffer that holds the data to be written */
    uint8 writeBuffer[2] = {registerAddress, value};
    /* Pointer to the buffer that will be written */
    uint8 *writePtr = writeBuffer;
    /* Write to MAG3110 */
    uint8 status = I2C_MasterWriteBuf(SLAVE_ADDRESS, writePtr, 2, I2C_MODE_COMPLETE_XFER);
	/*Wait for transmission to finish*/
	while (I2C_MasterStatus() && !I2C_MSTAT_WR_CMPLT){}
    CyDelay(1);
	//functioncomplete_flag = 1;/*set Flag for function complete*/
	return status;
}

/**
 * @brief Write multiple bytes of data to the MAG3110 in sequential memory
 * @details User supplies the register that the write will start on. User also supplues a buffer of data to write
 *          to the MAG3110 and the size of the buffer as well.
 *
 * @param firstRegister Register the sequential write will start on
 * @param bufferPtr Pointer that points to the buffer of data to write
 * @param size umber of elements that are in the buffer
 * @return Error status of the function
 */
uint8 MagWriteMultipleBytes(uint8 firstRegister, uint8 *bufferPtr, uint8 size)
{
    /* 	Create a new buffer that has that has the firstRegistsre as first element and the other elements as the data pointed to 
		in bufferPtr */
	uint8 i2c_status = I2C_MasterClearStatus();
	//LCD_ClearDisplay();
	LCD_Position(1,7);
	LCD_PrintInt8(i2c_status);
	I2C_MasterClearReadBuf();
	I2C_MasterClearWriteBuf();
	
    /* Pinter to the write buffer that will be written */
    

    /* Set the first element of the buffer equal to the first register we will write to */
    //*(Global_WritePtr) = firstRegister;
    /* Tranfers the data from the buffer that was passed to the function to the buffer that is to be written */
	//uint8 i = 1;
    //for(i=1;i<=size;i++)
	//{
	//	*(Global_WritePtr+i) = bufferPtr[i-1];
    //}
	
	/* Set the first element of the buffer equal to the first register we will write to */
	uint8 j;
	for(j = size;j>0;j--)
	{
		bufferPtr[j] = bufferPtr[j-1];
	}
	bufferPtr[0] = firstRegister;

    /* Write multiple bytes to the MAG3110 */
    uint8 status = I2C_MasterWriteBuf(SLAVE_ADDRESS, bufferPtr, (size+1), I2C_MODE_COMPLETE_XFER);
    while (I2C_MasterStatus() && !I2C_MSTAT_WR_CMPLT){}
    /* Needed because of some bug in the psoc I2C tramission */
    CyDelay(1);
	//functioncomplete_flag = 1;/*set Flag for write completion*/
	return status;
}

/*Requires Delay after call*/
uint8 MagReadByte(uint8 registerAddress, uint8 *readPtr)
{
	/* Pointer to the register address */
    //uint8 *writePtr = &registerAddress;/* changed writeptr to &registerAddress*/
	uint8 i2c_status = I2C_MasterClearStatus();
	//LCD_ClearDisplay();
	LCD_Position(1,7);
	LCD_PrintInt8(i2c_status);
	
	I2C_MasterClearReadBuf();
	I2C_MasterClearWriteBuf();
	
    /* Start the I2C transmission for a read */
    uint8 status = I2C_MasterWriteBuf(SLAVE_ADDRESS, &registerAddress, 1, I2C_MODE_NO_STOP);
    /*wait for the tranmission to finish */
    while (I2C_MasterStatus() && !I2C_MSTAT_WR_CMPLT){}
    /* Needed because of some bug in the psoc I2C tramission */
    CyDelay(1);
	/* read a byte using I2C */
	//return I2C_MasterReadBuf(SLAVE_ADDRESS, readPtr, 1, I2C_MODE_REPEAT_START);
	
	//or TO ENSURE READ IS COMPLETE BEFORE ADDITIONAL CODE EXECUTED
	status |= I2C_MasterReadBuf(SLAVE_ADDRESS,readPtr , 1, I2C_MODE_REPEAT_START);
	while (I2C_MasterStatus() && !I2C_MSTAT_RD_CMPLT){}
    CyDelay(1); //Needed because of some bug in the psoc I2C tramission
	return status;
}

/**
 * @brief Read multiple bytes from the MAG3110
 * @details User supplies the register that we are going to read from and the buffer that we are going to place the data 
 *          that we read from the MAG3110. REQUIRES DELAY AFTER CALL
 *      
 * @param registerAddress Address of the first register we are going to read
 * @param readPtr Pointer to the buffer that the data that we read will be placed  
 * @param size Number of bytes that we are going to read from the MAG3110
 * @return Error status of the function
 */
uint8 MagReadMultipleByte(uint8 registerAddress, uint8 *readPtr,uint8 size)
{
	uint8 i2c_status = I2C_MasterClearStatus();
	LCD_Position(1,7);
	LCD_PrintInt8(i2c_status);
	I2C_MasterClearReadBuf();
	I2C_MasterClearWriteBuf();
	
    /* We start by writng the slave address and the register we are going to start the rea from*/
    uint8 status = I2C_MasterWriteBuf(SLAVE_ADDRESS, &registerAddress, 1, I2C_MODE_NO_STOP);
    /*wait for the tranmission to finish */
    while (I2C_MasterStatus() && !I2C_MSTAT_RD_CMPLT){}
    /* Needed because of some bug in the psoc I2C tramission */
    CyDelay(1);
    
	
	/* read data from the MAG I2C */
	//return I2C_MasterReadBuf(SLAVE_ADDRESS,readPtr , size, I2C_MODE_REPEAT_START);
	//or TO ENSURE READ IS COMPLETE BEFORE ADDITIONAL CODE EXECUTED
	status |= I2C_MasterReadBuf(SLAVE_ADDRESS,readPtr , size, I2C_MODE_REPEAT_START);
	while (I2C_MasterStatus() && !I2C_MSTAT_RD_CMPLT){}
    CyDelay(1); //Needed because of some bug in the psoc I2C tramission
	return status;
}

/* [] END OF FILE */
