 #Mapeamento de variáveis:
 #ms -> $t0

.equ READ_CORE_TIMER, 11
.equ RESET_CORE_TIMER, 12
.equ K, 20000

.data
.text

.globl main

main:
    move $v0, $a0

for:
    ble $t0, 0, endFor # for(; ms > 0; ms--)

    li $v0, RESET_CORE_TIMER # resetCoreTimer();
    syscall

while:  # while(readCoreTimer() < K);
    li $v0, READ_CORE_TIMER # readCoreTimer()
    blt $v0, K, while

    subu $t0, $t0, 1 # ms--
    j for 

endFor: 
    jr $ra    



