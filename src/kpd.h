/**
 * kpd.h - 3x4 matrix keypad scanner
 */
#ifndef KPD_H
#define KPD_H

extern volatile unsigned char keypress;
extern volatile char const keyPadMatrix[];
extern volatile unsigned char const Seven_Segment_MAP[10];

char kpd_getc(void);

#endif

