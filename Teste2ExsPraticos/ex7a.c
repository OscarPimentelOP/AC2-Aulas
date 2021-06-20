/*a) Escreva um programa em linguagem C para escrever no ecrã (1 vez/segundo) o valor de cada um dos
bits lidos do porto de entrada. Por exemplo, se o porto de entrada tiver a combinação binária "1100",
no ecrã deverá aparecer: RB3: 1, RB2: 1, RB1: 0, RB0: 0 */

#include <detpic32.h>

void delay(int ms);
int main(void){

    //RB3-0 as inputs
    TRISB = (TRISB & 0xFFF0) | 0x000F;

    while(1){
        printStr("\nValues on switches: ");
        printInt(PORTBbits.RB0, 2);
        printInt(PORTBbits.RB1, 2);
        printInt(PORTBbits.RB2, 2);
        printInt(PORTBbits.RB3, 2);
        printStr("\n");

        delay(1000); 
    }


}

void delay(int ms){
    for(; ms > 0; ms--){
        resetCoreTimer();
        while(readCoreTimer() < 20000);
    }
}
