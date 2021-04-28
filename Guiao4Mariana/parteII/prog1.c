
/**
 * O programa desenvolvido nos pontos anteriores permite enviar 4 bits – um caracter
 * hexadecimal – para um dos displays. Escreva agora uma função que envie um byte (8 bits)
 * ou seja dois algarismos hexadecimais para os dois displays, fazendo corresponder os 4 bits
 * menos significativos ao display low e os 4 bits mais significativos ao display high.
 */

void send2displays(unsigned char value){
    static const char display7Scodes[] = {0x3F, 0x06,0x5b,...};
    // send digit_high (dh) to display_high: dh = value >> 4
    // send digit_low (dl) to display_low: dl = value & 0x0F
}