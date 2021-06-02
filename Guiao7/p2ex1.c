#include <detpic32.h>


int main(void){
    // Configure Timer T3 with interrupts enabled, 100HZ
    T3CONbits.TCKPS = 2; // 1:32 prescaler (i.e. fout_presc = 625 KHz)
    PR3 = 49999; // Fout = 20MHz / (32 * (62499 + 1)) = 10 Hz
    TMR3 = 0; // Reset timer T3 count register
    T3CONbits.TON = 1; // Enable timer T3 (must be the last command of the
    // timer configuration sequence)

     //conf T3 interrupts						 
	IPC3bits.T3IP = 2;	// Interrupt priority (must be in range [1..6])
	IEC0bits.T3IE = 1;	// Enable timer T1 interrupts
	IFS0bits.T3IF = 0;	// Reset timer T1 interrupt flag 

    //configure PWM
    OC1CONbits.OCM = 6; // PWM mode on OCx; fault pin disabled
    OC1CONbits.OCTSEL =0;// Use timer T2 as the time base for PWM generation
    OC1RS = 12500; // Ton constant
    OC1CONbits.ON = 1; // Enable OC1 module 


return 0;
}    



void _int_(12) isr_T3(void){
	IFS0bits.T3IF = 0; //Reset timer T3 interrupt flag
}