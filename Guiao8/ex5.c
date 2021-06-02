#include <detpic32.h>

void puts(char *str);
void delay(int ms);
void configureUART2(int baudrate,char parity,int data_bits,int stop_bits);
void putc(char byte2send);


int main(void){
    /*
    // Configure UART2:
    // 1 - Configure BaudRate Generator 
    U2MODEbits.BRGH = 0;
    U2BRG = ((fPBCLK + 8 * baudrate) / (16 * baudrate)) – 1 

    // 2 – Configure number of data bits, parity and number of stop bits (see U1MODE register)
    if(data_bits == 9){
        U2MODEbits.PDSEL = 3; // 9 data bits, no parity -> 11 (page 21-6 from manual)
    }
    else{
        if(parity == 0){   //no parity -> 00
            U2MODEbits.PDSEL = 0;
        }
        else if(parity == 1){ // even parity -> 01
            U2MODEbits.PDSEL = 1;
        }
        else{  //odd parity -> 02
            U2MODEbits.PDSEL = 2;
        }
    }

    //configure stop bits
    U2MODEbits.STSEL = stop_bits - 1;


    // 3 – Enable the trasmitter and receiver modules (see register U1STA)
    U2STAbits.UTXEN = 1; //page 21-5 from UART manual
    U2STAbits.URXEN = 1; //page 21-8 from UART manual
    //U1STAbits.UxSTA =

    // 4 - Enable UART2
    U2MODEbits.ON = 1; 
    */
    //configureUART2(115200, 'N', 8, 1);
    //test with command "pterm 1200,O,8,2"
    configureUART2(1200, 'O', 8, 2);
    //configureUART2(600, 'N', 8, 1);
    while(1){
    puts("AC2 é uma merda\n");
    delay(1000);	// wait 1 s
 } 


    return 0;
}


void puts(char *str)
 {
    // use putc() function to send each charater ('\0' should not
	// be sent
	while(*str != '\0'){
		putc(*str);
		str++;
	}	
 } 

void delay(int ms){
	for( ; ms>0 ; ms--){
		resetCoreTimer();
		while(readCoreTimer()<20000);
	}
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

void putc(char byte2send)
 {
    // wait while UTXBF == 1    
    while(U2STAbits.UTXBF==1);

    // Copy byte2send to the UxTXREG register
    U2TXREG = byte2send;	
 } 



