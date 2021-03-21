# int main(void){
#       char c;
#       do{
#               c = getChar();
#               if( c != '\n' )
#               putChar( c );
#       } while( c != '\n' );
#       return 0;
# }

        .equ GET_CHAR, 2
        .equ PUT_CHAR, 3

        .data
        .text
        .globl main

main:   
        li $v0, GET_CHAR
        syscall                             # getChar();
        move $t1,$v0                        # c = getChar();

        move $a0,$t1
        li $v0, PUT_CHAR
        syscall
        bne $t1, '\n', main                # while ( c != '\n')

endw:   li $v0,0x00                         # return 0
        jr $ra                              # fim do programa