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
	subu $sp, $sp, 256
	li $t0, 10
	sw $t0, -4($fp)
	li $t0, 0
	sw $t0, -48($fp)
	li $t0, 0
	sw $t0, -72($fp)
L0:
	lw $t0, -48($fp)
	li $t1, 10
	blt $t0, $t1, L1
	j L2
L1:
	subu $t0, $fp, 44
	sw $t0, -76($fp)
	lw $t0, -48($fp)
	li $t1, 4
	mul $t2, $t0, $t1
	sw $t2, -80($fp)
	lw $t0, -76($fp)
	lw $t1, -80($fp)
	add $t2, $t0, $t1
	sw $t2, -84($fp)
	subu $sp, $sp, 4
	sw $ra, 0($sp)
	jal read
	sw $v0, -88($fp)
	lw $ra, 0($sp)
	addu $sp, $sp, 4
	lw $t0, -84($fp)
	lw $t1, -88($fp)
	sw $t1, 0($t0)
	lw $t0, -48($fp)
	li $t1, 1
	add $t2, $t0, $t1
	sw $t2, -92($fp)
	lw $t0, -92($fp)
	sw $t0, -48($fp)
	j L0
L2:
	lw $t0, -4($fp)
	li $t1, 2
	div $t0, $t1
	mflo $t2	
	sw $t2, -96($fp)
	lw $t0, -96($fp)
	sw $t0, -52($fp)
L3:
	lw $t0, -52($fp)
	li $t1, 0
	bge $t0, $t1, L4
	j L5
L4:
	li $t0, 0
	sw $t0, -72($fp)
	lw $t0, -52($fp)
	sw $t0, -48($fp)
L6:
	lw $t0, -72($fp)
	li $t1, 0
	beq $t0, $t1, L7
	j L8
L7:
	li $t0, 1
	sw $t0, -72($fp)
	lw $t0, -48($fp)
	li $t1, 2
	mul $t2, $t0, $t1
	sw $t2, -100($fp)
	lw $t0, -100($fp)
	li $t1, 1
	add $t2, $t0, $t1
	sw $t2, -104($fp)
	lw $t0, -104($fp)
	sw $t0, -56($fp)
	lw $t0, -48($fp)
	li $t1, 2
	mul $t2, $t0, $t1
	sw $t2, -108($fp)
	lw $t0, -108($fp)
	li $t1, 2
	add $t2, $t0, $t1
	sw $t2, -112($fp)
	lw $t0, -112($fp)
	sw $t0, -64($fp)
	lw $t0, -56($fp)
	lw $t1, -4($fp)
	blt $t0, $t1, L9
	j L10
L9:
	subu $t0, $fp, 44
	sw $t0, -116($fp)
	lw $t0, -56($fp)
	li $t1, 4
	mul $t2, $t0, $t1
	sw $t2, -120($fp)
	lw $t0, -116($fp)
	lw $t1, -120($fp)
	add $t2, $t0, $t1
	sw $t2, -124($fp)
	lw $t0, -124($fp)
	lw $t1, 0($t0)
	sw $t1, -128($fp)
	lw $t0, -128($fp)
	sw $t0, -60($fp)
	lw $t0, -64($fp)
	lw $t1, -4($fp)
	blt $t0, $t1, L13
	j L12
L13:
	subu $t0, $fp, 44
	sw $t0, -132($fp)
	lw $t0, -64($fp)
	li $t1, 4
	mul $t2, $t0, $t1
	sw $t2, -136($fp)
	lw $t0, -132($fp)
	lw $t1, -136($fp)
	add $t2, $t0, $t1
	sw $t2, -140($fp)
	lw $t0, -140($fp)
	lw $t1, 0($t0)
	sw $t1, -144($fp)
	subu $t0, $fp, 44
	sw $t0, -148($fp)
	lw $t0, -56($fp)
	li $t1, 4
	mul $t2, $t0, $t1
	sw $t2, -152($fp)
	lw $t0, -148($fp)
	lw $t1, -152($fp)
	add $t2, $t0, $t1
	sw $t2, -156($fp)
	lw $t0, -156($fp)
	lw $t1, 0($t0)
	sw $t1, -160($fp)
	lw $t0, -144($fp)
	lw $t1, -160($fp)
	blt $t0, $t1, L11
	j L12
L11:
	subu $t0, $fp, 44
	sw $t0, -164($fp)
	lw $t0, -64($fp)
	li $t1, 4
	mul $t2, $t0, $t1
	sw $t2, -168($fp)
	lw $t0, -164($fp)
	lw $t1, -168($fp)
	add $t2, $t0, $t1
	sw $t2, -172($fp)
	lw $t0, -172($fp)
	lw $t1, 0($t0)
	sw $t1, -176($fp)
	lw $t0, -176($fp)
	sw $t0, -60($fp)
	lw $t0, -64($fp)
	sw $t0, -56($fp)
L12:
	subu $t0, $fp, 44
	sw $t0, -180($fp)
	lw $t0, -48($fp)
	li $t1, 4
	mul $t2, $t0, $t1
	sw $t2, -184($fp)
	lw $t0, -180($fp)
	lw $t1, -184($fp)
	add $t2, $t0, $t1
	sw $t2, -188($fp)
	lw $t0, -188($fp)
	lw $t1, 0($t0)
	sw $t1, -192($fp)
	lw $t0, -192($fp)
	lw $t1, -60($fp)
	bgt $t0, $t1, L14
	j L15
L14:
	li $t0, 0
	sw $t0, -72($fp)
	subu $t0, $fp, 44
	sw $t0, -196($fp)
	lw $t0, -56($fp)
	li $t1, 4
	mul $t2, $t0, $t1
	sw $t2, -200($fp)
	lw $t0, -196($fp)
	lw $t1, -200($fp)
	add $t2, $t0, $t1
	sw $t2, -204($fp)
	subu $t0, $fp, 44
	sw $t0, -208($fp)
	lw $t0, -48($fp)
	li $t1, 4
	mul $t2, $t0, $t1
	sw $t2, -212($fp)
	lw $t0, -208($fp)
	lw $t1, -212($fp)
	add $t2, $t0, $t1
	sw $t2, -216($fp)
	lw $t0, -216($fp)
	lw $t1, 0($t0)
	sw $t1, -220($fp)
	lw $t0, -204($fp)
	lw $t1, -220($fp)
	sw $t1, 0($t0)
	subu $t0, $fp, 44
	sw $t0, -224($fp)
	lw $t0, -48($fp)
	li $t1, 4
	mul $t2, $t0, $t1
	sw $t2, -228($fp)
	lw $t0, -224($fp)
	lw $t1, -228($fp)
	add $t2, $t0, $t1
	sw $t2, -232($fp)
	lw $t0, -232($fp)
	lw $t1, -60($fp)
	sw $t1, 0($t0)
	lw $t0, -56($fp)
	sw $t0, -48($fp)
L15:
L10:
	j L6
L8:
	lw $t0, -52($fp)
	li $t1, 1
	sub $t2, $t0, $t1
	sw $t2, -236($fp)
	lw $t0, -236($fp)
	sw $t0, -52($fp)
	j L3
L5:
	li $t0, 0
	sw $t0, -48($fp)
L16:
	lw $t0, -48($fp)
	lw $t1, -4($fp)
	blt $t0, $t1, L17
	j L18
L17:
	subu $t0, $fp, 44
	sw $t0, -240($fp)
	lw $t0, -48($fp)
	li $t1, 4
	mul $t2, $t0, $t1
	sw $t2, -244($fp)
	lw $t0, -240($fp)
	lw $t1, -244($fp)
	add $t2, $t0, $t1
	sw $t2, -248($fp)
	lw $t0, -248($fp)
	lw $t1, 0($t0)
	sw $t1, -252($fp)
	lw $t0, -252($fp)
	move $a0, $t0
	subu $sp, $sp, 4
	sw $ra, 0($sp)
	jal write
	lw $ra, 0($sp)
	addu $sp, $sp, 4
	lw $t0, -48($fp)
	li $t1, 1
	add $t2, $t0, $t1
	sw $t2, -256($fp)
	lw $t0, -256($fp)
	sw $t0, -48($fp)
	j L16
L18:
	li $t0, 0
	move $v0, $t0
	addu $sp, $sp, 256
	lw $fp, 0($sp)
	addu $sp, $sp, 4
	jr $ra
