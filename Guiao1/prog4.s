.equ printStr, 8
.equ printInt, 6
.equ printInt10, 7
.equ readInt10, 5

.data
msg1:   .asciiz "\nIntroduza um numero (sinal e módulo): "
msg2:   .asciiz "\nValor lido em base 2: "
msg3:   .asciiz "\nValor lido em base 16: "
msg4:   .asciiz "\nValor lido em base 10 (unsigned): "
msg5:   .asciiz "\nValor lido em base 10 (signed): "

        .text
        .globl main

main:                       #void main(void) {
                     
while:                      #while (1) {
        li $v0, printStr    #printStr("\nIntroduza um numero (sinal e módulo): ");
        la $a0, msg1
        syscall

        li $v0, readInt10   #value = readInt10();
        syscall
        or $t0, $v0, $0

        li $v0, printStr    #printStr("\nValor lido em base 2: ");
        la $a0, msg2
        syscall

        li $v0, printInt    #printInt(value, 2);
        or $a0, $t0, $0
        li $a1, 2
        syscall

        li $v0, printStr    
        la $a0, msg3
        syscall

        li $v0, printInt    #16
        or $a0, $t0, $0
        li $a1, 16
        syscall

        li $v0, printStr    
        la $a0, msg4
        syscall

        li $v0, printInt   #10 unsigned 
        or $a0, $t0, $0
        li $a1, 10
        syscall

        li $v0, printStr    
        la $a0, msg5
        syscall

        li $v0, printInt10  #10 signed
        or $a0, $t0, $0
        syscall

        j while
endW:                       #   }
        jr $ra              # }