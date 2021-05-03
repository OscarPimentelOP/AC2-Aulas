#include <detpic32.h>

void send2displays(unsigned char value);
int main(void){
    unsigned char val;

    //RB0-3 as inputs
    TRISB = (TRISB | 0x000F);

    while(1){
        val = (PORTB & 0x000F);
        send2displays(val);
    }
    return 0;
}


void send2displays(unsigned char value){
     static const char display7Scodes[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x67, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};
     static char displayFlag = 0;

     TRISB = (TRISB & 0x00FF); //RB8-15 outputs

    //RD5-6 outputs
     TRISDbits.TRISD5 = 0;
     TRISDbits.TRISD6 = 0;

     //determine digits
     int digit_low = value & 0x0F;
     int digit_high = value >> 4;

     //send digit_low to display low
     if(displayFlag == 0){
         LATB = (LATB & 0x00FF) | (display7Scodes[digit_low] << 8);
         LATDbits.LATD5 = 1;
         LATDbits.LATD6 = 0;
     }
     else{ //send digit_high to display high
         LATB = (LATB & 0x00FF) | (display7Scodes[digit_high] << 8);
         LATDbits.LATD5 = 0;
         LATDbits.LATD6 = 1;
     }
     displayFlag = !displayFlag;
}
