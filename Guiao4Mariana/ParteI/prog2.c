/*
 * contador crescente, atualizado a uma frequência de 4Hz
 */

#include <detpic32.h>
#include "delay.c"

void main(void){
    LATE = (LATE & 0x0000);             // limpar
    TRISE = (TRISE & 0x0000);           // colocar tudo como saida
    int count = 0;

    while(1){
        LATE = count;
        count++;
        delay(250);
    }
}