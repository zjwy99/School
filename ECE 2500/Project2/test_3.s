	.text		
       	.globl __start 
__start:			# execution starts here
		sll $zero, $zero, 0
		lui $s2, 0x1000
		beq $zero, $zero, equal
		addi $t0, $zero, 4
		equal: addi $t0, $zero, 5
		bne $zero, $s2, notequal
		addi $t0, $zero, 0
		notequal: addi $t1, $zero, 1