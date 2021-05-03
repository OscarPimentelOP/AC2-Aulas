#include <detpic32.h>

void delay(int ms);
void send2displays(unsigned char value);
unsigned char toBcd(unsigned char value);

int main(void){

    char input;

    while(1){
        input = getChar();
        LATB = (LATB & 0x00FF); //clean RB8-15

        if((input == '0') || (input == '1') || (input == '2') || (input == '3')){
            delay(1000); //update display in 100Hz
            send2displays(toBcd(input));
            LATB = (LATB & 0x00FF); //clean RB8-15
        }
        else{
            send2displays(15);
            delay(100);
            LATB = (LATB & 0x00FF); //clean RB8-15
        }

    }

}

void delay(int ms){
    for(; ms > 0; ms--){
        resetCoreTimer();
        while(readCoreTimer() < 20000);
    }
}  

void send2displays(unsigned char value) {

    static const char display7Scodes[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x67, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};
    static char displayFlag = 0;

    // Configure RB8-15 as outputs
    TRISB = (TRISB & 0x00FF);

    // Configure RD5 and RD6 as outputs
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

    //send digit_high to display high
    else{
        LATB = (LATB & 0x00FF) | (display7Scodes[digit_high] << 8);
        LATDbits.LATD5 = 0;
        LATDbits.LATD6 = 1;
    }
    displayFlag = !displayFlag;
}

unsigned char toBcd(unsigned char value){
    return ((value/10)<<4) + (value % 10);
}