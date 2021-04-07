/* Exercicio 3
* Usando como base a função delay() , escreva um programa em linguagem C que
* incremente, em ciclo infinito, 3 variáveis inteiras: a variável cnt1 deve ser incrementada a
* uma frequência de 1Hz, a variável cnt5 deve ser incrementada a uma frequência de 5Hz,
* e a variável cnt10 deve ser incrementada a uma frequência de 10Hz.
* O valor das 3 variáveis deve ser mostrado no ecrã, sempre na mesma linha, formatado em
* base 10 com 5 dígitos. Exemplo de visualização:
*       00200       00100       00020
*
*/

int cnt1 = 0;
int cnt5 = 0;
int cnt10 = 0;

void delay(int ms){
    for(; ms > 0; ms--){
        resetCoreTimer();
        while(readCoreTimer() < 20000);
    }
}

void main(){
    while(1){
        printInt(cnt1++);
        delay(200000000); // 1 HZ --> 20M/1
        cnt5++;
        delay(400000); // 5 Hz --> 20M/5
        cnt10++;
        delay(20000); // 10 Hz --> 20M/10
        putChar('\r');
    }
}