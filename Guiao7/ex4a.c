 #include <detpic32.h>
 
 void main(void) {
   
    //configure timer 1 
    T1CONbits.TCKPS = 8;	//(PBCLK / ((PRmax-1)* fout))  <= kpresc  ->  Kpresc >= 20*10⁶ / ((2¹⁶ + 1)*2) = 152 logo 256
 	PR1 = 39062;			// PRX = (PBCLK)/((K*Fout)-1) ->PRX = 20*10⁶/(256*2)-1
 	TMR1 = 0;				// Reset timer T3 count register
	T1CONbits.TON = 1;		// Enable timer T3 (must be the last command of the timer configuration sequence)
   
    // Configure Timer T3 with interrupts enabled
    T3CONbits.TCKPS = 7; // 1:32 prescaler (i.e. fout_presc = 625 KHz)
    PR3 = 62499; // Fout = 20MHz / (32 * (62499 + 1)) = 10 Hz
    TMR3 = 0; // Reset timer T3 count register
    T3CONbits.TON = 1; // Enable timer T3 (must be the last command of the
    // timer configuration sequence)

    //configure interrupts						 
	IPC3bits.T3IP = 2;	// Interrupt priority (must be in range [1..6])
	IEC0bits.T3IE = 1;	// Enable timer T3 interrupts
	IFS0bits.T3IF = 0;	// Reset timer T3 interrupt flag 
    IPC1bits.T1IP = 2;	// Interrupt priority (must be in range [1..6])
	IEC0bits.T1IE = 1;	// Enable timer T3 interrupts
	IFS0bits.T1IF = 0;	// Reset timer T3 interrupt flag 
    
    EnableInterrupts();
    while(1);
 }
 void _int_(12) isr_T3(void) // Replace VECTOR by the timer T3
 // vector number
 {
    putChar('3');
 	IFS0bits.T3IF = 0;	// Reset timer T3 interrupt flag 
 } 

 void _int_(4) isr_T1(void) // Replace VECTOR by the timer T1
 // vector number
 {
    putChar('1');
 	IFS0bits.T1IF = 0;	// Reset timer T1 interrupt flag 

 } 