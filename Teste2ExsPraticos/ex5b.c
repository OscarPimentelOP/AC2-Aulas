/*b) Faça as alterações que achar necessárias de modo a que, para além da especificação anterior,
quando o utilizador carrega nas teclas 0 a 4 a frequência de atualização da contagem seja modificada
de acordo com a seguinte expressão: freq=10 * (1 + tecla_carregada).
Sempre que o utilizador carregar na tecla ENTER deverá ser impresso o valor atual da contagem, a
seguir ao valor do contador (por exemplo: 76 >> 76).
Utilize o core timer do PIC32 para controlar a frequência de atualização do contador.*/

#include <detpic32.h>

void delay(int ms);

int main(void){

    int count = 0;
    int freq = 10;
    char number = '0', currentNumber = '0';

    while(1){
        number = inkey();

        printStr("\r");

        if(count >=0 && count<=9){
            printInt(0,10);
        }
        printInt(count, 10 | 2 << 10);


        switch(number){
            case '0':
                freq = 10;
                currentNumber = '0';
                break;
            case '1':
                freq = 20;
                currentNumber = '1';
                break;
            case '2':
                freq = 30;
                currentNumber = '2';
                break; 
            case '3':
                freq = 40;
                currentNumber = '3';
                break; 
            case '4':
                freq = 50;
                currentNumber = '4';
                break;  
            case '5':
                freq = 60;
                currentNumber = '5';
                break;
            case '6':
                freq = 70;
                currentNumber = '6';
                break;  
            case '7':
                freq = 80;
                currentNumber = '7';
                break;  
            case '8':
                freq = 90;
                currentNumber = '8';
                break;
            case '9':
                freq = 100;
                currentNumber = '9';
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
        count++;
        if(count == 99) count = 0;

        delay(1000/freq);
    }
    return 0;
}

void delay(int ms){
    for(; ms > 0; ms--){
        resetCoreTimer();
        while(readCoreTimer()<20000);
    }

}