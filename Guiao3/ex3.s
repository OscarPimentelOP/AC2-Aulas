 .equ SFR_BASE_HI, 0xBF88 # 16 MSbits of SFR area
 .equ TRISE, 0x6100 # TRISE address is 0xBF886100
 .equ PORTE, 0x6110 # PORTE address is 0xBF886110
 .equ LATE, 0x6120 # LATE address is 0xBF886120

  
  .equ TRISB, 0x6040 # TRISE address is 0xBF886100
  .equ PORTB, 0x6050 # PORTE address is 0xBF886110
  .equ LATB, 0x6040 # LATE address is 0xBF886120

.data
.text
.globl main

main:
#set E0 as output
    lui $t1, SFR_BASE_HI #
    lw $t2, TRISE($t1)  # READ (mem_Addr = 0xBF880000+0x00006100)
    andi $t2, $t2, 0xFFF0 # MODIFY (E0 = 0)
    sw $t2, TRISE($t1) # WRITE (Write TRISE register) configured

#set B0 as input
    lw $t2, TRISB($t1)  # READ (mem_Addr = 0xBF880000+0x00006040)
    ori $t2, $t2, 0x000F # MODIFY (B0 = 1s)    
    sw $t2,TRISB($t1) # WRITE (Write TRISE register) configured


while: #while 1	
    lw $t2, PORTB($t1)   #load what is on B
    
    xori $t3,$t3,0x09 #1001
    sw $t3, LATE($t1)    #store to RE

    j while

jr $ra