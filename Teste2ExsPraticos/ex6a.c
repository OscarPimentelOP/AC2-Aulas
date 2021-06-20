/*a) Altere o programa que escreveu em 5b), e utilize o timer 3 para controlar a frequência de
atualização do contador. A atualização do contador deve ser feita por interrupção. */

#include <detpic32.h>

void delay(int ms);
void configT3();
void changeTimerT3Config(int freq);

int count = 0;

int main(void){

    configT3();
    EnableInterrupts();

    char number = '0', currentNumber = '0';

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
                number = currentNumber;
                break;
            case '\0':
                number = currentNumber;
                break;                          
        }
    }
    return 0;
}

void delay(int ms){
    for(; ms > 0; ms--){
        resetCoreTimer();
        while(readCoreTimer()<20000);
    }

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