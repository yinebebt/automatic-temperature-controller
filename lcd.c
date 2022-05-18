#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include  <string.h>
#include "config.h"
#include <stdbool.h>
#include <stdint.h>
#include "kpd.h"
#include "acd.h"
#include "ldc.h"

#define vref 5.00 /*Reference Voltage is 5V*/
#define HEATER PORTAbits.RA1
#define FAN PORTAbits.RA2

volatile short int Temp_Ref;
volatile unsigned char data[10], data2[10];
volatile short int digital;
volatile short int voltage;
void main(void) { 
    
    ANSELC = 0X00;
    ANSELB = 0x00;
    ANSELD &= 0X00;
    TRISA = 0xF9;
    //LATA = 0x00;
    ANSELA = 0x29;
    TRISC = 0X00;
    TRISB = 0xF0;
    LATB = 0x00;
    LATC =0x00;
    INTCON2bits.nRBPU = 1;
    
    TRISDbits.RD0 = 0;
    TRISDbits.RD1 = 0;
    TRISDbits.RD2 = 0;
    
    lcd_init();
    ADC_Initialize(); /*Initialize 10-bit ADC*/
    lcd_string_wrt("Temp_sensor");
    __delay_ms(10);
    
    HEATER = 0;                 //Switch OFF Heater on start up
    FAN = 0;                    //Switch OFF Fan on start up
    
    START:
       lcd_cmd(0x01);                     //Clear display
       lcd_cmd(0x80);                   // Move to row=1,column=1
       lcd_string_wrt("Enter Temp Ref");
       Temp_Ref=0;
       lcd_cmd(0xC0);                   // Move to row=2,column=1
       lcd_string_wrt("Temp Ref: ");

   while (1){
       keypress = kbd_getc();
       if ( keypress == '*' )goto START;  //If Clear '*' pressed
       if ( keypress == '#' )break;
       if(keypress!=0xFF){
          lcd_chrctr_wrt(keypress);
           Temp_Ref = 10*Temp_Ref + (Seven_Segment_MAP[keypress-'0']);
       }       
     }
  
       lcd_cmd(0x01);                     //Clear display
       lcd_cmd(0x80);                   // Move to row=1,column=1
       lcd_string_wrt("Temp Ref: ");
      sprintf(data,"%d",Temp_Ref); /*It is used to convert integer value to ASCII string*/
       lcd_string_wrt(data);            //Display the temp ref
       lcd_cmd(0xC0);                   // Move to row=2,column=1
        lcd_string_wrt("Press # to Cont.");
     
      keypress =0;
      while(keypress!='#')
      {
         do
           keypress = kbd_getc();  // read keypad
         while(!keypress);
      }
       lcd_cmd(0x01);               //Clear display
       lcd_cmd(0x80);             // Move to row=1,column=1
       lcd_string_wrt("Temp Ref: ");
       lcd_string_wrt(data);      //Display the temp ref
       lcd_chrctr_wrt(223);           // Different LCD displays have different char code for degree
       lcd_string_wrt(" C"); /*Concatenate result and unit to print*/
   
    while(1){   //digital =0;
 //ADC channel selection     
        ADCON0bits.CHS = 0;    // select the A/D channel 
        ADCON0bits.ADON = 1;   // Turn on the ADC module

        ADCON0bits.GO_nDONE = 1;   // start the ADC conversion 
        while( (bool)ADCON0bits.GO_nDONE)   // wait till the conversion is done/completed, after completion, the go_ndone bit will cleared.
              ;
        digital = ADC_GetConversionResult(); 
        voltage = ((float)digital*5.0)/10.24;       //convert data into temperature (LM35 produces 10mV per degree celcius)      
        sprintf(data2, "%d",voltage ); // Convert voltage to string
        lcd_cmd(0xC0);                   // Move to row=2,column=1
        lcd_string_wrt("Temp is: ");
        lcd_string_wrt(data2);      //Display the temp ref
        lcd_chrctr_wrt(223);// Different LCD displays have different char code for degree
        lcd_chrctr_wrt('C'); 
        lcd_string_wrt("   ");              // Clear after comma     
        if (Temp_Ref >  voltage)  //If Temp Ref is greater than actual Temp, Switch ON Heater
      {
      HEATER = 1,
      FAN = 0;
      }
       if (Temp_Ref <  voltage)  //If Temp Ref is less than actual Temp, Switch ON Fan
      {
      HEATER = 0,
      FAN = 1;
      }
      if (Temp_Ref ==  voltage)  //If Temp Ref is equal to actual Temp, Switch OFF Fan and Heater
      {
      HEATER = 0,
      FAN = 0;
      } 
   Delay_Seconds(1);   //Wait 10 s then repeat    
}
}
