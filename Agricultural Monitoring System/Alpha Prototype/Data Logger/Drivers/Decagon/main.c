#include <project.h>

void PrintResult(void);
void TogglePower(void);

int main()
{
    UART_1_Start();
    
    LCD_Start();
    LCD_ClearDisplay();
    LCD_Position(0,0);
    LCD_PrintString("Bytes: ");
    
    AMux_1_Start();
    AMux_1_Select(0);      
    
    CyGlobalIntEnable;
   
    for(;;)
    {
        TogglePower();
    }
}


void PrintResult(void) {
    //clear lcd display
    LCD_ClearDisplay();
    LCD_Position(0,0);
    LCD_PrintString("Bytes: ");
    
    //find number of bytes available in rx buffer
    char size = UART_1_GetRxBufferSize(); 
    LCD_Position(7,0);
    LCD_PrintDecUint16(size);
    
    //print response
    LCD_Position(1,0);
    char res[size];
    int i;
    for (i = 0; i < size; ++i) {
        res[i] = UART_1_GetChar();
        if (res[i] == 13) {
            res[i] = ' ';
            //UART_1_ClearRxBuffer();
            break;
        }
    }
    
    LCD_PrintString(res);
}

void TogglePower(void) {
    UART_1_Start();
    CyDelay(5000);    
    Pin_led_Write(1);
    AMux_1_Select(1);
    CyDelay(120);
    PrintResult();
    UART_1_ClearRxBuffer();
    UART_1_Stop();
    CyDelay(5000);
    Pin_led_Write(0);
    AMux_1_Select(0);
}
