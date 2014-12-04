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
#ifndef MAG_BASE_FUNCTIONS_H
#define MAG_BASE_FUNCTIONS_H

/***************************************
*        Includes            
***************************************/
#include "cytypes.h" /* For data types? */
#include "CyLib.h" /*For delays? */
#include "I2C.h"

#include <project.h>

/***************************************
*          Global Variables      
***************************************/
extern uint8 Global_ReadBuffer[];
extern uint8 *Global_ReadPtr;
extern uint8 DataInBuffer_Global[];
extern uint8 *DataInPtr_Global;	

/***************************************
*           API Constants        
***************************************/

#define	SLAVE_ADDRESS	0x0E




/***************************************
*        Function Prototypes             
***************************************/    
//Base Functions
uint8 MagWriteMultipleBytes(uint8, uint8*,uint8);
uint8 MagReadMultipleByte(uint8, uint8*,uint8);
uint8 MagReadByte(uint8, uint8*);
uint8 MagWriteByte(uint8, uint8);


/***************************************
*        MANDATORY HEADER CODE(2 of 2)          
***************************************/
#endif
/* [] END OF FILE */
