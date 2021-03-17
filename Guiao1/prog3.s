.equ inkey, 1
.equ putChar, 3

.data
.text
.globl main

main:

doWhile:                      #while {
    li $v0, inkey			  #c = inkey();
    syscall

    bne $v0, 0, if			  #while ((c = inkey()) == 0);
    j doWhile

if:			    
    beq $v0,'\n', endWhile   #if (c == '\n') break;

    or $a0, $0, $v0          #putChar(c);
    li $v0, putChar
    syscall

    j doWhile					 #}

endWhile:		ori $v0, $0, 0   #return 0
				jr $ra