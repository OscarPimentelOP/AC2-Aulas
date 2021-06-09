#include<detpic32.h>

void putc(char byte2send);
void configUart(unsigned int baud, char parity, unsigned int stopbits);



int main(void){
    configUart(115200, 'N', 1);
    EnableInterrupts();
    while(1);
    return 0;
}

void putc(char byte2send)
{
    while (U2STAbits.UTXBF == 1);
    //wait while UTXBF==1
    U2TXREG = byte2send;
}

void configUart(unsigned int baud, char parity, unsigned int stopbits)
{
     if (baud < 600 || baud > 115200)
        baud = 115200;
    if (parity != 'N' || parity != 'E' || parity != 'O')
        parity = 'N';
    if (stopbits != 1 || stopbits != 2)
        stopbits = 1;

    U2BRG = ((PBCLK + 8 * baud) / (16 * baud)) - 1; //configure baudrate generator
    U2MODEbits.BRGH = 0;                            //16x baud clk enabled

    //config parity,  nºbits = 8
    switch (parity)
    {
    case 'N':
        U2MODEbits.PDSEL = 0; //8 bits, no parity
        break;
    case 'E':
        U2MODEbits.PDSEL = 1; //8 bits, even parity
        break;
    case 'O':
        U2MODEbits.PDSEL = 2; //8 bits, odd parity
        break;
    }

    //config stopbits
    switch (stopbits)
    {
    case 2:
        U2MODEbits.STSEL = 1; //2 stop bits
        break;
    case 1:
        U2MODEbits.STSEL = 0; //1 stop bit
        break;
    }

    U2STAbits.URXEN = 1; //UART2 receiver is enabled
    U2STAbits.UTXEN = 1; //UART2 transmitter is enabled
    IPC8bits.U2IP = 2;   //priority 2
    IEC1bits.U2RXIE = 1; //interrupts enabled
    IFS1bits.U2RXIF = 0; //clear reception interrupts fla
    U2MODEbits.ON = 1;   //UART2 enabled
    

}

void _int_(32) isr_uart2(void)
{
    putc(U2RXREG);
    
    IFS1bits.U2RXIF = 0; // Clear UART2 rx interrupt flag
}

