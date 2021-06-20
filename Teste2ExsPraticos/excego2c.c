/*c) Passe a permitir que, na placa de teste, os parâmetros da porta série sejam configuráveis através dos
dip switches da forma indicada abaixo. O número de bits não é programável devendo estar fixo a 8 bits.
O relógio deve arrancar quando o DP1 é posto a "1"
• DP1: "0" em programação; "1" arranca o relógio
• DP2: "0" sem paridade; "1" com paridade
• DP3: "0" paridade par; "1" paridade ímpar
• DP4: cada transição de "0" para "1" faz alterar o baud rate o qual deve variar entre os seguintes
valores, em sequência: 600, 1200, 2400, 4800, 9600, 19200, 57600 e 115200. A sinalização do
baud rate é feita pela ativação de um dos 8 leds da placa, respetivamente LED7, … LED0. */

#include <detpic32.h>

void delay(int ms);
void send2displays(unsigned char value);
unsigned char toBcd(unsigned char value);
void configTimer3Display();
void configureUART2(int baudrate,char parity,int data_bits,int stop_bits);

volatile unsigned char msCount = 0;
volatile unsigned char segCount = 0;
volatile unsigned char minCount = 0;

int main(void){

    //RB3-0 as inputs
    TRISB = (TRISB & 0xFFF0) | 0x000F;
    
    configTimer3Display();
    EnableInterrupts();

    int baudrates[] = {600, 1200, 2400, 4800, 9600, 19200, 57600, 115200};
    int pos = 0;
    static char flag;

    char input;


    while(1){
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

    if(PORTBbits.RB1 == 1){
         if(PORTBbits.RB2 == 0){
             configureUART2(baudrates[pos], 'E', 8, 1);
         }
         else{
             configureUART2(baudrates[pos], 'O', 8, 1);
         }
            
    }
    else{
        configureUART2(baudrates[pos], 'N',8, 1); 
    }
    if(PORTBbits.RB3 == 1){
        flag = 1;
        if(flag == 1){
           pos = pos +1; 
        }

    }
    flag = !flag;

    //TODO: acabar de fazer o DP4 pq a flag não temos a certeza se vai funcionar. 

    



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

    while(PORTBbits.RB0 == 0);
    printInt(minCount, 10 | 2 << 10);
    printStr(":");
    printInt(segCount, 10 | 2 << 10);
    printStr(":");
    printInt(msCount, 10  | 2 << 10);
    printStr("\n");
    msCount = msCount + 1;
    IFS0bits.T3IF = 0; 
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




