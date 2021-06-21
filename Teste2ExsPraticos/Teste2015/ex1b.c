#include <detpic32.h>

void delay(int ms);
void send2displays(unsigned char value);

int main(void){

    //RB3-0 as inputs DS
    TRISB = (TRISB & 0xFFF0) | 0x000F;

    //RE0-7 as outputs 
    TRISE = (TRISE & 0xFFF0);

    int dip_switch;

    while(1){
        dip_switch = PORTB & 0x000F;
        send2displays(dip_switch);
        PORTE = PORTB;
        printInt(dip_switch, 10);
        printStr("\n");
        delay(100);

    }

    return 0;
}

void delay(int ms){
    for(; ms > 0; ms--){
        resetCoreTimer();
        while(readCoreTimer() < 20000);
    }
}

void send2displays(unsigned char value){
    static const char display7Scodes[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x67, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};
    static char displayFlag   = 0;

    //RB8-15 as outputs
    TRISB = (TRISB & 0x00FF);

    //RD5-6 as outputs
    TRISDbits.TRISD5 = 0;
    TRISDbits.TRISD6 = 0;

    //determine digits
    int digit_low = (value & 0x0F);
    int digit_high = (value >> 4);

    if(displayFlag == 0){
        LATB = (LATB & 0x00FF) | (display7Scodes[digit_low] << 8);
        LATDbits.LATD5 = 1;
        LATDbits.LATD6 = 0;
    }
    else{
        LATB = (LATB & 0x00FF) | (display7Scodes[digit_high] << 8);
        LATDbits.LATD5 = 0;
        LATDbits.LATD6 = 0;
    }

}