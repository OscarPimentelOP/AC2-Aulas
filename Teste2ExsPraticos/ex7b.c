/*b) Faça as alterações que achar convenientes de modo a que a taxa de atualização da informação
escrita no ecrã seja dada pela seguinte expressão:
freq = 1 + (valADC/255) hz
Tenha em consideração que a ADC que utiliza é uma ADC de 10 bits e, por esse motivo, o valor da
conversão poderá variar entre 0 e 1023. */


#include <detpic32.h>

volatile unsigned char voltage = 0;

void delay(int ms);

int main(void){
    //config ADC module
    TRISBbits.TRISB4 = 1;
    AD1PCFGbits.PCFG4 = 0;
    AD1CON1bits.SSRC = 7;
    AD1CON1bits.CLRASAM = 1;
    AD1CON3bits.SAMC = 16;
    AD1CON2bits.SMPI = 4-1;
    AD1CHSbits.CH0SA = 4;
    AD1CON1bits.ON = 1;

    IPC6bits.AD1IP = 2;
    IEC1bits.AD1IE = 1;
    IFS1bits.AD1IF = 0;

    //RB3-0 as inputs
    TRISB = (TRISB & 0xFFF0) | 0x000F;

    EnableInterrupts();
    int freq = 10;

    while(1){
        AD1CON1bits.ASAM = 1;

        printStr("\nValues on switches: ");
        printInt(PORTBbits.RB0, 2);
        printInt(PORTBbits.RB1, 2);
        printInt(PORTBbits.RB2, 2);
        printInt(PORTBbits.RB3, 2);
        printStr("\n");

        freq = 1 + (voltage/255);
        delay(1000/freq); 
    }

}

void delay(int ms){
    for(; ms > 0; ms--){
        resetCoreTimer();
        while(readCoreTimer() < 20000);
    }
}

void _int_(27) rsi_ad1(void){ //interrupt ADC
    int val_ad = 0; 
    int j;
	int *p = (int *)(&ADC1BUF0);
	for(j = 0; j<4; j++){
		val_ad = val_ad + p[j*4]; // Calculate buffer average (4 samples)
        //printInt(p[4*j],16); // Print ADC1BUF0 value // Hexadecimal (3 digits format)   
        //printInt(val_ad,16);    
        //printStr("-");
	}
    val_ad = val_ad/ (4); //media de 4 amostras
    voltage=(val_ad * 33 + 511)/1023; 
    IFS1bits.AD1IF = 0;

}