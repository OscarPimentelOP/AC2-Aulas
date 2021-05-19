#
# Mariana Pinto, nmec84792
#
# int main(void){ 
#    char c;
#    do{
#        while( (c = inkey()) == 0 );
#        if( c != '\n' )
#            putChar( c );
#    } while( c != '\n' );
#    return 0;
# }

        .equ INKEY, 1
        .equ PUT_CHAR, 3

        .data
        .text
        .globl main

main:   
        li $v0,INKEY
        syscall                         # inkey();
        move $t1,$v0                    # c = inkey();
while:
        beq $t1,0,main                  # while((c=inkey())==0);
        beq $t1, '\n', endw             # if( c != '\n' )

        move $a0,$t1                    # c como valor de entrada
        li $v0, PUT_CHAR                
        syscall                         # putchar(c);
        bne $t1, '\n', main             # while(c != '\n');

endw:   li $v0, 0x00                    # return 0;
        jr $ra                          # fim do programa
    