#include <detpic32.h>


int main(void){
    TRISB = (TRISB & 0x00FF); //B8-B15 as outputs
    LATB = (LATB & 0x00FF); //clean  //00?

    //RD5-6 as outputs
    TRISDbits.TRISD5 = 0;
    TRISDbits.TRISD5 = 0;

    //select display
    LATDbits.LATD5 = 1; //low
    LATDbits.LATD6 = 1; //high

    char letter;

    while(1){
        letter = getChar();
        LATB = (LATB & 0x00FF); //clean

        switch(letter){
            case 'a':
				//LATB = (LATB | 0x0100);
                LATBbits.LATB8 = 1;
				break;
			case 'A':
				//LATB = (LATB | 0x0100);
                LATBbits.LATB8 = 1;
				break;

            case 'b':
				LATBbits.LATB9 = 1;
				break;
			case 'B':
				LATBbits.LATB9 = 1;
				break;  

            case 'c':
				LATBbits.LATB10 = 1;
				break;
			case 'C':
				LATBbits.LATB10 = 1;
				break;    

            case 'd':
				LATBbits.LATB11 = 1;
				break;
			case 'D':
				LATBbits.LATB11 = 1;
				break; 

            case 'e':
				LATBbits.LATB12 = 1;
				break;
			case 'E':
				LATBbits.LATB12 = 1;
				break; 

            case 'f':
				LATBbits.LATB13 = 1;
				break;
			case 'F':
				LATBbits.LATB13 = 1;
				break;  

            case 'g':
				LATBbits.LATB14 = 1;
				break;
			case 'G':
				LATBbits.LATB14 = 1;
				break;    

            case '.':
				LATBbits.LATB15 = 1;
				break;                      

        }
    }

}

