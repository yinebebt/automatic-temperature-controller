/**
 * adc.h - 10-bit ADC driver for LM35 temperature sensor
 */
#ifndef ADC_H
#define ADC_H

void ADC_Initialize(void);
int ADC_GetConversionResult(void);
void Delay_Seconds(unsigned char z);

#endif

