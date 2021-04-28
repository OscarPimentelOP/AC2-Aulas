/**
 * A  inicialização dos portos RD6 e RD5 (a '1' e a '0', respetivamente) e a inversão da seleção
 * do display também podem ser feitas do modo que a seguir se indica. Analise as duas linhas
 * de código e tire conclusões.
 */


#include <detpic32.h>
#include "delay.c"

int main(void){
    unsigned char segment;
    TRISB = (TRISB & 0x80FF);               // portos RB8 a RB15 como saída
    TRISD = (TRISD & 0xFF9F);               // portos RD5 e RD6 como saída
    LATB = (LATB & 0x0000);                 //sets the output of B8-B14 as '0'
    LATD = (LATD & 0xFF9F) | 0x0040;       // display high active, low inactive

    while(1){
        LATD = LATD ^ 0x0060;                       // toggle display selection
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

