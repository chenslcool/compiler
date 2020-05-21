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

add1:
	subu $sp, $sp, 4
	sw $fp, 0($sp)
	move $fp, $sp
	subu $sp, $sp, 32
	sw $a0, -4($fp)
	li $t0, 0
	li $t1, 4
	mul $t2, $t0, $t1
	sw $t2, -8($fp)
	lw $t0, -4($fp)
	lw $t1, -8($fp)
	add $t2, $t0, $t1
	sw $t2, -12($fp)
	lw $t0, -12($fp)
	lw $t1, 0($t0)
	sw $t1, -16($fp)
	li $t0, 1
	li $t1, 4
	mul $t2, $t0, $t1
	sw $t2, -20($fp)
	lw $t0, -4($fp)
	lw $t1, -20($fp)
	add $t2, $t0, $t1
	sw $t2, -24($fp)
	lw $t0, -24($fp)
	lw $t1, 0($t0)
	sw $t1, -28($fp)
	lw $t0, -16($fp)
	lw $t1, -28($fp)
	add $t2, $t0, $t1
	sw $t2, -32($fp)
	lw $t0, -32($fp)
	move $v0, $t0
	addu $sp, $sp, 32
	lw $fp, 0($sp)
	addu $sp, $sp, 4
	jr $ra

main:
	subu $sp, $sp, 4
	sw $fp, 0($sp)
	move $fp, $sp
	subu $sp, $sp, 100
	li $t0, 0
	sw $t0, -20($fp)
	li $t0, 0
	sw $t0, -24($fp)
L0:
	lw $t0, -20($fp)
	li $t1, 2
	blt $t0, $t1, L1
	j L2
L1:
L3:
	lw $t0, -24($fp)
	li $t1, 2
	blt $t0, $t1, L4
	j L5
L4:
	subu $t0, $fp, 8
	sw $t0, -28($fp)
	lw $t0, -24($fp)
	li $t1, 4
	mul $t2, $t0, $t1
	sw $t2, -32($fp)
	lw $t0, -28($fp)
	lw $t1, -32($fp)
	add $t2, $t0, $t1
	sw $t2, -36($fp)
	lw $t0, -20($fp)
	lw $t1, -24($fp)
	add $t2, $t0, $t1
	sw $t2, -40($fp)
	lw $t0, -36($fp)
	lw $t1, -40($fp)
	sw $t1, 0($t0)
	lw $t0, -24($fp)
	li $t1, 1
	add $t2, $t0, $t1
	sw $t2, -44($fp)
	lw $t0, -44($fp)
	sw $t0, -24($fp)
	j L3
L5:
	subu $t0, $fp, 16
	sw $t0, -48($fp)
	li $t0, 0
	li $t1, 8
	mul $t2, $t0, $t1
	sw $t2, -52($fp)
	lw $t0, -48($fp)
	lw $t1, -52($fp)
	add $t2, $t0, $t1
	sw $t2, -56($fp)
	lw $t0, -20($fp)
	li $t1, 4
	mul $t2, $t0, $t1
	sw $t2, -60($fp)
	lw $t0, -56($fp)
	lw $t1, -60($fp)
	add $t2, $t0, $t1
	sw $t2, -64($fp)
	subu $t0, $fp, 8
	sw $t0, -68($fp)
	lw $t0, -68($fp)
	move $a0, $t0
	subu $sp, $sp, 4
	sw $ra, 0($sp)
	jal add1
	sw $v0, -72($fp)
	lw $ra, 0($sp)
	addu $sp, $sp, 4
	lw $t0, -64($fp)
	lw $t1, -72($fp)
	sw $t1, 0($t0)
	subu $t0, $fp, 16
	sw $t0, -76($fp)
	li $t0, 0
	li $t1, 8
	mul $t2, $t0, $t1
	sw $t2, -80($fp)
	lw $t0, -76($fp)
	lw $t1, -80($fp)
	add $t2, $t0, $t1
	sw $t2, -84($fp)
	lw $t0, -20($fp)
	li $t1, 4
	mul $t2, $t0, $t1
	sw $t2, -88($fp)
	lw $t0, -84($fp)
	lw $t1, -88($fp)
	add $t2, $t0, $t1
	sw $t2, -92($fp)
	lw $t0, -92($fp)
	lw $t1, 0($t0)
	sw $t1, -96($fp)
	lw $t0, -96($fp)
	move $a0, $t0
	subu $sp, $sp, 4
	sw $ra, 0($sp)
	jal write
	lw $ra, 0($sp)
	addu $sp, $sp, 4
	lw $t0, -20($fp)
	li $t1, 1
	add $t2, $t0, $t1
	sw $t2, -100($fp)
	lw $t0, -100($fp)
	sw $t0, -20($fp)
	li $t0, 0
	sw $t0, -24($fp)
	j L0
L2:
	li $t0, 0
	move $v0, $t0
	addu $sp, $sp, 100
	lw $fp, 0($sp)
	addu $sp, $sp, 4
	jr $ra
