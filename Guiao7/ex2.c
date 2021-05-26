 #include <detpic32.h>
 
 void main(void) {
 // Configure Timer T3 with interrupts enabled
  T3CONbits.TCKPS = 7; // 1:32 prescaler (i.e. fout_presc = 625 KHz)
  PR3 = 39062; // Fout = 20MHz / (32 * (62499 + 1)) = 10 Hz
  TMR3 = 0; // Reset timer T3 count register
  T3CONbits.TON = 1; // Enable timer T3 (must be the last command of the
 // timer configuration sequence)

    //configure interrupts						 
	IPC3bits.T3IP = 2;	// Interrupt priority (must be in range [1..6])
	IEC0bits.T3IE = 1;	// Enable timer T3 interrupts
	IFS0bits.T3IF = 0;	// Reset timer T3 interrupt flag 
    EnableInterrupts();
    while(1);
 }
 void _int_(12) isr_T3(void) // Replace VECTOR by the timer T3
 // vector number
 {
     static int count = 0;
 if(count == 2){ //1Hz, one each 2 interrupts
		putChar('.');
		count = 0;
	}
    count++;

 // Reset T3 interrupt flag
 IFS0bits.T3IF = 0;
 } 