/*
 * faça um programa que configure os portos RB8 a RB15, RD5 e RD6 como
 * saídas, que selecione apenas o "display low" (RD5=1, i.e. "CNTL_DISP_L"=1, e RD6=0)
 * e que, em ciclo infinito, execute as seguintes tarefas:
 * Ler um caracter do teclado e esperar que seja digitada uma letra entre 'a' e 'g' (ou 'A'
 * e 'G'). Use o system call getChar().
 * Escrever no porto B a combinação binária que ative apenas o segmento do display
 * correspondente ao caracter lido; note que a Figura 3 contém a informação de qual o
 * porto que corresponde a cada segmento: por exemplo, o segmento A está ligado ao
 * porto RB8.
 */

#include <detpic32.h>

void main(void){
    TRISB = (TRISB & 0x00FF); // portos RB8 a RB15 como saída
    TRISD = (TRISD & 0xFF9F); // portos RD5 e RD6 como saída
    LATB = (LATB & 0x0000); // limpar saída
    LATDbits.LATD5 = 0; // Display low control
    LATDbits.LATD6 = 1; // display high control

    //code of each segment = {'a','b','c','d','e','f','g','.'};
    static const unsigned int code[8] = {0x0100, 0x0200, 0x0400, 0x0800,
                                       0x1000, 0x2000, 0x4000, 0x8000};
    
    char key = 0;
    //valor de a
    static const int seg = 0x61;
    //indice do array
    int index;

    while(1){
        // enquanto char != a-g ou não é é pressed '.'
        while(key !='.' && (key < 0x61 || key > 0x67)){
            key = getChar();
        }
        // indice do array = key pressionada - valor de a = dá a posição no array certa para acender o segmento certo
        index = (int)key-seg;

        // se o index for entre 0 e 7 acende o display correspondente
        if(index >=0 && index <7){
            LATB = LATB & 0x0000;   // clear 
            LATB = LATB | code[index];  // acende o display correto
        }else if(key=='.'){
            LATB = LATB & 0x0000;   //clear
            LATB = LATB | code[7];  // acende o ponto
        }
        key = 0;
    }
}