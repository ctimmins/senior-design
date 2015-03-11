#ifndef UTILS_H
#define UTILS_H
    
    #include <stddef.h>
    #include <project.h>
    
    char calculateCheckSum(char *res, int len);
    void sleepPsoc();
    uint8 interpretHealth(void);        //read battery health
    float readVWC(void);                //read VWC on I2C Soil
    float readTemp(void);               //read Temp on I2C Soil
    
#endif
    