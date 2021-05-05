#include <detpic32.h>

void delay(int ms){
    for(; ms>0;ms--){
        resetCoreTimer();
        while(readCoreTimer() < 20000);
    }
}

void send2displays(unsigned char value){
    static const char display7Scodes[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07,
                                        0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};

    static char displayFlag = 0;

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

unsigned char toBCD(unsigned char value){
    return ((value/10)<<4) + (value % 10);
}

int main(void){

    TRISB = (TRISB & 0x00FF);       // RB8-RB15 as output
    TRISD = (TRISD & 0x0090);
    LATB = (LATB & 0x0000);         // clean ports
    char value;
    int i;

    while(1){
        i = 0;
        value = getChar();
        if(value=='0'|| value == '1' || value == '2' || value == '3'){
            send2displays((value & 0x0f));
        }else{
            while(i++ < 100){
                send2displays(0xFF);
                delay(10);                    
            }
            LATB = LATB & 0x00FF;               // apaga os ports como pedido
        }
        delay(10);
    }
    return 0;
}