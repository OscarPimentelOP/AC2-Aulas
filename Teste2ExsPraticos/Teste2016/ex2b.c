#include <detpic32.h>

void configADC();
void delay(int ms);
void send2displays(unsigned char value);
unsigned char toBcd(unsigned char value);
void configTimer2();

volatile char v_dec;

int main(void){

      configADC();
      configTimer2();
      EnableInterrupts();

    while(1) {
      int x;
     if(x++ % 25 == 0){ // 0, 250ms, 500ms, 750ms, ...
        // Start conversion
        AD1CON1bits.ASAM = 1; // Start conversion
        while( IFS1bits.AD1IF == 0 );// Wait while conversion not done (AD1IF == 0)
        int i;
        int *p = (int *)(&ADC1BUF0);
        int val_ad = 0;
        int v; //voltage 
        putChar('\n');
        for( i = 0; i < 16; i++ ){
            val_ad = val_ad + p[i*4];
            //printInt(p[i*4],0x0005000A);
        } 
        val_ad = val_ad / (AD1CON2bits.SMPI +1); // Average of 4 samples
        v = (val_ad * 70 + 511)/1023;
        v_dec = toBcd(v);
        printInt(v,10);
    }
    delay(20); //50 Hz amostragem ADC
 }


    return 0;
}

void configADC(){
    //config ADC module
    TRISBbits.TRISB4 = 1;
    AD1PCFGbits.PCFG4 = 0;
    AD1CON1bits.SSRC = 7;
    AD1CON1bits.CLRASAM = 1;
    AD1CON3bits.SAMC = 16;
    AD1CON2bits.SMPI = 8-1;
    AD1CHSbits.CH0SA = 4;
    AD1CON1bits.ON = 1;

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
        LATDbits.LATD6 = 1;
    }
    displayFlag = !displayFlag;

}

unsigned char toBcd(unsigned char value){
    return ((value/10)<<4)  + (value%10);
}

void configTimer2(){  //timer for 400Hz event
    T2CONbits.TCKPS = 0;
    PR2 = 1999999;
    TMR2 = 0;
    T2CONbits.TON = 1;

    IPC2bits.T2IP = 2;
    IEC0bits.T2IE = 1;
    IFS0bits.T2IF = 0;

    //PWM signal config
    OC2CONbits.OCM = 6; // PWM mode on OCx; fault pin disabled
    OC2CONbits.OCTSEL =0;// Use timer T2 as the time base for PWM generation
    OC2RS = 50000; // Ton constant
    OC2CONbits.ON = 1; // Enable OC1 module
    
}

void _int_(8) isr_T2(void){
    send2displays(v_dec); // Send voltage value to displays
    IFS0bits.T2IF = 0;  
}