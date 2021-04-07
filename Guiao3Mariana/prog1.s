# Escreva e teste um programa em assembly que:
#  - configure o porto RE0 como saída e o porto RB0 como entrada;
#  - em ciclo infinito, leia o valor do porto de entrada e escreva 
#    esse valor no porto de saída (i.e., RE0 = RB0).

        .equ SFR_BASE_HI, 0xBF88
                                                            # I/O pin E
        .equ TRISE, 0x6100
        .equ PORTE, 0x6110
        .equ LATE, 0x6120
                                                            # I/O pin B
        .equ TRISB, 0x6040
        .equ PORTB, 0X6050
        .equ LATB, 0x6060
        .data
        .text
        .globl main

main:   lui $t1, SFR_BASE_HI
                                                            # pin E0 como saída
        lw $t2,TRISE($t1)                                   # READ (Mem_addr = 0xBF880000 + 0x6100)
        andi $t2, $t2, 0xFFFE                               # MODIFY (bit0=0 (O MEANS OUTPUT))
        sw $t2, TRISE($t1)                                  # WRITE (Write TRISE register)
                                                            # pin B0 como entrada
        lw $t3, TRISB($t1)                                  # READ (Mem_addr = 0xBF880000 + 0x6040)
        andi $t3, $t3, 0x0001                               # MODIFY (bit0=1 (1 MEANS INPUT))
        sw $t3, TRISB($t1)                                  # WRITE (Write TRISB register)

while:  lw $t4, PORTB($t1)                                  # verificar
        sw $t4, LATE($t1)                                   # verificar
        j while

        jr $ra