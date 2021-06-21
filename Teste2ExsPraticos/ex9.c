/*Retome o exercício anterior. Configure a UART1 com os seguintes parâmetros de comunicação: 1200
bps, 8 data bits, sem paridade, 1 stop bit. Escreva as funções de transmissão de um carater e de
transmissão de uma string que lhe permitam substituir a system call que usou para imprimir o valor de
cada um dos bits do porto de entrada.
Para o teste deste exercício não se esqueça de executar o “pterm” com os mesmos parâmetros de
comunicação com que configurou a UART.
 */


#include <detpic32.h>

volatile unsigned char voltage = 0;
int freq = 10;

void delay(int ms);
void send2displays(unsigned char value);
unsigned char toBcd(unsigned char value);
void configureUART2(int baudrate,char parity,int data_bits,int stop_bits);
void putc(char byte2send);
void puts(char *str);
void portValue(unsigned char value);
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
    configureUART2(115200, 'N', 8, 1);
    EnableInterrupts();
   
    while(1){
        AD1CON1bits.ASAM = 1;

        puts("\nValues on switches: ");
        portValue(PORTBbits.RB0);
        portValue(PORTBbits.RB1);
        portValue(PORTBbits.RB2);
        portValue(PORTBbits.RB3);
        puts("\n");

        puts("\nFreq: ");
        printInt(freq, 10);

        //freq = 10 + (voltage*10/255);
        
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

void configureUART2(int baudrate,char parity,int data_bits,int stop_bits){
 	// Configure UART1:
 	// 1 - Configure BaudRate Generator
 	U2MODEbits.BRGH = 0; // pq é fator de divisao x16 se fosse x4 era 1
 	U2BRG = ((PBCLK + 8*baudrate)/(16*baudrate))-1; 

 	// 2 – Configure number of data bits, parity and number of stop bits
 	// (see U1MODE register)
 	//databits
 	if(data_bits == 9){
 		U2MODEbits.PDSEL = 3; // 9 data bits, no parity -> 11 (page 21-6 from manual)
 	}
 	else{
 		if(parity == 'N'){ //NOT EQUAL
 			U2MODEbits.PDSEL = 0;
 		}
 		else if(parity == 'E'){ //EQUAL
 			U2MODEbits.PDSEL = 1;
 		}
 		else{
 			U2MODEbits.PDSEL = 2;
 		} 	
 	}
 	//nº stopbits
 	U2MODEbits.STSEL = stop_bits - 1;

 	// 3 – Enable the trasmitter and receiver modules (see register U1STA)
 	U2STAbits.URXEN = 1; // RX enable, page 21-5 from UART manual
 	U2STAbits.UTXEN = 1; // TX enable, page 21-8 from UART manual
 	// 4 – Enable UART1 (see register U1MODE)
 	U2MODEbits.ON = 1; 
} 

void putc(char byte2send){
    // wait while UTXBF == 1
    while(U2STAbits.UTXBF == 1);
    // Copy byte2send to the UxTXREG register
    U2TXREG = byte2send;
}
void puts(char *str){
    // use putc() function to send each charater ('\0' should not be sent)
    int i=0;
    while(str[i] != '\0'){
        putc(str[i]);
        i++;
    } 
}

void portValue(unsigned char value){
    if((value & 0x0001) == 0x0000)
        putc('0');
    else
        putc('1');
}

