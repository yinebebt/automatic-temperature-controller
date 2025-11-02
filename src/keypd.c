/**
 * keypd.c - 3x4 matrix keypad scanner
 * Rows: RB0-RB3 (outputs), Cols: RB4-RB6 (inputs with pull-ups)
 */
#include <xc.h>
#include "kpd.h"

#define _XTAL_FREQ 8000000

#define row1port LATBbits.LATB0
#define row2port LATBbits.LATB1
#define row3port LATBbits.LATB2
#define row4port LATBbits.LATB3
#define col1port PORTBbits.RB4
#define col2port PORTBbits.RB5
#define col3port PORTBbits.RB6

static char old_key = 0xFF;

char kpd_getc() {
    char key = 0;
    char row;

    for (row = 0b00000001; row < 0b00010000; row <<= 1) {
        row1port = (row & 0x0001) >> 0;
        row2port = (row & 0x0002) >> 1;
        row3port = (row & 0x0004) >> 2;
        row4port = (row & 0x0008) >> 3;
        __delay_ms(1);

        if (col1port) break;
        key++;
        if (col2port) break;
        key++;
        if (col3port) break;
        key++;
    }

    row1port = 0;
    row2port = 0;
    row3port = 0;
    row4port = 0;

    if (key != old_key) {
        old_key = key;
        return keyPadMatrix[key];
    }
    return keyPadMatrix[0x0C];
}