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

f:
	subu $sp, $sp, 4
	sw $fp, 0($sp)
	move $fp, $sp
	subu $sp, $sp, 28
	sw $a0, -24($fp)
	sw $a1, -20($fp)
	sw $a2, -16($fp)
	sw $a3, -12($fp)
	lw $t0, 12($fp)
	sw $t0, -8($fp)
	lw $t0, 8($fp)
	sw $t0, -4($fp)
	lw $t0, -8($fp)
	lw $t1, -24($fp)
	add $t2, $t0, $t1
	sw $t2, -28($fp)
	lw $t0, -28($fp)
	move $v0, $t0
	addu $sp, $sp, 28
	lw $fp, 0($sp)
	addu $sp, $sp, 4
	jr $ra
main:
	subu $sp, $sp, 4
	sw $fp, 0($sp)
	move $fp, $sp
	subu $sp, $sp, 20
	subu $sp, $sp, 4
	sw $ra, 0($sp)
	jal read
	sw $v0, -12($fp)
	lw $ra, 0($sp)
	addu $sp, $sp, 4
	lw $t0, -12($fp)
	sw $t0, -4($fp)
	subu $sp, $sp, 4
	sw $ra, 0($sp)
	jal read
	sw $v0, -16($fp)
	lw $ra, 0($sp)
	addu $sp, $sp, 4
	lw $t0, -16($fp)
	sw $t0, -8($fp)
	li $t0, 6
	move $a0, $t0
	li $t0, 5
	move $a1, $t0
	li $t0, 4
	move $a2, $t0
	li $t0, 3
	move $a3, $t0
	li $t0, 2
	subu $sp, $sp, 4
	sw $t0, 0($sp)
	li $t0, 1
	subu $sp, $sp, 4
	sw $t0, 0($sp)
	subu $sp, $sp, 4
	sw $ra, 0($sp)
	jal f
	sw $v0, -20($fp)
	lw $ra, 0($sp)
	addu $sp, $sp, 4
	addu $sp, $sp, 8
	lw $t0, -20($fp)
	sw $t0, -4($fp)
	lw $t0, -4($fp)
	move $a0, $t0
	subu $sp, $sp, 4
	sw $ra, 0($sp)
	jal write
	lw $ra, 0($sp)
	addu $sp, $sp, 4
	li $t0, 0
	move $v0, $t0
	addu $sp, $sp, 20
	lw $fp, 0($sp)
	addu $sp, $sp, 4
	jr $ra
