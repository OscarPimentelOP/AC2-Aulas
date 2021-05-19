#include <detpic32.h>

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
    // digital I/0
	TRISBbits.TRISB6 = 0; // RB6 as output

    // Configure interrupt system
    IPC6bits.AD1IP = 2; // configure priority of A/D interrupts (entre 1 e 6) o 0 e o 7 nao sao ultizados
	IFS1bits.AD1IF = 0; // clear A/D interrupt flag 
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
    int i;
	int *p;
	p= &ADC1BUF0;
	AD1CON1bits.ASAM = 1; // Start A/D conversion
	for(i = 0; i<AD1CON2bits.SMPI+1;i++){
		printInt(p[4*i],16);// Print ADC1BUF0 value // Hexadecimal (3 digits format)
	}
 	printStr("\n");

    IFS1bits.AD1IF = 0; // Reset AD1IF flag
 } 