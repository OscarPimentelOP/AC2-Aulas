 #Mapeamento de variáveis:
 #ms -> $t0

.equ printInt, 6
.equ READ_CORE_TIMER, 11
.equ RESET_CORE_TIMER, 12
.equ K, 20000

.data
.text

.globl main

main:
    addiu $sp,$sp,-8 		
	sw $ra,0($sp)
    sw $s0, 4($sp) 

    li $s0, 0   # cnt1 = 0;

while:
    addi $s0, $s0, 1
    move $a0, $s0 
    li $a1, 10
    li $v0, printInt 
    syscall

    li $a0, 20000000
    jal delay

    j while



endWhile:
    lw	$ra,0($sp)
    lw  $s0, 4($sp)
	addi	$sp,$sp,8
	jr	$ra

#--------------------------------------------------
delay:
    move $v0, $a0

for:
    ble $t0, 0, endFor # for(; ms > 0; ms--)

    li $v0, RESET_CORE_TIMER # resetCoreTimer();
    syscall

while2:  # while(readCoreTimer() < K);
    li $v0, READ_CORE_TIMER # readCoreTimer()
    blt $v0, K, while2

    subu $t0, $t0, 1 # ms--
    j for 

endFor: 
    jr $ra 