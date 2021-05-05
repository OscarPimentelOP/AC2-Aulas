#include <detpic32.h>

void adc(){
    if(x++ % 25 == 0){
        // Start conversion
        AD1CON1bits.ASAM = 1; // Start conversion
        while( IFS1bits.AD1IF == 0 );// Wait while conversion not done (AD1IF == 0)
        int *p = (int *)(&ADC1BUF0);
        int val_ad = 0;
        int freq_count = 0;
        for( i = 0; i < 16; i++ ){
            val_ad = val_ad + p[i*4];
            //printInt(p[i*4],0x0005000A);
        }
        val_ad = val_ad / (AD1CON2bits.SMPI +1); // Average of 4 samples
        
        freq_count = 1 + (val_ad/255);
    }
}