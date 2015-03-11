#include "utils.h"

char calculateCheckSum(char *res, int len) {
    int sum = 0;
    int length = len;
    int i;
    for (i=0; i < length; ++i){
        sum += res[i];
    }
    
    return sum % 26 + 97;
    
}

//Sleep utility functions
void sleepPsoc() {
    //Prepare clock tree configuration
    CyPmSaveClocks();
    CyDelay(25);
    CyPmSleep(PM_SLEEP_TIME_NONE, PM_SLEEP_SRC_CTW);
    CyPmRestoreClocks();
}

//interpret/read battery health
uint8 interpretHealth(void){
    uint8 value;
    double voltage;
    
    BattRead_Write(1);                                                  //set MOSFET on
    voltage = (BatteryHealth_GetResult16(0)) * (3.3/4096) * (3 / 2);		//read, scale ADC
    BattRead_Write(0);                                                  //set MOSFET to off
    
    if(voltage > 3.75){
        value = 3;       //excellent   
    } else if (voltage > 3.6 && voltage <= 3.75){
        value = 2;       //good   
    } else if(voltage > 3.4 && voltage <= 3.6){
        value = 1;      //fair
    }
    else
        value = 0;  //replace now
        
    return value;
}

//Read soil VWC on I2C bus
float readVWC(void){
    
    //Declarations of Addresses/Values
    uint8 SlaveAdd = 36;
    uint8 ConfigAdd = 2;
    uint8 ConfigData = 16;
    uint8 ReadAdd = 16;    
    	
    	//Configure
    I2C_Soil_MasterSendStart(SlaveAdd, 0);
    I2C_Soil_MasterWriteByte(ConfigAdd);
    I2C_Soil_MasterWriteByte(ConfigData);
    I2C_Soil_MasterSendStop();
    I2C_Soil_MasterSendStart(SlaveAdd,0);
    I2C_Soil_MasterWriteByte(ReadAdd);
    I2C_Soil_MasterSendStop();
    uint8 b1;
    uint8 b2;

    	//Read
    I2C_Soil_MasterSendStart(SlaveAdd,1);
    b1 = I2C_Soil_MasterReadByte(1);
    b2 = I2C_Soil_MasterReadByte(0);
    I2C_Soil_MasterSendStop();

    	//Math
    uint16 ADCReading = b1*256 + b2;
    float Voltage = ADCReading * (3.3/4096);
    	
    return Voltage;
}

//Read soil Temperature on I2C bus
float readTemp(void){
    
    //Declarations of Addresses
    uint8 TempSlaveAdd = 79;
    uint8 TempReadAdd = 0;
    
    //configure Temp
    I2C_Soil_MasterSendStart(TempSlaveAdd,0);
    I2C_Soil_MasterWriteByte(TempReadAdd);
    I2C_Soil_MasterSendRestart(TempSlaveAdd,1);
    uint8 t1, t2;
    t1 = I2C_Soil_MasterReadByte(1);
    t2 = I2C_Soil_MasterReadByte(0);
    I2C_Soil_MasterSendStop();
    t2 = t2>>5;

    uint16 t3 = t1<<3;
    
    uint16 tempRaw = t2 + t3;
    
    float tempOut;
    
   if (tempRaw < 2048){
        
    tempOut = tempRaw * .125;
    }
    
    else{
    tempOut = -4095 + tempRaw;
    tempOut = tempRaw * .125;
    }
    
    
    return tempOut;
}

