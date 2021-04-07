            .equ READ_CORE_TIMER,11
            .equ RESET_CORE_TIMER,12
            .equ PUT_CHAR,10
            .equ PRINT_INT,6
            .equ k, 20000
            .data
            .text
            .globl main

main:       addiu $sp,$sp,-8
            sw $ra,0($sp)
            sw $s0,4($sp)

            li $s0,0

 while:     addi $s0,$s0,1
            move $a0,$s0
            li $v0,PRINT_INT
            syscall

            li $a0,20000000
            jal delay

            j while

endWhile1:  lw $ra,0($sp)
            lw $s0,4($sp)
            addi $sp,$sp,8
            jr $ra         

################################################################

delay:      li $t0,0                        # int counter = 0
for:        li $v0,READ_CORE_TIMER          # while (1) {
            syscall                         #   
            
            ble $t0,0,endFor                # for(; ms > 0; ms--){}

            li $v0, RESET_CORE_TIMER
            syscall                         # resetCoreTimer();

while1:     li $v0,READ_CORE_TIMER          
            blt $v0, k, while1              # while(readCoreTimer() < K);

            subu $t0,$t0,1                  # ms--
            j for                           # }

endFor:     jr $ra                          # }