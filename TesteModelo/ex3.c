#include <detpic32.h>

void delay(int ms);
void send2displays(unsigned char value);
unsigned char toBcd(unsigned char value);

int main(void) {
    // Configure the A/D module and port RB4 as analog input

    TRISBbits.TRISB4 = 1; // RBx digital output disconnected
    AD1PCFGbits.PCFG4= 0; // RBx configured as analog input (AN4)
    AD1CON1bits.SSRC = 7; // Conversion trigger selection bits: in this
    // mode an internal counter ends sampling and
    // starts conversion
    AD1CON1bits.CLRASAM = 1; // Stop conversions when the 1st A/D converter
    // interrupt is generated. At the same time,
    // hardware clears the ASAM bit
    AD1CON3bits.SAMC = 16; // Sample time is 16 TAD (TAD = 100 ns)
    AD1CON2bits.SMPI = 1-1; // Interrupt is generated after XX samples
    // (replace XX by the desired number of
    // consecutive samples)
    AD1CHSbits.CH0SA = 4; // replace x by the desired input
    // analog channel (0 to 15)
    AD1CON1bits.ON = 1; // Enable A/D converter
    // This must the last command of the A/D
    // configuration sequence

    int value = 20;
    while(1){

        if(x++ % 25 == 0){
            // Start conversion
        AD1CON1bits.ASAM = 1; // Start conversion
        while( IFS1bits.AD1IF == 0 );// Wait while conversion not done (AD1IF == 0)
        int *p = (int *)(&ADC1BUF0);
        int val_ad = 0;
        int freq_count = 0;
        for( i = 0; i < 16; i++ ){
            val_ad = val_ad + p[i*4];
            //printInt(p[i*4],0x0005000A);
        }
        val_ad = val_ad / (AD1CON2bits.SMPI +1); // Average of 4 samples
        
        freq_count = 1 + (val_ad/255);
        }
    send2displays(toBcd(value));
    delay(freq_count);
    value--;

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
    static char displayFlag = 0;

    //configure RB8-15 as outputs
    TRISB = (TRISB & 0x00FF);

    //configure RD5-6 as outputs
    TRISDbits.TRISD5 = 0;
    TRISDbits.TRISD6 = 0;

    //determine digits
    int digit_low = value & 0x0F;
    int digit_high = value >> 4;

    //send to display low
    if(displayFlag == 0){
        LATB = (LATB & 0x00FF) | (display7Scodes[digit_low] << 8);
        LATDbits.LATD5 = 1;
        LATDbits.LATD6 = 0;
    }
    //send to display high
    else{
        LATB = (LATB & 0x00FF) | (display7Scodes[digit_high] << 8);
        LATDbits.LATD5 = 0;
        LATDbits.LATD6 = 1;
    }
    displayFlag = !displayFlag;
}

unsigned char toBcd(unsigned char value){
    return ((value/10)<<4) + (value%10);
}