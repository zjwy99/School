	.text		
       	.globl __start 
__start:			# execution starts here
		sll $zero, $zero, 0
		lui $s2, 0x1000
		sw $zero, 0($s2)
		lw $t1, 0($s2)