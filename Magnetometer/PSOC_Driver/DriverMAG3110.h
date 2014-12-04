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
*        MANDATORY HEADER CODE (1 of 2)        
***************************************/
#ifndef DRIVER_MAG3110_H
#define DRIVER_MAG3110_H

/***************************************
*        Includes            
***************************************/

#include "MagBaseFunctions.h" 


//#include <project.h>

/***************************************
*        API Constants    
***************************************/ 	
/* Device Slave Address */
#define SLAVE_ADDRESS   0x0E
	
/* Register Addresses */
#define DR_STATUS   	0x00
#define OUT_X_MSB   	0x01
#define OUT_X_LSB   	0x02
#define OUT_Y_MSB   	0x03
#define OUT_Y_LSB   	0x04
#define OUT_Z_MSB   	0x05
#define OUT_Z_LSB   	0x06
#define WHO_AM_I    	0x07
#define SYSMOD      	0x08
#define OFF_X_MSB   	0x09
#define OFF_X_LSB   	0x0A
#define OFF_Y_MSB   	0x0B
#define OFF_Y_LSB   	0x0C
#define OFF_Z_MSB   	0x0D
#define OFF_Z_LSB 		0x0E
#define DIE_TEMP		0x0F
#define CTRL_REG1 		0x10
#define CTRL_REG2 		0x11


/***************************************
*        Control Register Modes          
***************************************/  
/* Control Register 1 */
/*  When implementing functions that alter this register
    so that the setting we want to change are the only ones changed.
    This is done by using the ReadCtrlReg2  and the & functions along with
    the #define that te user wants to change. This way the seetings that
    they dont want to change will not be altered.
*/

/* Over-Sampling Ratio and Data Rate Description Constants */
/*  The folling consants are in the form
    CTR_REG1_V_W_X_Y_Z

    V = Output Rate         wx.yz   ( Hz)
    W = Over Sample Ratio   xyz
    X = ADC Rate            wxyz    (Hz)
    Y = Current Typ         wxy.z   (uA)
    Z = Noise Typ           x.yz    (uT rms)

    All follow the format above.
*/

#define CTRL_REG1_8000_016_1280_9000_040	0x00
#define CTRL_REG1_4000_032_1280_9000_035	0x08
#define CTRL_REG1_2000_064_1280_9000_030	0x10
#define CTRL_REG1_1000_128_1280_9000_025	0x18

#define CTRL_REG1_4000_016_0640_5500_040	0x20
#define CTRL_REG1_2000_032_0640_5500_035	0x28
#define CTRL_REG1_1000_064_0640_5500_030	0x30
#define CTRL_REG1_0500_128_0640_5500_025	0x38

#define CTRL_REG1_2000_016_0320_2750_040	0x40
#define CTRL_REG1_1000_032_0320_2750_035	0x48
#define CTRL_REG1_0500_064_0320_2750_030	0x50
#define CTRL_REG1_0250_128_0320_2750_025	0x58

#define CTRL_REG1_1000_016_0128_1375_040	0x60
#define CTRL_REG1_0500_032_0128_1375_035	0x68
#define CTRL_REG1_0250_064_0128_1375_030	0x70
#define CTRL_REG1_0125_128_0128_1375_025	0x78

#define CTRL_REG1_0500_016_0080_0688_040	0x80
#define CTRL_REG1_0250_032_0080_0688_035	0x88
#define CTRL_REG1_0125_064_0080_0688_030	0x90
#define CTRL_REG1_0063_128_0080_0688_025	0x98

#define CTRL_REG1_0250_016_0080_0344_040	0xA0
#define CTRL_REG1_0125_032_0080_0344_035	0xA8
#define CTRL_REG1_0063_064_0080_0344_030	0xB0
#define CTRL_REG1_0031_128_0080_0344_025	0xB8

#define CTRL_REG1_0125_016_0080_0172_040	0xC0
#define CTRL_REG1_0063_032_0080_0172_035	0xC8
#define CTRL_REG1_0031_064_0080_0172_030	0xD0
#define CTRL_REG1_0016_128_0080_0172_025	0xD8

#define CTRL_REG1_0063_016_0080_0086_040	0xE0
#define CTRL_REG1_0031_032_0080_0086_035	0xE8
#define CTRL_REG1_0016_064_0080_0086_030	0xF0
#define CTRL_REG1_0008_128_0080_0086_025	0xF8
	
	#define DELTAVALS_SET_SAMP_AND_RATE		(0x08u) /*Used in MAIN */

//===============
	
#define CTRL_REG1_FAST_READ_EN 				0x04
#define CTRL_REG1_FAST_READ_NEN				0x00 /* Keith Edit - Unneeded OR with zero mask due to preceeding AND mask. */

#define CTRL_REG1_DEFAULT					0x00 
#define CTRL_REG1_STANDBY 					0x00 /* Keith Edit - Unneeded OR with zero mask due to preceeding AND mask. */
#define CTRL_REG1_CONTINUOUS				0x01
#define CTRL_REG1_SINGLE_MEASURMENT			0x02
#define CTRL_REG1_TRIGGER_MEASURMENT		0x03

	
/* Control Register 2 */
/*  When implementing functions that alter this register
    so that the setting we want to change are the only ones changed.
    This is done by using the ReadCtrlReg2  and the & functions along with
    the #define that te user wants to change. This way the seetings that
    they dont want to change will not be altered.
*/
#define CTRL_REG2_DEFAULT       		0x00
#define CTRL_REG2_AUTO_MRST_EN  		0x80
#define CTRL_REG2_AUTO_MRST_NEN         0x00 /* Keith Edit - Unneeded OR with zero mask due to preceeding AND mask. */
#define CTRL_REG2_RAW_NEN       		0x00 /* Keith Edit - Unneeded OR with zero mask due to preceeding AND mask. */
#define CTRL_REG2_RAW_EN        		0x20
#define CTRL_REG2_MAG_RST       		0x10

	
/***************************************
*        Function Prototypes             
***************************************/  
	
/* Write Functions */
uint8 WriteOffsetCorrection	(uint8*);/* [NOTE: Handles 16 bit UNSIGNED VALUES (but stored as (6 times)two 8 bit uint8 values. previously ptr value was set for 3 uint16 values)]*/
uint8 WriteCtrlReg1		(uint8);
uint8 WriteCtrlReg2		(uint8);

/* Set Functions */
uint8 SetOverSampleAndDataRate(uint8);
uint8 SetStandbyMode		(); 
uint8 SetCtrlReg1Default	();					
uint8 SetContinuousMode	(); 			
uint8 SetSingleMeasurmentMode	();
uint8 SetTriggerMeasurmentMode();			
uint8 SetFastReadOn		();
uint8 SetFastReadOff		();		
uint8 SetCtrlReg2Default	();
uint8 SetAutoResetOn		();
uint8 SetAutoResetOff		();
uint8 SetUserCorrectedData	();
uint8 SetRawData		();
uint8 ResetMag		();

/* Read Functions */
uint8 ReadDrStatus    	(uint8 *);
uint8 ReadMagData    (uint8 *); /* [NOTE: VALUES HANDLED ARE THREE 16 bit SIGNED VALUES (but stored as two 8 bit uint8 values(total of six bytes) previously ptr value was set for int16]*/
uint8 ReadWhoAmI	(uint8 *);
uint8 ReadSysMod      (uint8 *);
uint8 ReadDieTemp    	(uint8 *); 	/* [NOTE: VALUE HANDLED IS A 8 bit SIGNED VALUE (but stored as uint8)] Keith Edit - ReadDieTep --> ReadDieTemp. (changed in code as well) */
uint8 ReadCtrlReg1    (uint8 *);
uint8 ReadCtrlReg2    (uint8 *);

/* Other Functions */
void ClearGlobalArray(uint8*, uint8);

/***************************************
*        MANDATORY HEADER CODE (1 of 2)        
***************************************/
#endif





/* [] END OF FILE */
