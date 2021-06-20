/*b) O valor do relógio pode ser ajustado através do potenciómetro que está ligado à ADC. Se o valor lido
pela ADC não estiver a ser alterado dentro de um intervalo de 10% do seu valor máximo (a ADC é de 10
bits, logo o valor máximo é 1023) o ajuste não é feito. Uma alteração de mais de 10% leva a que,
durante 10 segundos, o valor do relógio possa ser inicializado nos minutos entre 00 e 99, de forma
proporcional ao valor lido na ADC (0 – 00, 512 – 50, 1023 – 99 e restantes em proporção). */

#include <detpic32.h>

void delay(int ms);
void send2displays(unsigned char value);
unsigned char toBcd(unsigned char value);
void configTimer3Display();

volatile unsigned char msCount = 0;
volatile unsigned char segCount = 0;
volatile unsigned char minCount = 0;
volatile unsigned char voltage = 0;
volatile unsigned char voltageAtual = 0;

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
    
    configTimer3Display();
    EnableInterrupts();

    char input;


    while(1){
        AD1CON1bits.ASAM = 1;
        input = inkey();
        if(msCount == 10){
            segCount = segCount + 1;
            msCount = 0;
        }
        if(segCount == 60){
            minCount = minCount + 1;
            segCount = 0;
        }
        if(minCount == 60){
            msCount = 0;
            segCount = 0;
            minCount = 0;
        }
        if(input == 'r'){
            msCount = 0;
            segCount = 0;
            minCount = 0;
        }
        if(input == 'u'){
            msCount = 8;
            segCount = 59;
            minCount = 59;
        }
        
    }

    



    return 0;
}

void delay(int ms){
    for(; ms > 0; ms--){
        resetCoreTimer();
        while(readCoreTimer() < 20000);
    }
}

void send2displays(unsigned char value) {

  static const char display7Scodes[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x67, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};
  static char displayFlag   = 0;

  // Configure RB8 to RB15 as outputs
  TRISB = (TRISB & 0x00FF);

  // Configure RD5 and RD6 as outputs
  TRISDbits.TRISD5 = 0;
  TRISDbits.TRISD6 = 0;

  // Determine digits
  int digit_low = value & 0x0F;     
  int digit_high = value >> 4;      

  // Send digit_low to display low
  if (displayFlag == 0) {
    LATB = (LATB & 0x00FF) | (display7Scodes[digit_low] << 8);
    LATDbits.LATD5 = 1;
    LATDbits.LATD6 = 0;
  }

  // Send digit_high to display high
  else {
    LATB = (LATB & 0x00FF) | (display7Scodes[digit_high] << 8);
    LATDbits.LATD5 = 0;
    LATDbits.LATD6 = 1;
  }
  displayFlag = !displayFlag;

}

unsigned char toBcd(unsigned char value){
    return ((value/10) << 4) + (value%10);
}

void configTimer3Display(){ //10hz
    T3CONbits.TCKPS = 5;
    PR3 = 62499;
    TMR3 = 0;
    T3CONbits.TON = 1;

    IPC3bits.T3IP = 2;
    IEC0bits.T3IE = 1;
    IFS0bits.T3IF = 0;

}

void _int_(12) isr_T3(void){
    printInt(minCount, 10 | 2 << 10);
    printStr(":");
    printInt(segCount, 10 | 2 << 10);
    printStr(":");
    printInt(msCount, 10  | 2 << 10);
    printStr("\n");
    msCount = msCount + 1;
    if((voltage < (voltageAtual-1023*0.10)) || (voltage > (1023*0.10 + voltageAtual))){
        printStr("PORTUGAL CRLH");
        delay(10000);
        voltageAtual = voltage;
        minCount = voltage;
    }
    IFS0bits.T3IF = 0; 
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
    voltage=(val_ad * 1024 + 511)/1023; 
    //printInt(voltage, 10);
    //printStr("\n");
    IFS1bits.AD1IF = 0;

}




