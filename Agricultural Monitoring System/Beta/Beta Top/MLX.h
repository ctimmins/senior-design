#ifndef MLX_H
    #define MLX_H
    #include <project.h>
    
    void readEEPROM(void);
    void writeOscTrimValue(void);
    void writeConfigValue(void);
    void writeConfigValue(void);
    uint16 readCompensation(void);
    uint16 readPTAT(void);
    void readFrame(void);
    void XBeeTxPTAT(void);
    void XBeeTxPTATparam(void);
    void XBeeTxCompensation(void);
    void XBeeTxPixelOffsets(void);
    void XBeeTxPixelSlope(void);
    void XBeeTxPixelSensitivity(void);
    void XBeeTxParameters(void);
    void XBeeTxFrame(void);
    
#endif

/* [] END OF FILE */
