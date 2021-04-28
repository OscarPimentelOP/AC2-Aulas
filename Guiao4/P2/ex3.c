#include <detpic32.h>

void send2displays(unsigned char);
void delay(int);

void main(void){

    unsigned count = 0; //count variable

    //RB8-14 configured as outputs (0)
    TRISB = (TRISB & 0x00FF);
    LATB = (LATB & 0x00FF);

    //RD5-6 configured as outputs (0)
    TRISDbits.TRISD5 = 0;
    TRISDbits.TRISD6 = 0;

    //display high control
	LATDbits.LATD5 = 0; //low
	LATDbits.LATD6 = 0; //high

    while(1){
        int i = 0;
        //do 4 bits, then move to next
        do{
            delay(15); //wait 50, disolay atualizado de 100 em 100 ms
            //delay(500); //50Hz
            //delay(1000); //100Hz
            send2displays(count); //send counter value to displays
            i++;
        } while(i < 4);

        count++; //increase counter
        count = count % 256; //max 9 bits
        delay(10); //200 was the value but I want this faster kek
    }


}

void send2displays(unsigned char value) {

  static const char display7Scodes[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x67, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};
  static char displayFlag   = 0;

  // Configure RB8 to RB15 as outputs
  TRISB = (TRISB & 0x00FF);

  // Configure RD5 and RD6 as outputs
  TRISDbits.TRISD5 = 0;
  TRISDbits.TRISD6 = 0;

  // Determine digits
  int digit_low = value & 0x0F;     
  int digit_high = value >> 4;      

  // Send digit_low to display low
  if (displayFlag == 0) {
    LATB = (LATB & 0x00FF) | (display7Scodes[digit_low] << 8);
    LATDbits.LATD5 = 1;
    LATDbits.LATD6 = 0;
  }

  // Send digit_high to display high
  else {
    LATB = (LATB & 0x00FF) | (display7Scodes[digit_high] << 8);
    LATDbits.LATD5 = 0;
    LATDbits.LATD6 = 1;
  }

  // Toggle displayFlag
  displayFlag = !displayFlag;
}

void delay(int ms){
    int i;
    for(i=ms; i>=0; i--){
        resetCoreTimer();
        while(readCoreTimer() < 20000);
    }
}