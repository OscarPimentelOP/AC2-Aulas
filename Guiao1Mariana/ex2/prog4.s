# int main(void){
#   int value;
#   while(1){
#       printStr("\nIntroduza um numero (sinal e módulo): ");
#       value = readInt10();
#       printStr("\nValor lido, em base 2: ");
#       printInt(value, 2);
#       printStr("\nValor lido, em base 16: ");
#       printInt(value, 16);
#       printStr("\nValor lido, em base 10 (unsigned): ");
#       printInt(value, 10);
#       printStr("\nValor lido, em base 10 (signed): ");
#       printInt10(value);
#   }
#   return 0;
# }

        .equ PRINT_STRING, 8
        .equ READ_INT10, 5
        .equ PRINT_INT, 6
        .equ PRINT_INT10, 7

        .data
msg1:   .asciiz "\nIntroduza um numero (sinal e módulo): "
msg2:   .asciiz "\nValor lido, em base 2: "
msg3:   .asciiz "\nValor lido, em base 16: "    
msg4:   .asciiz "\nValor lido, em base 10 (unsigned): "
msg5:   .asciiz "\nValor lido, em base 10 (signed): "
        .text
        .globl main

main:   
        la $a0,msg1
        li $v0, PRINT_STRING
        syscall                                                         # printStr("\nIntroduza um numero (sinal e módulo): ");

        li $v0,READ_INT10
        syscall                                                         # readInt10();
        move $t1,$v0                                                    # guardar o valor

        la $a0,msg2                                                     # value = readInt10();
        li $v0,PRINT_STRING
        syscall                                                         # printStr("\nValor lido, em base 2: ");

        move $a0,$t1                                                    # valor entrada 
        li $a1,2                                                        # base de entrada       
        li $v0,PRINT_INT                                                
        syscall                                                         # printInt(value,2);   

        la $a0, msg3                                                    # mensagem entrada
        li $v0, PRINT_STRING
        syscall                                                         # printStr("\nValor lido, em base 16: ");

        move $a0,$t1                                                    # valor entrada
        li $a1,16                                                       # base entrada
        li $v0,PRINT_INT
        syscall                                                         # printInt(value,16);   

        la $a0, msg4
        li $v0,PRINT_STRING
        syscall                                                         # printStr("\nValor lido, em base 10 (unsigned): ");

        move $a0,$t1                                                    # valor entrada
        li $a1,10                                                       # base entrada
        li $v0,PRINT_INT
        syscall                                                         # printInt(value,10); 

        la $a0,msg5
        li $v0, PRINT_STRING
        syscall                                                         # printStr("\nValor lido, em base 10 (signed): ");

        move $a0,$t1                                                    # valor entrada
        li $v0,PRINT_INT10
        syscall                                                         # printInt10(value);

        jr $ra                                                          # fim do programa 

