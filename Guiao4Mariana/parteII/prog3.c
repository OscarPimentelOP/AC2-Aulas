#include <detpic32.h>

void delay(int ms){
    for(; ms > 0; ms--){
         resetCoreTimer();
        while(readCoreTimer() < 20000);
    }
}

void send2displays(unsigned char value){
    static const char display7Scodes[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07,
                                        0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};

    static char displayFlag = 0;

    TRISB = (TRISB & 0x00FF);               // RB8-RB15 AS OUTPUT
    TRISD = (TRISD & 0xFF9F);               // RD5 E RD6 AS OUTPUT

    LATB = (LATB & 0x0000);                 // limpar pins

    // Determine digits
    int digit_low = value & 0x0F;           // últimos 4 bits  
    int digit_high = value >> 4;            // primeiros 4 bits

    // Send digit_low to display low
    if (displayFlag == 0) {
        LATB = (LATB & 0x00FF) | (display7Scodes[digit_low] << 8);
        LATDbits.LATD5 = 1;                 // LIGA DIGIT LOW
        LATDbits.LATD6 = 0;                 // DESLIGA DIGIT HIGH
    }else{
         LATB = (LATB & 0x00FF) | (display7Scodes[digit_high] << 8);
        LATDbits.LATD5 = 0;
        LATDbits.LATD6 = 1;
    }

  // Toggle displayFlag
  displayFlag = !displayFlag;
}

int main(void){
    int counter;
    int i;
    TRISB = (TRISB & 0x00FF);
    LATB = (LATB & 0x00FF);     // limpar ports
    counter = 0;

    while(1){
        i = 0;
        do{
            send2displays(counter);
            delay(15);
        }while (i++<4);
        counter++; //increase counter
        counter = counter % 256; //mod 256
    }
    return 0;
}