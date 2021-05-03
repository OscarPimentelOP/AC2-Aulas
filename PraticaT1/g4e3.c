#include <detpic32.h>


int main(void){
    TRISB = (TRISB & 0x00FF); //RB8-15 as outputs
    LATB = (LATB & 0x00FF); //clean

    //RD5-6 as outputs
    TRISDbits.TRISD5 = 0;
    TRISDbits.TRISD6 = 0;

    //select display
    LATDbits.LATD5 = 1; //low
    LATDbits.LATD6 = 1; //high

    char letter;

    while(1){
        letter = getChar();
        LATB = (LATB & 0x00FF);

        switch(letter){
            case 'a':
                LATBbits.LATB8 = 1;
                break;

            case 'b':
                LATBbits.LATB9 = 1;
                break;

            case 'c':
                LATBbits.LATB10 = 1;
                break;
            case 'd':
                LATBbits.LATB11 = 1;
                break;
            case 'e':
                LATBbits.LATB12 = 1;
                break;
            case 'f':
                LATBbits.LATB13 = 1;
                break;
            case 'g':
                LATBbits.LATB14 = 1;
                break;
            case 'h' :
                LATBbits.LATB15 = 1;
                break;                        
        }
    }

}