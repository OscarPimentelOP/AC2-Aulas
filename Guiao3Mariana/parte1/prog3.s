# Configure agora os portos RB0 a RB3 como entradas e os portos RE0 a RE3 como saídas
# e, em ciclo infinito, faça as seguintes atribuições nas saídas:
# RE0 = RB0\, RE1 = RB1, RE2 = RB2 e RE3 = RB3\
# NOT NORMAL NORMAL NOT -> 0x9 (1001)

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
        andi $t2, $t2, 0xFFF0                               # MODIFY (bit0=bit1=bit2=bit3=0 (O MEANS OUTPUT))
        sw $t2, TRISE($t1)                                  # WRITE (Write TRISE register)

                                                            # pin B0 como entrada
        lw $t3, TRISB($t1)                                  # READ (Mem_addr = 0xBF880000 + 0x6040)
        andi $t3, $t3, 0x000F                               # MODIFY (bit0=bit1=bit2=bit3=1 (1 MEANS INPUT))
        sw $t3, TRISB($t1)                                  # WRITE (Write TRISB register)
 
while: 
        lw $t4, PORTB($t1)                                  # READ
        xori $t5,$t4,0x9                                    # MODIFY + negação entrada
        sw $t5, LATE($t1)                                   # WRITE
        j while                                             # 

        jr $ra                                              # TERMINA O PROGRAMA