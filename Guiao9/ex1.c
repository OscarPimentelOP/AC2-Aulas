#include<detpic32.h>

#define DisableUart2RxInterrupt() IEC0bits.U1RXIE = 0
#define EnableUart2RxInterrupt() IEC0bits.U1RXIE = 1
#define DisableUart2TxInterrupt() IEC0bits.U1TXIE = 0
#define EnableUart2TxInterrupt() IEC0bits.U1TXIE = 1
 //RX and TX buffer size
#define BUF_SIZE 8
#define INDEX_MASK 7 


typedef struct{
    unsigned char data[BUF_SIZE];
    unsigned int head;
    unsigned int tail;
    unsigned int count;
 } circularBuffer; 

 volatile circularBuffer txb; // Transmission buffer
 volatile circularBuffer rxb; // Reception buffer

void comDrv_flushRx(void);
void comDrv_flushTx(void);
void comDrv_putc(char ch);
void comDrv_puts(char *s);
void comDrv_config(unsigned int baud, char parity, unsigned int stopbits);


int main(void){
    comDrv_config(115200,'N',1); // default "pterm" parameters
    // with TX and RX interrupts disabled
    comDrv_flushRx();
    comDrv_flushTx();
    EnableInterrupts();
    while(1)
    comDrv_puts("Teste do bloco de transmissao do device driver!...\n");

    return 0;
}


void comDrv_flushRx(void)
 {
 // Initialize variables of the reception buffer
    rxb.head =0;
    rxb.tail=0;
    rxb.count =0;
 }

 void comDrv_flushTx(void)
 {
 // Initialize variables of the transmission buffer
    txb.head =0;
    txb.tail=0;
    txb.count =0;
 }  

 void comDrv_putc(char ch)
 {
    while(txb.count == BUF_SIZE){} // Wait while buffer is full
    txb.data[txb.tail] = ch; // Copy character to the transmission
    // buffer at position "tail"
    txb.tail = (txb.tail + 1) & INDEX_MASK; // Increment "tail" index
    // (mod. BUF_SIZE)
    DisableUart2TxInterrupt(); // Begin of critical section
    // Increment "count" variable
    txb.count++; 
    EnableUart2TxInterrupt(); // End of critical section
 }   

void comDrv_puts(char *s)
 {
    while (*s != '\0'){
    comDrv_putc(*s);
        s++;
    }
 } 

 void comDrv_config(unsigned int baud, char parity, unsigned int stopbits){
    if (baud < 600 || baud > 115200)
        baud = 115200;
    if(parity!='N' || parity!='E' || parity!='O')
        parity = 'N';
    if(stopbits!=1 || stopbits!=2)
        stopbits = 1;
    
    U2BRG = ((PBCLK+8*baud)/(16*baud))-1;       //configure baudrate generator
    U2MODEbits.BRGH = 0;                        //16x baud clk enabled

    //config parity,  nºbits = 8
    switch(parity){
        case 'N':
            U2MODEbits.PDSEL = 0;               //8 bits, no parity
            break;
        case 'E':
            U2MODEbits.PDSEL = 1;               //8 bits, even parity
            break;
        case 'O':
            U2MODEbits.PDSEL = 2;               //8 bits, odd parity
            break;
    }

    //config stopbits
    switch(stopbits){
        case 2:
            U2MODEbits.STSEL = 1;               //2 stop bits
            break;
        case 1:
            U2MODEbits.STSEL = 0;               //1 stop bit
            break;
    }

    U2STAbits.URXEN = 1;                        //UART1 receiver is enabled
    U2STAbits.UTXEN = 1;                        //UART1 transmitter is enabled

    IPC8bits.U2IP = 2;                          //interrupts priority is 2
    IEC1bits.U2RXIE = 1;                        //Rx interrupts enabled
    IEC1bits.U2TXIE = 1;                        //Tx interrupts enabled
    IFS1bits.U2RXIF = 0;                        //reset Rx interrupt flag
    IFS1bits.U2TXIF = 0;                        //reset Tx interrupt flag

    U2MODEbits.ON = 1;                          //UART1 enabled

} 

void _int_(32) isr_uart2(void){
  if (IFS1bits.U2TXIF == 1)   // if U2TXIF is set
    {

        if (txb.count > 0) // if "count" variable (transmission buffer, txb) is greater than 0
        {
            U2TXREG =  txb.data[txb.head];          // Copy character pointed by "head" to U2TXREG register
            txb.head = (txb.head+1) & INDEX_MASK;   // Increment "head" variable (mod BUF_SIZE)
            txb.count--;                            // Decrement "count" variable
        }

        if (txb.count == 0) // if "count" variable is 0 then
            DisableUart2TxInterrupt();

        IFS1bits.U2TXIF = 0;       // Reset UART2 TX interrupt flag
    }

}    


