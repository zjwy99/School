	.text		
       	.globl __start 
__start:			# execution starts heresll $zero, $zero, 0
		lui $s2, 0x1000
		addi $s0, $zero, 5			
		addi $s1, $zero, 0	
		add $t0, $s1, $s2
		sub $t1, $s0, $s2
		slt $t2, $s1, $s2
		andi $t3, $s0, 0x1
		nor $t4, $zero, $zero