#include <detpic32.h>

void delay(int ms);

int main(void){
    TRISE = (TRISE & 0xFFF0); //RE0-3 as outputs
    LATE = (LATE & 0xFFF0);

    int count = 0;

    while(1){
        delay(250);
        LATE = count;
        count++;
    }
}


void delay(int ms){
    for(; ms > 0; ms--){
        resetCoreTimer();
        while (readCoreTimer() < 20000);
    }
}