	.text		
       	.globl __start 
__start:			# execution starts here
		sll $zero, $zero, 0
		addi $ra, $ra, 0
		j go1
		addi $t1, $zero, 1
		addi $t2, $zero, 2
		go1: addi $s1, $zero, 1
		jal go2
		addi $t3, $zero, 3
		addi $t4, $zero, 4
		addi $ra, $ra, 20  
		go2: addi $s2, $zero, 2
		jr $ra