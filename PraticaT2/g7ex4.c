#include <detpic32.h>


void main(void){
    
    //config timer 1
    T1CONbits.TCKPS = 3; // 1:32 prescaler (i.e. fout_presc = 625 KHz)
    PR1 = 39062; // Fout = 20MHz / (32 * (62499 + 1)) = 10 Hz
    TMR1 = 0; // Reset timer T2 count register
    T1CONbits.TON = 1; // Enable timer T2 (must be the last command of the
    // timer configuration sequence)

    //configure interrupts
    IPC1bits.T1IP = 2; // Interrupt priority (must be in range [1..6])
    IEC0bits.T1IE = 1; // Enable timer T1 interrupts
    IFS0bits.T1IF = 0; // Reset timer T1 interrupt flag


    //config timer 3
    T3CONbits.TCKPS = 5; // 1:32 prescaler (i.e. fout_presc = 625 KHz)
    PR3 = 62499; // Fout = 20MHz / (32 * (62499 + 1)) = 10 Hz
    TMR3 = 0; // Reset timer T2 count register
    T3CONbits.TON = 1; // Enable timer T2 (must be the last command of the
    // timer configuration sequence)

    //configure interrupts
    IPC3bits.T3IP = 2; // Interrupt priority (must be in range [1..6])
    IEC0bits.T3IE = 1; // Enable timer T3 interrupts
    IFS0bits.T3IF = 0; // Reset timer T3 interrupt flag

     EnableInterrupts();
     while(1);
}

void _int_(4) isr_T1(void)  //interrupt timer 1
 {
    putChar('1');// print character '1'
    IFS0bits.T1IF = 0;// Reset T1IF flag
 } 

 void _int_(12) isr_T3(void)   //interrupt timer 3
 {
    putChar('3');// print character '3'
    IFS0bits.T3IF = 0;// Reset T3IF flag
 }
 