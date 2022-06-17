#include "ldc.h"
#include <xc.h>

#define lcd PORTC
#define rs PORTDbits.RD0
#define rw PORTDbits.RD1
#define en PORTDbits.RD2


void lcd_lat(){
    en = 1;
    delay(1);
    en = 0;
}
void lcd_cmd(unsigned char cmd){
    lcd = cmd;
    rs = 0;
    lcd_lat();
}
void lcd_chrctr_wrt(unsigned char chrctr){
    lcd = chrctr;
    rs = 1;
    rw = 0;
    lcd_lat();
}
void lcd_string_wrt (unsigned char *strng){
    while(*strng != '\0'){   // if the pointer is not null, same to array
        lcd_chrctr_wrt(*strng++);
    }
}
void lcd_init(){
    lcd_cmd (0x38);
    lcd_cmd (0x0c);
    lcd_cmd (0x80);
}
void delay(unsigned char ms){
    unsigned int i,j;
    for(i=0;i<=120;i++)
        for(j=0;j<=ms;j++);
}
