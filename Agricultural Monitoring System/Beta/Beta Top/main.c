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
#include <stdio.h>
#include "MLX.h"
#include "utils.h"
#include "stem_isr.h"

//State Definitions.
#define WAKE 0            
#define GETDATA 1
#define TXDATA 2
#define SLEEPMODE 3

//number of levels (depths)
#define NUM_LEVELS 1

//Time asleep ~= 4 seconds
uint8 wakeCycles = 8;

//Vegetronix Definitions
uint8 state;
double voltage[NUM_LEVELS];
uint16 value;
char Veg_out[16];

//Temperature Definitions
double temperature[NUM_LEVELS];
char temp_out[6];
int main()
{
    //Enable custom ISR's
    initAllISR();
    
    //Enable Globals
    CyGlobalIntEnable;    
    
    //INITIALIZE 12C, XBEE, Veg
    Veg_Start();
    Veg_StartConvert();
    Xbee_UART_Start();
    I2C_1_Start();   
    CyDelay(250);
    
    //read EEPROM, config, and oscillator trimming value
    readEEPROM();
    writeOscTrimValue();
    writeConfigValue();
    
    //Initialize PSOC state
    state = WAKE;
    
    //Start Sleep Timer 
    SleepTimer_Start();
    
    
    for(;;)
    {
        /* Place your application code here. */
        if (state == WAKE){
            
            //change state
            state = GETDATA;
        } // end WAKE
        
        else if (state == GETDATA){
            
            //MLX READ------------------------------------------
            //Temperature, compensation, and whole frame read
            uint16 ptat = readPTAT();          
            uint16 comp = readCompensation();
            readFrame();    
            //END MLX READ--------------------------------------

            //VEG READ------------------------------------------
            //Get and scale ADC
            int depth;
            for (depth = 0; depth < NUM_LEVELS; depth++) {
                voltage[depth] = Veg_GetResult16(0)*(3.3/4096);
                temperature[depth] = 22.3;
            }
            
            
            
            //END VEG READ--------------------------------------
            
            //change state
            state = TXDATA;
        } // end GETDATA
        
        else if (state == TXDATA){
            
            //TX MLX
            //tx whole frame, PTAT, and compensation
            XBeeTxFrame();
            XBeeTxPTAT();
            XBeeTxCompensation();
            //END TX MLX----------------------------------------
            
            //TX VEG--------------------------------------------
            char level[4];
            int depth;
            //string for calculating Checksum
            char toCksm[56];
            Xbee_UART_PutString("V,");
            
            for (depth = 0; depth < NUM_LEVELS; depth++) {
                sprintf(level, "%d:", depth);
                sprintf(Veg_out, "%1.4f:", voltage[depth]);
                //Add hard coded temperature value
                sprintf(temp_out, "%2.1f,", temperature[depth]);
                //if first level, copy string, otherwise concatenate
                if (depth == 0) {
                    strcpy(toCksm, level);    
                }
                else {
                    strcat(toCksm, level);
                }
                
                strcat(toCksm, Veg_out);
                strcat(toCksm, temp_out);
                
                //Transmit level string
                Xbee_UART_PutString(level);
                Xbee_UART_PutString(Veg_out);
                Xbee_UART_PutString(temp_out);
            }       //end for
            
            //Transmit Checksum for Vegetronix
             Xbee_UART_PutChar(calculateCheckSum(toCksm, strlen(toCksm)));
            
            //END TX VEG----------------------------------------
            
            //Change State
            state = SLEEPMODE;
        } // end TXDATA
        
        else if (state == SLEEPMODE){
            //put her to sleep
            //CyDelay(2000);
            if (wake_count % wakeCycles == 0) {
                //Change State
                state = WAKE;
                continue;
            }
            else {
                sleepPsoc();   
            }
            
            
        } // END SLEEPM0DE
        
        else{ // Error in state machine, to be filled out. Likely a reset will occur here. 
            
            //Change State
            state = WAKE;
        } // End Error Catch
        
    }
}

/* [] END OF FILE */