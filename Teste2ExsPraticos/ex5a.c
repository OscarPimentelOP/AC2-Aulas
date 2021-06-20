/*Escreva um programa em linguagem C que implemente, em ciclo infinito, um contador circular,
módulo 100, com uma frequência de atualização de contagem de 10 Hz. O valor do contador deve ser
impresso sempre na mesma linha do ecrâ, formatado com 2 dígitos (nota: para começar a impressão
sempre na mesma linha use o carater '\r'). */

#include <detpic32.h>

void delay(int ms);

int main(void){
    int count = 0;

    while(1){
        printStr("\r");
        printInt(count, 10 | 2 << 10);

        count++;

        if(count == 99){
            count = 0;
        }
        delay(100);

    }
    return 0;
}



void delay(int ms){
    for(; ms> 0; ms--){
        resetCoreTimer();
        while(readCoreTimer() < 20000);
    }
}


