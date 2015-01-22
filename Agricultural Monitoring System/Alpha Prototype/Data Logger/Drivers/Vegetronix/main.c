#include <project.h>
#include <math.h>


int main()
{
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
ADC_Start();
LCD_Start();

float32 voltage;

char tstr[16];
   CyGlobalIntEnable;  /* Uncomment this line to enable global interrupts. */
    for(;;)
    {
        LCD_ClearDisplay();
        ADC_StartConvert();
        if(ADC_IsEndConversion(ADC_RETURN_STATUS)){
        voltage = ADC_CountsTo_Volts(ADC_GetResult16());
        if (voltage

        LCD_Position(0,0); 
        sprintf(tstr, "%+1.4f", voltage );
        LCD_PrintString(tstr);  }
        CyDelay(1000);
        
        
        
        /* Place your application code here. */
    }
}