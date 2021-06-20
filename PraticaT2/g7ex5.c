#include <detpic32.h>

volatile int val_ad; 
volatile int voltage = 0; // Global variable

void delay(int ms);
void send2displays(unsigned char value);
unsigned char toBcd(unsigned char value);

int main(void){
    //config timer 1 -> 4Hz
    T1CONbits.TCKPS = 3; // 1:32 prescaler (i.e. fout_presc = 625 KHz)
    PR1 = 19531; // Fout = 20MHz / (32 * (62499 + 1)) = 10 Hz
    TMR1 = 0; // Reset timer T2 count register
    T1CONbits.TON = 1; // Enable timer T2 (must be the last command of the
    // timer configuration sequence)

    //configure interrupts
    IPC1bits.T1IP = 2; // Interrupt priority (must be in range [1..6])
    IEC0bits.T1IE = 1; // Enable timer T1 interrupts
    IFS0bits.T1IF = 0; // Reset timer T1 interrupt flag

    //config timer 3 -> 100Hz
    T3CONbits.TCKPS = 2; // 1:32 prescaler (i.e. fout_presc = 625 KHz)
    PR3 = 49999; // Fout = 20MHz / (32 * (62499 + 1)) = 10 Hz
    TMR3 = 0; // Reset timer T2 count register
    T3CONbits.TON = 1; // Enable timer T2 (must be the last command of the
    // timer configuration sequence)

    //configure interrupts
    IPC3bits.T3IP = 2; // Interrupt priority (must be in range [1..6])
    IEC0bits.T3IE = 1; // Enable timer T3 interrupts
    IFS0bits.T3IF = 0; // Reset timer T3 interrupt flag


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
    AD1CON2bits.SMPI = 8-1; // Interrupt is generated after XX samples
    // (replace XX by the desired number of
    // consecutive samples)
    AD1CHSbits.CH0SA = 4; // replace x by the desired input
    // analog channel (0 to 15)
    AD1CON1bits.ON = 1; // Enable A/D converter
    // This must the last command of the A/D
    // configuration sequence


    // Configure all (digital I/O, analog input, A/D module)
    TRISEbits.TRISE0 = 0; // RE0 as output

    // Configure interrupt system
    // config interrupts
	IPC6bits.AD1IP = 2; // configure priority of A/D interrupts
	IEC1bits.AD1IE = 1; // enable A/D interrupts
	IFS1bits.AD1IF = 0; // Reset AD1IF flag

    EnableInterrupts(); // Global Interrupt Enable
    while(1); 

    return 0;
}

void _int_(4) isr_T1(void) //interrupt timer 1
 {
    AD1CON1bits.ASAM = 1;	// Start A/D conversion
 	IFS0bits.T1IF = 0;	// Reset T1IF flag
 } 

 void _int_(12) isr_T3(void)
 {
 // Send "voltage" global variable to displays
 // Reset T3IF flag
 }

 void _int_(27) isr_adc(void)
 {
 LATEbits.LATE0 = 0;    
    int val_ad = 0; 
    int j;
	int *p = (int *)(&ADC1BUF0);
	for(j = 0; j<8; j++){
		val_ad = val_ad + p[j*4]; // Calculate buffer average (8 samples)
        //printInt(p[4*j],16); // Print ADC1BUF0 value // Hexadecimal (3 digits format)   
        printInt(val_ad,16);    
        printStr("-");
	}
    val_ad = val_ad/ (8); //media de 8 amostras
    voltage=(val_ad * 33 + 511)/1023; 
    printStr("\n");
     LATEbits.LATE0 = 1;    
     AD1CON1bits.ASAM = 1; // Start A/D conversion

    IFS1bits.AD1IF = 0; // Reset AD1IF flag
 }

 void delay(int ms){
     for(; ms > 0; ms--){
         resetCoreTimer();
         while(readCoreTimer()< 20000);
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
    int digit_low = value & 0x0F;
    int digit_high = value >> 4;

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
     return ((value/10)<<4) + (value%10);
 }

 