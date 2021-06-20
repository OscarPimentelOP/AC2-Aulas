/*b) Acrescente o módulo de visualização de modo a mostrar nos dois dígitos o valor atual da contagem,
quando é premida a tecla ENTER. A frequência de refrescamento dos displays deve ser 50 Hz. */

#include <detpic32.h>

void delay(int ms);
void configT3();
void changeTimerT3Config(int freq);
void send2displays(unsigned char value);
unsigned char toBcd(unsigned char value);

int count = 0;

int main(void){

    configT3();
    EnableInterrupts();

    char number = '0', currentNumber = '0';
    int value = 0;

    while(1){
        number = inkey();   

        switch(number){
            case '0':
                changeTimerT3Config(10);
                currentNumber = '0';
                break;
            case '1':
                changeTimerT3Config(20);
                currentNumber = '1';
                break;
            case '2':
                changeTimerT3Config(30);
                currentNumber = '2';
                break; 
            case '3':
                changeTimerT3Config(40);
                currentNumber = '3';
                break; 
            case '\n':
                printStr(" >> ");
                if(count >=0 && count<=9)
                    printInt(0, 10);
                printInt(count, 10 | 2 << 10);
                value = count;
                number = currentNumber;
                break;
            case '\0':
                number = currentNumber;
                break;                          
        }
        send2displays(toBcd(value));
        delay(2);  //50Hz
    }
    return 0;
}

void delay(int ms){
    for(; ms > 0; ms--){
        resetCoreTimer();
        while(readCoreTimer()<20000);
    }

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
    return ((value/10) << 4) + (value%10);
}

void _int_(12) isr_T3(void){

    printStr("\r");

    if(count >=0 && count<=9){
        printInt(0,10);
    }
    printInt(count, 10 | 2 << 10);

    count++;
    if(count == 100) count = 0;

    IFS0bits.T3IF = 0; //reset T3 interrupt flag

}

void configT3(){
    T3CONbits.TCKPS = 5;
    PR3 = 62499;
    TMR3 = 0;
    T3CONbits.TON = 1;

    IPC3bits.T3IP = 2;
    IEC0bits.T3IE = 1;
    IFS0bits.T3IF = 0;
}

void changeTimerT3Config(int freq){

    if(freq == 10){  //10Hz
        T3CONbits.TCKPS = 5;
        PR3 = 62499;
    }
    else if(freq == 20){  //20Hz
        T3CONbits.TCKPS = 4;
        PR3 = 62499;
    }
    else if(freq == 30){  //30Hz
        T3CONbits.TCKPS = 4;
        PR3 = 41666;
    }
    else if(freq == 40){  //40Hz
        T3CONbits.TCKPS = 3;
        PR3 = 62499;
    }
}