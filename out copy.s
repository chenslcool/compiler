.data
_prompt: .asciiz "Enter an integer:"
_ret: .asciiz "\n"
.globl main
.text

read:
	li $v0, 4
	la $a0, _prompt
	syscall
	li $v0, 5
	syscall
	jr $ra

write:
	li $v0, 1
	syscall
	li $v0, 4
	la $a0, _ret
	syscall
	move $v0, $0
	jr $ra

main:
	subu $sp, $sp, 4
	sw $fp, 0($sp)
	move $fp, $sp
	subu $sp, $sp, 60
	li $t0, 3
	sw $t0, -4($fp)
	li $t0, 0
	sw $t0, -20($fp)
L0:
	lw $t0, -20($fp)
	lw $t1, -4($fp)
	blt $t0, $t1, L1
	j L2
L1:
	subu $t0, $fp, 8 
	sw $t0, -24($fp) //t0 = &a
	lw $t0, -20($fp) //t0 = i
	li $t1, 4
	mul $t2, $t0, $t1 // t1 = i * 4
	sw $t2, -28($fp)
	lw $t0, -24($fp) //t0
	lw $t1, -28($fp)    //t1
	add $t2, $t0, $t1
	sw $t2, -32($fp)
	li $t0, 2 //#2
	lw $t1, -20($fp) //i
	mul $t2, $t0, $t1 // 2 * i
	sw $t2, -36($fp)//t3 = 2 * i
	lw $t0, -32($fp) //t2
	lw $t1, -36($fp) //t3
	sw $t1, 0($t0) //*t2 = t3
	lw $t0, -20($fp)
	li $t1, 1
	add $t2, $t0, $t1
	sw $t2, -40($fp)
	lw $t0, -40($fp)
	sw $t0, -20($fp)
	j L0
L2:
	li $t0, 0
	sw $t0, -20($fp)
L3:
	lw $t0, -20($fp)
	lw $t1, -4($fp)
	blt $t0, $t1, L4
	j L5
L4:
	subu $t0, $fp, 8
	sw $t0, -44($fp)
	lw $t0, -20($fp)
	li $t1, 4
	mul $t2, $t0, $t1
	sw $t2, -48($fp)
	lw $t0, -44($fp)
	lw $t1, -48($fp)
	add $t2, $t0, $t1
	sw $t2, -52($fp)
	lw $t0, -52($fp)
	lw $t1, 0($t0)
	sw $t1, -56($fp)
	lw $t0, -56($fp)
	move $a0, $t0
	subu $sp, $sp, 4
	sw $ra, 0($sp)
	jal write
	lw $ra, 0($sp)
	addu $sp, $sp, 4
	lw $t0, -20($fp)
	li $t1, 1
	add $t2, $t0, $t1
	sw $t2, -60($fp)
	lw $t0, -60($fp)
	sw $t0, -20($fp)
	j L3
L5:
	li $t0, 0
	move $v0, $t0
	addu $sp, $sp, 60
	lw $fp, 0($sp)
	addu $sp, $sp, 4
	jr $ra
