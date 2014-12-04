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
/***************************************
*        Includes            
***************************************/
#include "DriverMAG3110.h"
//#include "my_command_isr.h"

//#include <project.h>

extern uint8 Global_ReadBuffer[];
extern uint8 *Global_ReadPtr;

extern uint8 DataInBuffer_Global[];
extern uint8 *DataInPtr_Global;


/**
 * @brief Clears the global array used by functions
 * @details Iterate through array, overwriting values with Zero.
 * 			
 * @param  Array pointer, Array Size
 */
void ClearGlobalArray(uint8 *Array_Ptr, uint8 array_size)
{
	//functioncomplete_flag2 = 0;/*set Flag for function Incomplete*/
	uint8 i;
	for(i = 0;i>=(array_size+1);i++)/*Added "+1" due to addition of register address in MagBaseFunctions.c code */
	{
		Array_Ptr[i] = 0;
	}
	//functioncomplete_flag2 = 1;/*set Flag for function complete*/
}


/**
 * @brief Write offset correction to the MAG3110
 * @details Write offset correction data to the MAG3110. This is done in order to get more precice output data from 
 * 			MAG3110. Offset correction data does not need to be changed unless there will be magnetic interference
 * 			with the MAG3110 in normal operating conditions
 * 			
 * @param  offsetData that contains the offset values for the X,Y, and Z registers
 * @return Error Status of the function
 */
uint8 WriteOffsetCorrection(uint8 *offsetData) /* [NOTE: Handles 16 bit UNSIGNED VALUES (but each stored as two 8 bit uint8 values(total of six uint bytes). previously ptr value was set for 3 uint16 values)]*/
{
	/* Declare a buffer that will hold the MSB,LSB for the X,Y, and Z registers */
    //uint8 writeData[10] = {0};
	
	//uint8 i;
    //for (i = 0;i < 3; i++)
    //{
    //    Global_WriteBuffer[i] = offsetData[i] >> 8;
    //    Global_WriteBuffer[i + 1] = offsetData[i] << 8;
    //}

    /* Call and return the error status of the multibyte write function */
    return MagWriteMultipleBytes(OFF_X_MSB, offsetData, 6);
}

/**
 * @brief Write a byte to control register 1
 * @details User supplies a byte that will be written to control register 1
 * 
 * @param data Byte that will be written to the control register
 * @return Error status of the function
 */
uint8 WriteCtrlReg1(uint8 value)
{
	/* call the single byte write function */
	return MagWriteByte(CTRL_REG1, value);

}

/**
 * @brief Write a byte to control register 2
 * @details User supplies a byte that will be written to control register 2
 * 
 * @param data Byte that will be written to the control register
 * @return Error status of the function
 */
uint8 WriteCtrlReg2(uint8 value)
{
	/* call the single byte write function */
    return MagWriteByte(CTRL_REG2, value);
}

/**
 * @brief Sets the over sampling and data rate for the MAG3110
 * @details Using the defined constants in the header file the user is able
 * 			to set the over sampling and the data rate for the MAG3110. 
 * 
 * @param CONSTANT Is one of the #defines taht is in the header file under Over-Sampling Ratio and Data Rate Description Constants 
 * @return Error status of the register 
 */
uint8 SetOverSampleAndDataRate(uint8 CONSTANT)
{
	/* Declare the variable tat will hold the current control register 1 state */
	uint8 temp_value = 0;
	/* Use the cotrol register read function to get the current state */
	uint8 status = ReadCtrlReg1(&temp_value);
	
	/* Set the 5 MSB bits to 0. We are going to replace these values with the new over sampling 
		ratio anddta rate bit patern. We preserve the 3 LSB bits */
	temp_value &= 0x07; /* 0x07 = 0b0000 0111 in binary. Anding preservs 3 LSB bits */
	temp_value |= CONSTANT; /* Oring with the CONSTANT set the 5 MSB bits and preservs the 3 LSB bits */

	status |= SetStandbyMode(); 
	//DELAY NEEDED??
	
	/* Write the new control register 1 state to control register 1. Retrun the error status */
	status |= WriteCtrlReg1(temp_value);
	return status;
}

uint8 SetCtrlReg1Default()
{
	uint8 status = SetStandbyMode();
	
	status |= WriteCtrlReg1(CTRL_REG1_DEFAULT);
	return status;
}

uint8 SetStandbyMode()
{
	uint8 temp_value = 0;
	/* Use the cotrol register read function to get the current state */
	uint8 status = ReadCtrlReg1(&temp_value);

	/* Set the 2 LSB bits to 0. We are going to replace these values with the new mode bits. We preserve the 6 MSB bits */
	temp_value &= 0xFC; /* 0xFC = 0b1111 1100 in binary. Anding preservs the 6 MSB bits */
	
	/*Keith Edit - Unneeded OR with zero mask, due to preceeding AND mask. */
	//current |= CTRL_REG1_STANDBY; /* Oring with the CONSTANT set the 2 LSB bits and preservs the 6 MSB bits */

	/* Write the new control register 1 state to control register 1. Retrun the error status */
	status |= WriteCtrlReg1(temp_value);
	return status;
}

uint8 SetContinuousMode()
{
	uint8 temp_value = 0;
	/* Use the cotrol register read function to get the current state */
	uint8 status = SetAutoResetOn();
	status |= ReadCtrlReg1(&temp_value);
	//CyDelay(100);	
	
	/* Set the 2 LSB bits to 0. We are going to replace these values with the new mode bits. We preserve the 6 MSB bits */
	temp_value &= 0xFC; /* 0xFC = 0b1111 1100 in binary. Anding preservs the 6 MSB bits */
	temp_value |= CTRL_REG1_CONTINUOUS; /* Oring with the CONSTANT set the 2 LSB bits and preservs the 6 MSB bits */
	
	//status |= SetStandbyMode();

	/* Write the new control register 1 state to control register 1. Retrun the error status */
	status |= WriteCtrlReg1(temp_value);
	return status;

}

uint8 SetSingleMeasurmentMode()
{
	uint8 temp_value = 0;
	/* Use the cotrol register read function to get the current state */
	uint8 status = ReadCtrlReg1(&temp_value);

	/* Set the 2 LSB bits to 0. We are going to replace these values with the new mode bits. We preserve the 6 MSB bits */
	temp_value &= 0xFC; /* 0xFC = 0b1111 1100 in binary. Anding preservs the 6 MSB bits */
	temp_value |= CTRL_REG1_SINGLE_MEASURMENT; /* Oring with the CONSTANT set the 2 LSB bits and preservs the 6 MSB bits */
	
	//SetStandbyMode(); /*Not required because we are only editing the last 2 bits of the register */
	/* Write the new control register 1 state to control register 1. Retrun the error status */
	status |= WriteCtrlReg1(temp_value);
	return status;
}

uint8 SetTriggerMeasurmentMode()
{
	uint8 temp_value = 0;
	/* Use the cotrol register read function to get the current state */
	uint8 status = ReadCtrlReg1(&temp_value);

	/* Set the 2 LSB bits to 0. We are going to replace these values with the new mode bits. We preserve the 6 MSB bits */
	temp_value &= 0xFC; /* 0xFC = 0b1111 1100 in binary. Anding preservs the 6 MSB bits */
	temp_value |= CTRL_REG1_TRIGGER_MEASURMENT; /* Oring with the CONSTANT set the 2 LSB bits and preservs the 6 MSB bits */
	
	//SetStandbyMode(); /*Not required because we are only editing the last 2 bits of the register */
	/* Write the new control register 1 state to control register 1. Retrun the error status */
	status |= WriteCtrlReg1(temp_value);
	return status;
}

uint8 SetFastReadOn()
{
	uint8 temp_value = 0;
	/* Use the cotrol register read function to get the current state */
	uint8 status = ReadCtrlReg1(&temp_value);

	/* Set the bit 2 to 0. We are going to replace this value with the fast mode bit. We preserve the 7 other bits */
	temp_value &= 0xFB; /* 0xFC = 0b1111 1011 in binary. all bits except for bit 2 */
	temp_value |= CTRL_REG1_FAST_READ_EN; /* Oring with the CONSTANT sets bit 2 to the vaue we want and preservs all other bits */

	status |= SetStandbyMode(); 

	/* Write the new control register 1 state to control register 1. Retrun the error status */
	status |= WriteCtrlReg1(temp_value);
	return status;
}

uint8 SetFastReadOff()
{
	uint8 temp_value = 0;
	/* Use the cotrol register read function to get the current state */
	uint8 status = ReadCtrlReg1(&temp_value);

	/* Set the bit 2 to 0. We are going to replace this value with the fast mode bit. We preserve the 7 other bits */
	temp_value &= 0xFB; /* 0xFC = 0b1111 1011 in binary. all bits except for bit 2 */
	
	/*Keith Edit - Unneeded OR with zero mask, due to preceeding AND mask. */
	//current |= CTRL_REG1_FAST_READ_NEN; /* Oring with the CONSTANT sets bit 2 to the vaue we want and preservs all other bits */
	
	status |= SetStandbyMode(); 

	/* Write the new control register 1 state to control register 1. Retrun the error status */
	status |= WriteCtrlReg1(temp_value);
	return status;
}

uint8 SetCtrlReg2Default()
{
	uint8 status = SetStandbyMode(); 
	status |= WriteCtrlReg2(CTRL_REG2_DEFAULT);
	return status;
}

uint8 SetAutoResetOn()
{
	uint8 temp_value = 0;
	/* Use the cotrol register read function to get the current state */
	uint8 status = ReadCtrlReg1(&temp_value);

	/* Set bit 7 to 0. We are going to replace this value with the auto reset bit to be on. We preserve the 7 other bits */
	temp_value &= 0x7F; /* 0x7F = 0b0111 1111 in binary. all bits except for bit 7 */
	temp_value |= CTRL_REG2_AUTO_MRST_EN; /* Oring with the CONSTANT sets bit 7 to the vaue we want and preservs all other bits */

	status |= SetStandbyMode(); 
	
	//DELAY NEEDED??
	/* Write the new control register 1 state to control register 1. Retrun the error status */
	status |= WriteCtrlReg2(temp_value);
	return status;
}
uint8 SetAutoResetOff	()
{
	uint8 temp_value = 0;
	/* Use the cotrol register read function to get the current state */
	uint8 status = ReadCtrlReg1(&temp_value);

	/* Set bit 7 to 0. We are going to replace this value with the auto reset bit to be off. We preserve the 7 other bits */
	temp_value &= 0x7F; /* 0x7F = 0b0111 1111 in binary. all bits except for bit 7 */
	
	/*Keith Edit - Unneeded OR with zero mask, due to preceeding AND mask. */	
	//current |= CTRL_REG2_AUTO_MRST_NEN; /* Oring with the CONSTANT sets bit 7 to the vaue we want and preservs all other bits */

	status |= SetStandbyMode(); 
	
	/* Write the new control register 1 state to control register 1. Retrun the error status */
	status |= WriteCtrlReg2(temp_value);
	return status;
}

uint8 SetUserCorrectedData()
{
	uint8 temp_value = 0;
	/* Use the cotrol register read function to get the current state */
	uint8 status = ReadCtrlReg1(&temp_value);

	/* Set bit 5 to 0. We are going to replace this value with the user corrected data bit to be on. We preserve the 7 other bits */
	temp_value &= 0xF7; /* 0x7F = 0b1111 0111 in binary. all bits except for bit 7 */
	
	/*Keith Edit - Unneeded OR with zero mask, due to preceeding AND mask. */
	//current |= CTRL_REG2_RAW_NEN; /* Oring with the CONSTANT sets bit 7 to the vaue we want and preservs all other bits */

	status |= SetStandbyMode(); 
	//DELAY NEEDED??
	
	/* Write the new control register 1 state to control register 1. Retrun the error status */
	status |=  WriteCtrlReg2(temp_value);
	return status;
}

uint8 SetRawData()
{
	uint8 temp_value = 0;
	/* Use the cotrol register read function to get the current state */
	uint8 status = ReadCtrlReg1(&temp_value);

	/* Set bit 5 to 0. We are going to replace this value with the raw data bit to be on. We preserve the 7 other bits */
	temp_value &= 0xF7; /* 0x7F = 0b1111 0111 in binary. all bits except for bit 7 */
	temp_value |= CTRL_REG2_RAW_EN; /* Oring with the CONSTANT sets bit 7 to the vaue we want and preservs all other bits */

	status |= SetStandbyMode(); 
	//DELAY NEEDED??
	
	/* Write the new control register 1 state to control register 1. Retrun the error status */
	status |= WriteCtrlReg2(temp_value);
	return status;
}

uint8 ResetMag()
{
	uint8 status = SetStandbyMode(); 
	status |= WriteCtrlReg2(CTRL_REG2_MAG_RST);
	return status;
}

//================END OF WRITE FUNCTIONS=========================



//================START OF READ FUNCTIONS=========================

/**
 * @brief Read the data ready status register
 * @details Use I2C to read the byte of data at the data ready register
 * 
 * @param readPtr Buffer that will store the result
 * @return Error status of the function
 */
uint8 ReadDrStatus(uint8 *readPtr)
{
    
    /* Call the read byte function */
    return MagReadByte(DR_STATUS, readPtr);
}

/**
 * @brief Read the X, Y, and Z data from the MAG3110
 * @details Reads the X, Y, and Z data from the MAG3110 (signed 16-bit values)
 * 
 * @param readPtr Pointer to the buffer that we will place the data read fron the MAG3110
 * @return Error status of the function
 */
uint8 ReadMagData(uint8 *readPtr) /* [NOTE: VALUES HANDLED ARE THREE 16 bit SIGNED VALUES (but stored as two 8 bit uint8 values(total of six bytes) previously ptr value was set for int16]*/
{
    /* Create the read buffer that will stor the MSB, and LSB data read from the MAG3110 */
    //uint8 readBuffer[6] = {0};
    //uint8 *readLocalPtr = readBuffer;

    uint8 status = MagReadMultipleByte(OUT_X_MSB, readPtr,6);
	
	//INSERT DELAY!!!!!!!
	
    /*  This is where we take the 8 bit MSB and LSB and convert them to one 16 bit number */
    //uint8 i;
	//for (i = 0; i <= 2; ++i)
    //{
    //	readPtr[i] = (readBuffer[i * 2] << 8) | readBuffer[i * 2 + 1];
    //}

	//Keith edit: return 0 --> return status 
    /* (This needs to be fix. Return is for the erroe status not 0) */
    return status;
}

/**
 * @brief Read from the who am I register
 * @details Reads the device identification for the MAG3110
 * 
 * @param readPtr Buffer that will store the result
 * @return Error status of the function
 */
uint8 ReadWhoAmI(uint8 *readPtr)
{ 
   	/* Call the single byte read function */
    return MagReadByte(WHO_AM_I, readPtr);
}

/**
 * @brief Read the system mode register
 * @details Reads from the system mode register 
 * 
 * @param readPtr Buffer that will store the result
 * @return Error status of the function
 */
uint8 ReadSysMod(uint8 *readPtr)
{
    /* Call the single byte read function */
    return MagReadByte(SYSMOD, readPtr);
}

/** [NOTE: VALUE RETURNED IS A 8 bit SIGNED VALUE]
 * @brief Find the current temp of the MAG3110 sensor
 * @details Reads the data from the die temp register which represents the current temp of the chip
 * 
 * @param readPtr Buffer that will store the result 
 * @return Error Status of the function
 */
uint8 ReadDieTemp(uint8 *readPtr)
{
    /* Call the single byte read function */
    return MagReadByte(DIE_TEMP, readPtr);
}

/**
 * @brief Read the current byte in control register 1
 * @details Read the current byte that is stored in control register 1 of the MAG3110
 * 
 * @param readPtr Buffer that will store the result 
 * @return Error status of the function
 */
uint8 ReadCtrlReg1(uint8 *readPtr)
{
    /* call the read one byte function */
    return MagReadByte(CTRL_REG1, readPtr);
}

/**
 * @brief Read the current byte in control register 2
 * @details Read the current byte that is stored in control register 1 of the MAG3110
 * 
 * @param readPtr Buffer that will store the result
 * @return Error status of the function
 */
uint8 ReadCtrlReg2(uint8 *readPtr)
{
    /* Call the single byte read function */
    return MagReadByte(CTRL_REG2, readPtr);
}

/* [] END OF FILE */
