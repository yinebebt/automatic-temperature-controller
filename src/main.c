/**
 * main.c - Temperature controller for PIC18F43K22
 * Hysteresis control: ±2°C deadband prevents relay chatter
 */
#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include "config.h"
#include "kpd.h"
#include "adc.h"
#include "lcd.h"

#define vref 5.00
#define HEATER PORTAbits.RA1
#define FAN PORTAbits.RA2

#define HYSTERESIS 2
#define MIN_TEMP 0
#define MAX_TEMP 99
#define SENSOR_ERROR_LOW 0
#define SENSOR_ERROR_HIGH 100

volatile short int Temp_Ref;
volatile unsigned char data[10], data2[10];
volatile short int digital;
volatile short int temperature_c;
volatile unsigned char keypress;

volatile char const keyPadMatrix[] = {
    '1', '2', '3',
    '4', '5', '6',
    '7', '8', '9',
    '*', '0', '#',
    0xFF
};

volatile unsigned char const Seven_Segment_MAP[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
void main(void) {
    // Port configuration
    ANSELC = 0x00;
    ANSELB = 0x00;
    ANSELD = 0x00;
    ANSELA = 0x29;  // RA0=analog (LM35)

    TRISA = 0xF9;   // RA1,RA2=outputs (heater,fan), RA0=input (ADC)
    TRISB = 0xF0;   // RB0-3=outputs (keypad rows), RB4-6=inputs (columns)
    TRISC = 0x00;   // LCD data
    TRISDbits.RD0 = 0;  // LCD RS
    TRISDbits.RD1 = 0;  // LCD RW
    TRISDbits.RD2 = 0;  // LCD EN

    LATB = 0x00;
    LATC = 0x00;
    INTCON2bits.nRBPU = 1;  // Disable internal pull-ups

    lcd_init();
    ADC_Initialize();
    lcd_string_wrt("Temp_sensor");
    __delay_ms(1000);

    HEATER = 0;
    FAN = 0;

    // Get setpoint from keypad
    while(1) {
        Temp_Ref = 0;
        unsigned char digit_count = 0;

        lcd_cmd(LCD_CLEAR);
        lcd_cmd(LCD_LINE1);
        lcd_string_wrt("Enter Temp Ref");
        lcd_cmd(LCD_LINE2);
        lcd_string_wrt("Temp Ref: ");

        while (1) {
            keypress = kpd_getc();

            if (keypress == '*') break;

            if (keypress == '#') {
                if (digit_count == 0) {
                    lcd_cmd(LCD_LINE2);
                    lcd_string_wrt("Enter value!    ");
                    __delay_ms(1000);
                    break;
                }
                if (Temp_Ref < MIN_TEMP || Temp_Ref > MAX_TEMP) {
                    lcd_cmd(LCD_LINE2);
                    lcd_string_wrt("Range: 0-99C!   ");
                    __delay_ms(2000);
                    break;
                }
                goto CONFIRM_SETPOINT;
            }

            if (keypress >= '0' && keypress <= '9' && keypress != 0xFF) {
                if (digit_count < 2) {
                    lcd_chrctr_wrt(keypress);
                    Temp_Ref = 10 * Temp_Ref + (keypress - '0');
                    digit_count++;
                }
            }
        }
        continue;

CONFIRM_SETPOINT:
        lcd_cmd(LCD_CLEAR);
        lcd_cmd(LCD_LINE1);
        lcd_string_wrt("Temp Ref: ");
        sprintf(data, "%d", Temp_Ref);
        lcd_string_wrt(data);
        lcd_chrctr_wrt(LCD_DEGREE_SYMBOL);
        lcd_string_wrt("C");
        lcd_cmd(LCD_LINE2);
        lcd_string_wrt("Press # to Cont.");

        keypress = 0;
        while (keypress != '#') {
            do {
                keypress = kpd_getc();
            } while (!keypress);
        }

        lcd_cmd(LCD_CLEAR);
        lcd_cmd(LCD_LINE1);
        lcd_string_wrt("Temp Ref: ");
        lcd_string_wrt(data);
        lcd_chrctr_wrt(LCD_DEGREE_SYMBOL);
        lcd_string_wrt("C");


        // Control loop
        while(1) {
            ADCON0bits.CHS = 0;
            ADCON0bits.ADON = 1;
            __delay_us(10);  // Acquisition time

            ADCON0bits.GO_nDONE = 1;
            while((bool)ADCON0bits.GO_nDONE);

            digital = ADC_GetConversionResult();
            temperature_c = ((float)digital * 5.0) / 10.24;  // LM35: 10mV/°C

            // Sensor error check
            if (temperature_c <= SENSOR_ERROR_LOW || temperature_c >= SENSOR_ERROR_HIGH) {
                lcd_cmd(LCD_LINE2);
                lcd_string_wrt("SENSOR ERROR!   ");
                HEATER = 0;
                FAN = 0;
                __delay_ms(1000);
                continue;
            }

            // Display current temperature
            sprintf(data2, "%d", temperature_c);
            lcd_cmd(LCD_LINE2);
            lcd_string_wrt("Temp is: ");
            lcd_string_wrt(data2);
            lcd_chrctr_wrt(LCD_DEGREE_SYMBOL);
            lcd_chrctr_wrt('C');
            lcd_string_wrt("   ");

            // Hysteresis control (±2°C deadband prevents relay chatter)
            if (temperature_c < (Temp_Ref - HYSTERESIS)) {
                HEATER = 1;
                FAN = 0;
            }
            else if (temperature_c > (Temp_Ref + HYSTERESIS)) {
                HEATER = 0;
                FAN = 1;
            }
            // else: within deadband - no change

            Delay_Seconds(1);
        }
    }
}
