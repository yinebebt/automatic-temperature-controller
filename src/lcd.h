/**
 * lcd.h - 16x2 LCD driver (HD44780), 8-bit mode
 */
#ifndef LCD_H
#define LCD_H
#include <xc.h>            // include processor files - each processor file is guarded.  
#include <stdio.h>

// LCD Hardware Pin Definitions
#define lcd PORTC
#define rs PORTDbits.RD0
#define rw PORTDbits.RD1
#define en PORTDbits.RD2

// LCD Command Definitions
#define LCD_CLEAR           0x01  // Clear display
#define LCD_HOME            0x02  // Return cursor to home
#define LCD_ENTRY_MODE      0x06  // Entry mode: increment cursor, no shift
#define LCD_DISPLAY_OFF     0x08  // Display off
#define LCD_DISPLAY_ON      0x0C  // Display on, cursor off, blink off
#define LCD_CURSOR_ON       0x0E  // Display on, cursor on, blink off
#define LCD_CURSOR_BLINK    0x0F  // Display on, cursor on, blink on
#define LCD_SHIFT_LEFT      0x18  // Shift display left
#define LCD_SHIFT_RIGHT     0x1C  // Shift display right
#define LCD_8BIT_2LINE      0x38  // 8-bit mode, 2 lines, 5x8 font
#define LCD_4BIT_2LINE      0x28  // 4-bit mode, 2 lines, 5x8 font

// LCD Cursor Position Commands
#define LCD_LINE1           0x80  // Move to line 1, position 0
#define LCD_LINE2           0xC0  // Move to line 2, position 0

// Special Characters
#define LCD_DEGREE_SYMBOL   223   // Degree symbol (°) - may vary by LCD

// Function Prototypes
void lcd_cmd(unsigned char cmd);
void lcd_chrctr_wrt(unsigned char chrctr);
void lcd_string_wrt(unsigned char *strng);
void lcd_lat();
void lcd_init();
void delay(unsigned int ms);

#endif

