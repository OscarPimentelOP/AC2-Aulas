#include <detpic32.h>

volatile int adc_value; 
int main(void){
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


    // Configure all (digital I/O, analog input, A/D module)
    TRISEbits.TRISE0 = 0; // RE0 as output


    // Configure interrupt system
    // config interrupts
	IPC6bits.AD1IP = 2; // configure priority of A/D interrupts
	IEC1bits.AD1IE = 1; // enable A/D interrupts
	IFS1bits.AD1IF = 0; // Reset AD1IF flag
	EnableInterrupts(); // Global Interrupt Enable
    // Start A/D conversion
    AD1CON1bits.ASAM = 1; // Start A/D conversion

    while(1) { } // all activity is done by the ISR
    return 0; 

}

// Interrupt Handler
 void _int_(27) isr_adc(void) // Replace VECTOR by the A/D vector
 // number - see "PIC32 family data
 // sheet" (pages 74-76)
 {
    LATEbits.LATE0 = 0;    
    int val_ad = 0; 
    int j;
	int *p = (int *)(&ADC1BUF0);
	for(j = 0; j<8; j++){
		adc_value = adc_value + p[j*4]; // Calculate buffer average (8 samples)
        //printInt(p[4*j],16); // Print ADC1BUF0 value // Hexadecimal (3 digits format)   
        printInt(adc_value,16);      
	}
    printStr("\n");
     LATEbits.LATE0 = 1;    
     AD1CON1bits.ASAM = 1; // Start A/D conversion

    IFS1bits.AD1IF = 0; // Reset AD1IF flag
 } 