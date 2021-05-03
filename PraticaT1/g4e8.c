#include <detpic32.h>

int main(void){
    unsigned char val;
 	static const char display7Scodes[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};

    // configure RB0 to RB3 as inputs
    TRISB = (TRISB | 0x000F);

    // configure RB8 to RB14 as outputs
    TRISB = (TRISB & 0x00FF);
    LATB = (LATB & 0x00FF);

    //configure RD5 to RD6 as outputs
    TRISDbits.TRISD5 = 0;
    TRISDbits.TRISD6 = 0;

    // Select display low
    LATDbits.LATD5 = 1;
    LATDbits.LATD6 = 0;

    while(1){
        val = PORTB & 0x000F; // read dip-switch
        LATB = (LATB & 0x00FF) | (display7Scodes[val]<<8); // convert to 7 segments code, send to display
    }
    return 0;
}

//todo with send2displays