.equ getChar, 2
.equ putChar, 3
.data 
.text
.globl main

main:                       # int main(void) {
                            #   char c;
do:                         #   while (1) {
        li $v0, getChar     #     c = getChar();
        syscall

doWhile:     
        beq $v0,'\n', endWhile  #     if (c == '\n') break;
        or $a0, $0, $v0     #     putChar(c);
        li $v0, putChar
        syscall
        j doWhile             #   }

endWhile:   
        ori $v0, $0, 0       #   return 0;
        jr $ra              # }