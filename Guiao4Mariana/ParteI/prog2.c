/*
 * contador crescente, atualizado a uma frequência de 4Hz
 */

#include <detpic32.h>
#include "delay.c"

void main(void){
    LATE = (LATE & 0x0000);
    TRISE = (TRISE & 0x0000);
    int count = 0;

    while(1){
        LATE = count;
        count++;
        delay(250);
    }
}