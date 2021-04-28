/**
 * Altere o programa anterior de modo a mostrar o valor lido do dip-switch no display mais
 * significativo.
 */

#include <detpic32.h>
#include "delay.c"

void main(void) {
  TRISB = (TRISB & 0x00FF);   //sets pins B8-B15 as output
                              //sets pins B0-B3 as input
  TRISD = (TRISD & 0xFF9F);   //sets pins D5-D6 as output
  LATB = (LATB & 0x0000);     //sets the output of B8-B15 as '0'
  LATDbits.LATD5 = 0;         //display low control
  LATDbits.LATD6 = 1;         //display high control

  static const char display7Scodes[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07,
                                        0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};

  char value;

  while(1) {
    value = PORTB & 0x000F;
    LATB = (LATB & 0x000F) | (display7Scodes[value] << 8);
  }
}
