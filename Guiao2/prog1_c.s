 # 1 Hz para contar até 20M
 # X Hz para contar até 200000
 # X= 100 Hz

 # 1 Hz - 20M
 # 10 Hz - X M
 # X = 2000000 

 # 1 Hz - 20M
 # 5 Hz - X M
 # X = 4000000 
 
 # 1 Hz - 20M
 # 1 Hz - X M
 # X = 20000000 

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

    blt $v0, 200000, endWhile  # while(readCoreTimer() < 200000);
    #blt $v0, 2000000, endWhile  #   #10 Hz
    #blt $v0, 4000000, endWhile  #   #5 Hz
    #blt $v0, 20000000, endWhile   #   #1 Hz
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