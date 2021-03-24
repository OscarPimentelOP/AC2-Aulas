            .equ READ_CORE_TIMER,11
            .equ RESET_CORE_TIMER,
            .equ PUT_CHAR,
            .equ PRINT_INT,
            .data
            .text
            .globl main

main:       li $t0,0                        # int counter = 0
for:        li $v0,READ_CORE_TIMER          # while (1) {
            syscall                         #   
            
            ble $t0,0,endFor                # for(; ms > 0; ms--){}

            li $v0, RESET_CORE_TIMER
            syscall                         # resetCoreTimer();

while:      li $v0,READ_CORE_TIMER          
            blt $v0, k, while               # while(readCoreTimer() < K);

            subu $t0,$t0,1                  # ms--
            j for                           # }

            jr $ra                          # }
                                     