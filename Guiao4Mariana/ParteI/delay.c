/*
* gerar um atraso temporal programável múltiplo de 1ms.
*/
#include <detpic32.h>

void delay(int ms) {
    for(; ms > 0; ms--){
        resetCoreTimer();
        while(readCoreTimer() < 20000);
    }
}