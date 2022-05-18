#include <xc.h>
#define _XTAL_FREQ 8000000                 //Useful for __delay_ms() function 
#include "kpd.h"
//#define set_port_kbd PORTB // Change if port is different
#define row1port LATBbits.LATB0
#define row2port LATBbits.LATB1
#define row3port LATBbits.LATB2
#define row4port LATBbits.LATB3
#define col1port PORTBbits.RB4
#define col2port PORTBbits.RB5
#define col3port PORTBbits.RB6
//#define col4port PORTBbits.RB7   //if a 4x4 keypad is used

char kbd_getc(){
    // This routine returns the first key found to be pressed during the scan.
    char key = 0, row;
    for( row = 0b00000001; row < 0b00010000; row <<= 1 )
    {
        {   // turn on row output
            row1port = (row & 0x0001)>>0;
            row2port = (row & 0x0002)>>1;
            row3port = (row & 0x0004)>>2;
            row4port = (row & 0x0008)>>3;
            __delay_ms(1);
        }
        // read colums - break when key press detected
        if( col1port )break;  key++;
        if( col2port )break;  key++;
        if( col3port )break;  key++;
        //if( col4port )break;  key++;
     }
    row1port = 0;
    row2port = 0;
    row3port = 0;
    row4port = 0;
    if (key!=old_key){
      old_key=key;
      return keyPadMatrix[ key ];
    }
    else
    return keyPadMatrix[ 0x0C ];
}