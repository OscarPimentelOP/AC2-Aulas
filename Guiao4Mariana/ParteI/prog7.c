/**
 * Construa a tabela que relaciona as combinações binárias de 4 bits (dígitos 0 a F) com o
 * respetivo código de 7 segmentos, de acordo com o circuito montado no ponto anterior e
 * com a definição gráfica dos dígitos apresentada na Figura 4.
 */

#include <detpic32.h>

void delay(int ms){
   for(; ms > 0; ms--){
        resetCoreTimer();
        while(readCoreTimer() < 20000);
    }
}

int main(void){

   static const char display7Scodes[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07,
                              0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};

   TRISB = (TRISB & 0x00FF); // portos RB8 a RB15 como saída
   TRISD = (TRISD & 0xFF9F); // portos RD5 e RD6 como saída
   LATB = (LATB & 0x0000); // limpar saída
   LATDbits.LATD5 = 0; // Display low control
   LATDbits.LATD6 = 1; // display high control

   while(1){
      LATDbits.LATD5 = !LATDbits.LATD5;
      LATDbits.LATD6 = !LATDbits.LATD6;               //toggle display selection
      
      int i = 0;
      for (i = 0; i<16; i++){
         LATB = (LATB & 0x0000) | display7Scodes[i] <<8; // shift de 8 para avançar uma posição do array
         delay(1000);
      }

   }
}
   
