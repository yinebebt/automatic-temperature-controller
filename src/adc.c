#include <stdbool.h>
#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdbool.h>
#define _XTAL_FREQ 8000000                 //Useful for __delay_ms() function 
void ADC_Initialize(void){
    // set the ADC to the options selected in the User Interface
    
    // TRIGSEL CCP5; NVCFG VSS; PVCFG VDD; 
    ADCON1 = 0x00;
    
    // ADFM right; ACQT 2; ADCS FOSC/2; 
    ADCON2 = 0x88;
    
    // ADRESL 0; 
    ADRESL = 0x00;
    
    // ADRESH 0; 
    ADRESH = 0x00;
    
    // GO_nDONE stop; ADON enabled; CHS AN0; 
    ADCON0 = 0x01;
    
}
int ADC_GetConversionResult(void)
{
    // Conversion finished, return the result
     return ((int)((ADRESH << 8) + ADRESL));
}
void Delay_Seconds(unsigned char z)
{
    unsigned char x,y;
    for(y = 0; y < z; y++)
    {
        for(x = 0; x <  10; x++)__delay_ms(10);
    }
}