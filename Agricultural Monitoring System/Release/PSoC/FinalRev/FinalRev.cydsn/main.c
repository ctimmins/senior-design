/* ========================================
 *
 * Something witty goes here
 *
 * ========================================
*/

#include <project.h>
#include <stdio.h>
#include "MLX.h"
#include "utils.h"
#include "stem_isr.h"

//State Definitions.
#define WAKE          0            
#define GETDATA       1
#define TXDATA        2
#define WAIT_FOR_CMD  3
#define SLEEPMODE     4
#define BATTREAD	  5

//number of levels (depths)
#define NUM_LEVELS 1

//Time asleep ~= 3 x wakeCycles seconds
uint8 wakeCycles = 8;
uint8 retryCount = 0;       //transmission retries

//State definitions
uint8 state;

//battery read
double voltage;
uint16 value;
char Batt_out[16];
uint8 battHealth;

//soil moisture and temperature
float VWC;
float temp;

//Temperature Definitions
double temperature[NUM_LEVELS];
char temp_out[6];

int main(){
	//Enable custom ISR's
    initAllISR();
    
    //Enable Globals
    CyGlobalIntEnable;
    
    //Start I2C (IR sensor)
    I2C_1_Start();
	CyDelay(250);
    
    
	//Start I2C (Soil)
	I2C_Soil_Start();
    
    //Start ADC
    BatteryHealth_Start();
    BatteryHealth_StartConvert();

    //Start XBee
    Xbee_UART_Start();

    /*  something bad happens here 
	//read EEPROM, config, and oscillator trimming value
    readEEPROM();
    writeOscTrimValue();
    writeConfigValue();
    */
    
    //Initialize PSOC state
    state = WAKE;
    
    //Start Sleep Timer 
    SleepTimer_Start();
    
    //turn MOSFET off for battery health
    BattRead_Write(0);                                                  //set MOSFET to off

    uint8 i = 0;
    
    for(;;){
        /*
        Xbee_UART_PutChar(i);           //this works
        ++i;
        CyDelay(3000);
        */
        
        if (state == WAKE){

            
    		state = GETDATA;
    	}

    	else if (state == GETDATA){
            /*
            //MLX READ------------------------------------------
            //Temperature, compensation, and whole frame read
            uint16 ptat = readPTAT();          
            uint16 comp = readCompensation();
            readFrame();    
            //END MLX READ--------------------------------------
    		*/
    		//VEG READ------------------------------------------

    		//END VEG READ--------------------------------------

    		//BATTERY READ--------------------------------------
            //BATTREAD to high
            //battHealth = interpretHealth();
    		//END BATTERY READ----------------------------------

    		state = TXDATA;
    	}

    	else if (state == TXDATA){
            /*
            //TX MLX--------------------------------------------
            //tx whole frame, PTAT, and compensation
            XBeeTxFrame();
            XBeeTxPTAT();
            XBeeTxCompensation();
            //END TX MLX----------------------------------------
            */
            
            CyDelay(100);

            //TX VEG--------------------------------------------
            //toggle pin on ss3, read adc and temp
            ss3pwr_Write(1);
            CyDelay(5000);
            //read i2c
            VWC = readVWC();
            temp = readTemp();

            ss1pwr_Write(0);
            CyDelay(5000);
            
            
            
            
            //END TX VEG----------------------------------------

            //TX BATTERY----------------------------------------
            //sprintf(Batt_out, "%1.4f", voltage);		//raw voltage to string

            Xbee_UART_PutString(Batt_out);				//spews raw voltage

            //END TX BATTERY------------------------------------
            
            
            state = WAIT_FOR_CMD;
    	}
else if (state == WAIT_FOR_CMD) {
            //add firmware
    /*
            char res = Xbee_UART_GetChar();
            
            if(res == 'R'){     //if transmit error
                retryCount++;
                
                if(retryCount > 2){
                    retryCount = 0;
                    state = SLEEPMODE;      //abort transmit
                }
                else
                    state = TXDATA;         //attempt to send again
            
            }
            else if(res == 'C'){
                retryCount = 0;     //reset retry count
                state = SLEEPMODE;
            }
            else
                state = SLEEPMODE;      //should not be reached, but goes to sleep if error
      */      
            state = SLEEPMODE;
        }
        
        else if (state == SLEEPMODE){
            //put her to sleep
            //CyDelay(2000);
            
            if (wake_count % wakeCycles == 0) {
                //Change State
                state = WAKE;
                ++wake_count;
                //continue;
            }
            else {
                sleepPsoc();   
            }
            
            
        } // END SLEEPM0DE
        
        else{ // Error in state machine, to be filled out. Likely a reset will occur here. 
            
            //Reset variables
            wakeCycles = 8;
            retryCount = 0;
            
            //Stop all ISR
            stopAllISR();
            CyDelay(1000);
            
            //Start all ISR again
            initAllISR();
            
            //Change State
            state = WAKE;
        } // End Error Catch
        

    }
}

/* [] END OF FILE */
