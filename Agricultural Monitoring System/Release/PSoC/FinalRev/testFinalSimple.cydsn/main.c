#include <project.h>


//needed variables
float vwc;
float temp;


//functionize stuff
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
//end functions

int main()
{
    //Start XBee
    Xbee_UART_Start();
    I2C_Soil_Start();

    CyGlobalIntEnable;
    
    uint8 i = 0;
    
    for(;;)
    {
        /*
        Xbee_UART_PutChar(i);
        
        ++i;
        
        CyDelay(3000);
        */
        
        ss3pwr_Write(1);        //turn on MOSFET
        CyDelay(500);
        
        vwc = readVWC();
        temp = readTemp();
        
        //ss3pwr_Write(0);
        
        
        
    }
}

/* [] END OF FILE */
