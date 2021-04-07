	.equ SFR_BASE_HI, 0xBF88 
	.equ TRISE, 0x6100 #TRISE address is 0xBF886100
	.equ PORTE, 0x6110 # PORTE address is 0xBF886110
	.equ LATE, 0x6120 # LATE address is 0xBF886120
	.equ READ_CORE_TIMER, 11
	.equ RESET_CORE_TIMER, 12
	.equ PUT_CHAR, 3
	.equ PRINT_INT, 6

	.data
	.text
	.globl main

# v = $s0
# 500 = $a0
# sfr_base_hi = $s1
# lateE = v =$s2


main:	subu $sp, $sp,16
	    sw $ra, 0($sp)
	    sw $s0, 4($sp) 
	    sw $s1, 8($sp)
	    sw $s2, 12($sp)

	    li $s0,0 # v= 0
	    lui $s1, SFR_BASE_HI

	    #E0 as output
	    lw $t2, TRISE($s1)  #READ (mem_Addr = 0xBF880000+0x00006100)
		andi $t2, $t2, 0xFFFE #MODIFY (E0 = 0)
		sw $t2,TRISE($s1) #WRITE TRISE configured

while:	lw $t2,LATE($s1)
		#andi $t2,$t2 0xFFFE #limpa
		or $s2,$t2,$s0 #E0 = 0
		sw $s2, LATE($s1)

		li $a0, 500
		jal delay  #delay(500)

		xori $s0, $s0, 1 # v^=1

		j while

end:	lw $ra, 0($sp)
    	lw $s0, 4($sp)
    	lw $s1, 8($sp)
    	lw $s2, 12($sp)
    	addiu $sp,$sp, 16

    	jr $ra

delay:  li $t0, 0
while1:	li $v0, READ_CORE_TIMER #while(1)
		syscall
		bge $v0, 200000, exit #while(readCoreTimer()<200000);
		li $v0, RESET_CORE_TIMER
		syscall #resetCoreTimer();
		li $a0, ' '
		li $v0, PUT_CHAR
		syscall #putChar('');
		addi $t0, $t0, 1 
		move $a0, $t0
		li $a1, 10
		li $v0, PRINT_INT
		syscall #printInt(++counter, 10)
		j while1
exit:
		jr $ra	