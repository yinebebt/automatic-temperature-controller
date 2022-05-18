
#include <xc.h>            // include processor files - each processor file is guarded.  
#include <stdio.h>
#include <stdlib.h>
#include  <string.h>
#include <stdbool.h>
#include <stdint.h>



void delay(unsigned int ms);
void lcd_init();
void lcd_string_wrt (unsigned char *strng);
void lcd_chrctr_wrt(unsigned char chrctr);
void lcd_cmd(unsigned char cmd);
void lcd_lat();