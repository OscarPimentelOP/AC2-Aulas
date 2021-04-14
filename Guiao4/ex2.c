#include <detpic32.h>


void delay(int);

int main(void){
    TRISE = (TRISE & 0xFFF0);  //RE0 to RE3 as outputs

    LATE = (LATE & 0xFFF0); //initialize 0

    int count = 0;

    while(1){
        delay(250);
        LATE = count;  //send count value to leds
        count++;
    }
}

void delay(int ms) {
  for(; ms > 0; ms--) {
    resetCoreTimer();
    while(readCoreTimer() < 20000);
  }
}



