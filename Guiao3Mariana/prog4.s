# Traduza para assembly o trecho de código seguinte 1 , em que delay() é a função que
# implementou na aula anterior. Compile, transfira para a placa DETPIC32 e execute esse código.
# void main(void){
#   int v = 0;
#   TRISE0 = 0;                     // Configura o porto RE0 como saída
#   while(1){               
#       LATE0 = v;                  // Escreve v no bit 0 do porto E
#       delay(500);                 // Atraso de 500ms
#       v ^= 1;                     // complementa o bit 0 de v (v = v xor 1)
#   }
# }

        .equ SFR_BASE_HI, 0xBF88
        .equ READ_CORE_TIMER, 11
        .equ RESET_CORE_TIMER, 12
                                                            # I/O pin E
        .equ TRISE, 0x6100
        .equ PORTE, 0x6110
        .equ LATE, 0x6120
        .data   
        .text
        .globl main

main:   li $t0,0x00                                         # v = 0
        lui $t1, SFR_BASE_HI
                                                            # pin E0 como saída
        lw $t2,TRISE($t1)                                   # READ (Mem_addr = 0xBF880000 + 0x6100)
        andi $t2, $t2, 0xFFFE                               # MODIFY (bit0=0 (O MEANS OUTPUT))
        sw $t2, TRISE($t1)                                  # WRITE (Write TRISE register)

while:  sw $t0, LATE($t1)
        li $a0,500                                          # f = 1/T => f=1/0.5 => F=2hz
        jal delay
        xor $t0, $t0, 1
        j while

        jr $ra


# ###########################################################
delay:
        subu $sp, $sp, 4
        sw $ra, 0($sp)
        move $s0, $a0
for:
        ble $s0, 0x00, endf
        li $v0, RESET_CORE_TIMER
        syscall
read:
        li $v0, READ_CORE_TIMER
        syscall
        blt $v0, 20000, read
        sub $s0, $s0, 1
        j for
endf:
        lw $ra, 0($sp)
        addiu $sp, $sp, 4
        jr $ra