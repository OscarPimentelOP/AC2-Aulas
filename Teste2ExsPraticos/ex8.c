/*Retome o exercício anterior. Faça as alterações que permitam processar o fim de conversão da ADC por
interrupção e acrescente o módulo de visualização de modo a imprimir nos dois displays de 7
segmentos o valor da frequência (com uma casa decimal). A frequência de atualização dos displays
deve ser 100 Hz, obtida por interrupção do timer 2.
 */


#include <detpic32.h>

volatile unsigned char voltage = 0;
int freq = 10;

void delay(int ms);
void send2displays(unsigned char value);
unsigned char toBcd(unsigned char value);
void configTimer2();

int main(void){
    //config ADC module
    TRISBbits.TRISB4 = 1;
    AD1PCFGbits.PCFG4 = 0;
    AD1CON1bits.SSRC = 7;
    AD1CON1bits.CLRASAM = 1;
    AD1CON3bits.SAMC = 16;
    AD1CON2bits.SMPI = 8-1;
    AD1CHSbits.CH0SA = 4;
    AD1CON1bits.ON = 1;

    IPC6bits.AD1IP = 2;
    IEC1bits.AD1IE = 1;
    IFS1bits.AD1IF = 0;

    //RB3-0 as inputs
    TRISB = (TRISB & 0xFFF0) | 0x000F;

    configTimer2();
    EnableInterrupts();
   
    while(1){
        AD1CON1bits.ASAM = 1;

        printStr("\nValues on switches: ");
        printInt(PORTBbits.RB0, 2);
        printInt(PORTBbits.RB1, 2);
        printInt(PORTBbits.RB2, 2);
        printInt(PORTBbits.RB3, 2);
        printStr("\n");
        
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
	for(j = 0; j<8; j++){
		val_ad = val_ad + p[j*4]; // Calculate buffer average (4 samples)
        //printInt(p[4*j],16); // Print ADC1BUF0 value // Hexadecimal (3 digits format)   
        //printInt(val_ad,16);    
        //printStr("-");
	}
    val_ad = val_ad/ (8); //media de 4 amostras
    voltage=(val_ad * 33 + 511)/1023; 
    freq = 1 + (voltage/255);
    IFS1bits.AD1IF = 0;

}

void configTimer2(){
    T2CONbits.TCKPS = 2;
    PR2 = 49999;
    TMR2 = 0;
    T2CONbits.TON = 1;

    IPC2bits.T2IP = 2;
    IEC0bits.T2IE = 1;
    IFS0bits.T2IF = 0;
    
}

void _int_(8) isr_T2(void){
    send2displays(toBcd(freq));
    IFS0bits.T2IF = 0;  
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
    return ((value/10)<<4)  + (value%10);
}

