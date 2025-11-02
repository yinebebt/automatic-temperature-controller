/**
 * adc.c - 10-bit ADC for LM35 temperature sensor (AN0)
 * Config: 10-bit, right-justified, Vref=VDD(5V), FOSC/2
 */
#include <xc.h>
#include <stdbool.h>

#define _XTAL_FREQ 8000000

void ADC_Initialize(void) {
    ADCON1 = 0x00;  // Vref: VDD/VSS
    ADCON2 = 0x88;  // Right-justified, 2 TAD acquisition, FOSC/2
    ADRESL = 0x00;
    ADRESH = 0x00;
    ADCON0 = 0x01;  // Enable ADC, select AN0
}

int ADC_GetConversionResult(void) {
    return ((int)((ADRESH << 8) + ADRESL));
}

void Delay_Seconds(unsigned char z) {
    unsigned char x, y;
    for (y = 0; y < z; y++) {
        for (x = 0; x < 100; x++) {
            __delay_ms(10);
        }
    }
}