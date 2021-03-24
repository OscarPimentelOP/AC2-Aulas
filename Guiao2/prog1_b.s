 # 1 Hz para contar até 20M
 # X Hz para contar até 200000
 # X= 100 Hz
 
 #Mapeamento de variáveis:
 #counter -> $t0

 .equ READ_CORE_TIMER,11
 .equ RESET_CORE_TIMER, 12
 .equ PUT_CHAR, 3
 .equ PRINT_INT, 6
 .data
 .text
 .globl main

main: li $t0,0

while: 
    li $v0,READ_CORE_TIMER # while (1) {
    syscall #
while2:    
    blt $v0, 200000, endWhile2  # while(readCoreTimer() < 200000);
    j while2
endWhile2:
    li $v0, RESET_CORE_TIMER  # resetCoreTimer();
    syscall

    addi $t0, $t0, 1  #printInt(++counter, 10);
    move $a0, $t0
    li $a1, 10
    li $v0, PRINT_INT 
    syscall 

    li $a0, '\r' 
    li $v0, PUT_CHAR # putChar('\r');
    syscall

    j while

endWhile: 
    li $v0,0 # return 0;

    jr $ra # 