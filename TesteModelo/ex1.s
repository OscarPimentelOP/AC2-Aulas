.data

.equ SFR_BASE_HI, 0xBF88
.equ TRISE, 0x6100
.equ PORTE, 0x6110
.equ LATE, 0x6120

.equ TRISB, 0x6040
.equ PORTB, 0x6050
.equ LATB, 0x6040

.equ getChar, 2
.equ printStr, 8
.equ readCoreTimer, 11
.equ resetCoreTimer, 12

.text

.globl main

main:
    #configure RE0-4 as outputs (all 0)
    lui $t1, SFR_BASE_HI
    lw $t2, LATE($t1)  
    andi $t2, $t2, 0xFFF0
    sw $t2, LATE($1)
    lw $t2, TRISE($t1)
    andi $t2, $t2, 0xFFF0
    sw $t2, TRISE($t1)

whileInf:
    li $v0, getChar
    syscall

ifKey0:
    bne $v0, '0', ifKey1  #if (key == '0')        
    lw $t2, LATE($t1)
    andi $t2, $t2, 0xFFFE #configure RE0 as output (E = 1110)
    ori  $t2, $t2, 0x0001 #LATE0 for output (0001)
    sw $t2, LATE($t1)
    j whileInf

ifKey1:
    bne $v0, '1', ifKey2 #if (key == '1') 
    lw $t2, LATE($t1)
    andi $t2, $t2, 0xFFFD #configure RE1 as output (D = 1101)
    ori $t2, $t2, 0x0002  #LATE1 for output (0010)
    sw $t2, LATE($t1)
    j whileInf  

ifKey2:
    bne $v0, '2', ifKey3 #if (key == '2') 
    lw $t2, LATE($t1)
    andi $t2, $t2, 0xFFFB  #configure RE2 as output (B = 1011) 
    ori $t2, $t2, 0x0004   #LATE2 for output (0100)
    sw $t2, LATE($t1)
    j whileInf

ifKey3:
    bne $v0, '3', ifAnotherKey  #if (key == '3') 
    lw $t2, LATE($t1)
    andi $t2, $t2, 0xFFF7   #configure RE3 as output (7 = 0111)  
    ori $t2, $t2, 0x0008    #LATE3 for output (1000)
    sw $t2, LATE($t1)
    j whileInf

ifAnotherKey:           #if (key == anythingElse) 
    lw $t2, LATE($t1)   
    andi $t2, $t2, 0xFFE0  #configure RE4 as output (E = 1110)  
    ori $t2, $t2, 0x0010   #LATE4 for output (0001 0000) 
    sw $t2, LATE($t1)
    
    li $a0, 2000  #delay de 2 segundos = 2000 ms => delay(2000);

forDelay:  
    ble $a0, 0, endForDelay # for(; ms > 0; ms--){
    add $a0, $a0, -1    #ms--
    li $v0, resetCoreTimer
    syscall         # system call resetCoreTimer();

whileDelay:
    li $v0, readCoreTimer # system call readCoreTimer();
    syscall
    bge $v0, 20000, endWhileDelay  #while(readCoreTimer() < 20000);
    j whileDelay

endWhileDelay:
    j forDelay

endForDelay:
    lw $t2, LATE($t1)
    andi $t2, $t2, 0xFFE0    #turn off all the 5 LEDS (1110 0000)
    sw $t2, LATE($t1)

endProg:
    j whileInf
    jr $ra    
