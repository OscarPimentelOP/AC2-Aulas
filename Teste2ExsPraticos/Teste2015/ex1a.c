#include <detpic32.h>

void delay(int ms);

int main(void){

    //RB3-0 as inputs DS
    TRISB = (TRISB & 0xFFF0) | 0x000F;

    //RE0-7 as outputs 
    TRISE = (TRISE & 0xFFF0);

    int dip_switch;

    while(1){
        //(PORTBbits.RB0 == 0) ? (PORTEbits.RE0 = 0) : (PORTEbits.RE0 = 1);
        //(PORTBbits.RB1 == 0) ? (PORTEbits.RE1 = 0) : (PORTEbits.RE1 = 1);
        //(PORTBbits.RB2 == 0) ? (PORTEbits.RE2 = 0) : (PORTEbits.RE2 = 1);
        //(PORTBbits.RB3 == 0) ? (PORTEbits.RE3 = 0) : (PORTEbits.RE3 = 1);
        dip_switch = PORTB & 0x000F;
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