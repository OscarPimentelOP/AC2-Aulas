/*
 * Selecionando em sequência o "display low" e o "display high" envie para os portos RB8 a
 * RB14, em ciclo infinito e com uma frequência de 2 Hz, a sequência binária que ativa os
 * segmentos do display pela ordem a, b, c, d, e, f, g, a, ...; o período de 0.5s deve ser obtido
 * através da função delay()
 */

#include <detpic32.h>
#include "delay.c"

int main(void){
    unsigned char segment;
    TRISB = (TRISB & 0x80FF);               // portos RB8 a RB15 como saída
    TRISD = (TRISD & 0xFF9F);               // portos RD5 e RD6 como saída
    LATB = (LATB & 0x0000);                 //sets the output of B8-B14 as '0'
    LATDbits.LATD6 = 1;                     // display high active
    LATDbits.LATD5 = 0;                     // display low inactive1

    while(1){
        LATDbits.LATD6 = !LATDbits.LATD6;   //
        LATDbits.LATD5 = !LATDbits.LATD5;   // toggle display selection
        segment = 1;
        int i;
        for(i=0; i < 7; i++)
        {
            LATB = (LATB & 0x0000) | (segment << 8);
            delay(1000);                    // send "segment" value to display
                                            // wait 0.5 second
            segment = segment << 1;
        }
    }
}
