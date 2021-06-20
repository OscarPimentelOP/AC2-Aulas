#include <detpic32.h>

void delay(int ms);
void send2displays(unsigned char value);
unsigned char toBcd(unsigned char value);
void configTimer3Display();

volatile unsigned char dispCount =  99;

int main(void){

    //RE0-7 as outputs 
    TRISE = (TRISE & 0xFF00);
    //turn on the leds
    LATE = (LATE & 0xFF00) | 0x00FF;

    //t = 100 ms -> f = 10Hz para os displays
    configTimer3Display();
    EnableInterrupts();

    int whiteCount = 80;
    int blackCount = 80;
    static char whiteTurn = 1;

    while(1){
        char turn = inkey(); 
       if(whiteTurn==1){
            printInt(whiteCount, 10);
            printStr("-");
            whiteCount--;
             delay(1000);
            if((whiteCount != 0) && (turn =='b')){
                whiteTurn = 0;
                whiteCount = 80;
                dispCount =  99;
                //turn on the leds
                LATE = (LATE & 0xFF00) | 0x00FF;
                printStr("preto joga");
            }
            else if ((whiteCount != 0) && (turn == 'x')){
                printStr("White player wins!");
                return 0;
            }
            else if(whiteCount == 0){
                printStr("Black player wins!");
                return 0;
            }

        }
        else{
            printInt(blackCount, 10);
            blackCount--;
            delay(1000);

            if((blackCount != 0) && (turn =='b')){
                whiteTurn = 1;
                blackCount = 80;
                dispCount =  99;
                //turn on the leds
                LATE = (LATE & 0xFF00) | 0x00FF;
                printStr("branco joga");
            }
            else if((whiteCount != 0) && (turn == 'x')){
                printStr("Black player wins!");
                return 0;
            }
            else if(blackCount == 0){
                printStr("White player wins!");
                return 0;
            }
        }    
    }
    return 0;
}

void delay(int ms){
    for(; ms > 0; ms--){
        resetCoreTimer();
        while(readCoreTimer() < 20000);
    }
}

void send2displays(unsigned char value) {

  static const char display7Scodes[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x67, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};
  static char displayFlag   = 0;

  // Configure RB8 to RB15 as outputs
  TRISB = (TRISB & 0x00FF);

  // Configure RD5 and RD6 as outputs
  TRISDbits.TRISD5 = 0;
  TRISDbits.TRISD6 = 0;

  // Determine digits
  int digit_low = value & 0x0F;     
  int digit_high = value >> 4;      

  // Send digit_low to display low
  if (displayFlag == 0) {
    LATB = (LATB & 0x00FF) | (display7Scodes[digit_low] << 8);
    LATDbits.LATD5 = 1;
    LATDbits.LATD6 = 0;
  }

  // Send digit_high to display high
  else {
    LATB = (LATB & 0x00FF) | (display7Scodes[digit_high] << 8);
    LATDbits.LATD5 = 0;
    LATDbits.LATD6 = 1;
  }
  displayFlag = !displayFlag;

}

unsigned char toBcd(unsigned char value){
    return ((value/10) << 4) + (value%10);
}

void configTimer3Display(){ //10hz
    T3CONbits.TCKPS = 5;
    PR3 = 62499;
    TMR3 = 0;
    T3CONbits.TON = 1;

    IPC3bits.T3IP = 2;
    IEC0bits.T3IE = 1;
    IFS0bits.T3IF = 0;

}

void _int_(12) isr_T3(void){
    //printInt(dispCount, 10);
    send2displays(toBcd(dispCount));

    if(dispCount == 0){
        if(LATEbits.LATE7 == 1){
            LATEbits.LATE7 = 0;
        }
        else if(LATEbits.LATE6 == 1){
            LATEbits.LATE6 = 0;
        }
        else if(LATEbits.LATE5 == 1){
            LATEbits.LATE5 = 0;
        }
        else if(LATEbits.LATE4 == 1){
            LATEbits.LATE4 = 0; 
        }
        else if(LATEbits.LATE3 == 1){
            LATEbits.LATE3 = 0;
        }
        else if(LATEbits.LATE2 == 1){
            LATEbits.LATE2 = 0;
        }
        else if(LATEbits.LATE1 == 1){
            LATEbits.LATE1 = 0;
        }
        else if(LATEbits.LATE0 == 1){
            LATEbits.LATE0 = 0;
        }  
        if(LATEbits.LATE0 != 0){     
            dispCount = 99;
        }
    }
    dispCount = dispCount-1;
    IFS0bits.T3IF = 0;       

}


