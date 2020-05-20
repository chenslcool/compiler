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

safe_add:
	subu $sp, $sp, 4
	sw $fp, 0($sp)
	move $fp, $sp
	subu $sp, $sp, 60
	sw $a0, -8($fp)
	sw $a1, -4($fp)
	li $t0, 0
	sw $t0, -12($fp)
	li $t0, 0
	sw $t0, -16($fp)
	li $t0, 0
	sw $t0, -20($fp)
	lw $t0, -4($fp)
	li $t1, 0
	bgt $t0, $t1, L9
	j L10
L9:
	li $t0, 1
	sw $t0, -20($fp)
L10:
	lw $t0, -20($fp)
	li $t1, 0
	bne $t0, $t1, L8
	j L6
L8:
	li $t0, 0
	sw $t0, -24($fp)
	lw $t0, -8($fp)
	li $t1, 0
	bgt $t0, $t1, L11
	j L12
L11:
	li $t0, 1
	sw $t0, -24($fp)
L12:
	lw $t0, -24($fp)
	li $t1, 0
	bne $t0, $t1, L7
	j L6
L7:
	li $t0, 0
	sw $t0, -28($fp)
	li $t0, 2147483647
	lw $t1, -8($fp)
	sub $t2, $t0, $t1
	sw $t2, -32($fp)
	lw $t0, -4($fp)
	lw $t1, -32($fp)
	bgt $t0, $t1, L13
	j L14
L13:
	li $t0, 1
	sw $t0, -28($fp)
L14:
	lw $t0, -28($fp)
	li $t1, 0
	bne $t0, $t1, L5
	j L6
L5:
	li $t0, 1
	sw $t0, -16($fp)
L6:
	lw $t0, -16($fp)
	li $t1, 0
	bne $t0, $t1, L2
	j L4
L4:
	li $t0, 0
	sw $t0, -36($fp)
	li $t0, 0
	sw $t0, -40($fp)
	lw $t0, -4($fp)
	li $t1, 0
	blt $t0, $t1, L19
	j L20
L19:
	li $t0, 1
	sw $t0, -40($fp)
L20:
	lw $t0, -40($fp)
	li $t1, 0
	bne $t0, $t1, L18
	j L16
L18:
	li $t0, 0
	sw $t0, -44($fp)
	lw $t0, -8($fp)
	li $t1, 0
	blt $t0, $t1, L21
	j L22
L21:
	li $t0, 1
	sw $t0, -44($fp)
L22:
	lw $t0, -44($fp)
	li $t1, 0
	bne $t0, $t1, L17
	j L16
L17:
	li $t0, 0
	sw $t0, -48($fp)
	li $t0, 0
	li $t1, -2147483648
	sub $t2, $t0, $t1
	sw $t2, -52($fp)
	lw $t0, -52($fp)
	lw $t1, -8($fp)
	sub $t2, $t0, $t1
	sw $t2, -56($fp)
	lw $t0, -4($fp)
	lw $t1, -56($fp)
	blt $t0, $t1, L23
	j L24
L23:
	li $t0, 1
	sw $t0, -48($fp)
L24:
	lw $t0, -48($fp)
	li $t1, 0
	bne $t0, $t1, L15
	j L16
L15:
	li $t0, 1
	sw $t0, -36($fp)
L16:
	lw $t0, -36($fp)
	li $t1, 0
	bne $t0, $t1, L2
	j L3
L2:
	li $t0, 1
	sw $t0, -12($fp)
L3:
	lw $t0, -12($fp)
	li $t1, 0
	bne $t0, $t1, L0
	j L1
L0:
	lw $t0, -4($fp)
	move $v0, $t0
	addu $sp, $sp, 60
	lw $fp, 0($sp)
	addu $sp, $sp, 4
	jr $ra
L1:
	lw $t0, -4($fp)
	lw $t1, -8($fp)
	add $t2, $t0, $t1
	sw $t2, -60($fp)
	lw $t0, -60($fp)
	move $v0, $t0
	addu $sp, $sp, 60
	lw $fp, 0($sp)
	addu $sp, $sp, 4
	jr $ra

safe_sub:
	subu $sp, $sp, 4
	sw $fp, 0($sp)
	move $fp, $sp
	subu $sp, $sp, 56
	sw $a0, -8($fp)
	sw $a1, -4($fp)
	li $t0, 0
	sw $t0, -12($fp)
	li $t0, 0
	sw $t0, -16($fp)
	lw $t0, -4($fp)
	li $t1, 0
	bgt $t0, $t1, L32
	j L33
L32:
	li $t0, 1
	sw $t0, -16($fp)
L33:
	lw $t0, -16($fp)
	li $t1, 0
	bne $t0, $t1, L31
	j L29
L31:
	li $t0, 0
	sw $t0, -20($fp)
	lw $t0, -8($fp)
	li $t1, 0
	blt $t0, $t1, L34
	j L35
L34:
	li $t0, 1
	sw $t0, -20($fp)
L35:
	lw $t0, -20($fp)
	li $t1, 0
	bne $t0, $t1, L30
	j L29
L30:
	li $t0, 0
	sw $t0, -24($fp)
	li $t0, 2147483647
	lw $t1, -8($fp)
	add $t2, $t0, $t1
	sw $t2, -28($fp)
	lw $t0, -4($fp)
	lw $t1, -28($fp)
	bgt $t0, $t1, L36
	j L37
L36:
	li $t0, 1
	sw $t0, -24($fp)
L37:
	lw $t0, -24($fp)
	li $t1, 0
	bne $t0, $t1, L28
	j L29
L28:
	li $t0, 1
	sw $t0, -12($fp)
L29:
	lw $t0, -12($fp)
	li $t1, 0
	bne $t0, $t1, L25
	j L27
L27:
	li $t0, 0
	sw $t0, -32($fp)
	li $t0, 0
	sw $t0, -36($fp)
	lw $t0, -4($fp)
	li $t1, 0
	blt $t0, $t1, L42
	j L43
L42:
	li $t0, 1
	sw $t0, -36($fp)
L43:
	lw $t0, -36($fp)
	li $t1, 0
	bne $t0, $t1, L41
	j L39
L41:
	li $t0, 0
	sw $t0, -40($fp)
	lw $t0, -8($fp)
	li $t1, 0
	bgt $t0, $t1, L44
	j L45
L44:
	li $t0, 1
	sw $t0, -40($fp)
L45:
	lw $t0, -40($fp)
	li $t1, 0
	bne $t0, $t1, L40
	j L39
L40:
	li $t0, 0
	sw $t0, -44($fp)
	li $t0, 0
	li $t1, -2147483648
	sub $t2, $t0, $t1
	sw $t2, -48($fp)
	lw $t0, -48($fp)
	lw $t1, -8($fp)
	add $t2, $t0, $t1
	sw $t2, -52($fp)
	lw $t0, -4($fp)
	lw $t1, -52($fp)
	blt $t0, $t1, L46
	j L47
L46:
	li $t0, 1
	sw $t0, -44($fp)
L47:
	lw $t0, -44($fp)
	li $t1, 0
	bne $t0, $t1, L38
	j L39
L38:
	li $t0, 1
	sw $t0, -32($fp)
L39:
	lw $t0, -32($fp)
	li $t1, 0
	bne $t0, $t1, L25
	j L26
L25:
	lw $t0, -4($fp)
	move $v0, $t0
	addu $sp, $sp, 56
	lw $fp, 0($sp)
	addu $sp, $sp, 4
	jr $ra
L26:
	lw $t0, -4($fp)
	lw $t1, -8($fp)
	sub $t2, $t0, $t1
	sw $t2, -56($fp)
	lw $t0, -56($fp)
	move $v0, $t0
	addu $sp, $sp, 56
	lw $fp, 0($sp)
	addu $sp, $sp, 4
	jr $ra

safe_mul:
	subu $sp, $sp, 4
	sw $fp, 0($sp)
	move $fp, $sp
	subu $sp, $sp, 108
	sw $a0, -8($fp)
	sw $a1, -4($fp)
	li $t0, 0
	sw $t0, -12($fp)
	li $t0, 0
	sw $t0, -16($fp)
	li $t0, 0
	sw $t0, -20($fp)
	lw $t0, -4($fp)
	li $t1, 0
	bgt $t0, $t1, L59
	j L60
L59:
	li $t0, 1
	sw $t0, -20($fp)
L60:
	lw $t0, -20($fp)
	li $t1, 0
	bne $t0, $t1, L58
	j L56
L58:
	li $t0, 0
	sw $t0, -24($fp)
	lw $t0, -8($fp)
	li $t1, 0
	bgt $t0, $t1, L61
	j L62
L61:
	li $t0, 1
	sw $t0, -24($fp)
L62:
	lw $t0, -24($fp)
	li $t1, 0
	bne $t0, $t1, L57
	j L56
L57:
	li $t0, 0
	sw $t0, -28($fp)
	li $t0, 2147483647
	lw $t1, -8($fp)
	div $t0, $t1
	mflo $t2	
	sw $t2, -32($fp)
	lw $t0, -4($fp)
	lw $t1, -32($fp)
	bgt $t0, $t1, L63
	j L64
L63:
	li $t0, 1
	sw $t0, -28($fp)
L64:
	lw $t0, -28($fp)
	li $t1, 0
	bne $t0, $t1, L55
	j L56
L55:
	li $t0, 1
	sw $t0, -16($fp)
L56:
	lw $t0, -16($fp)
	li $t1, 0
	bne $t0, $t1, L50
	j L54
L54:
	li $t0, 0
	sw $t0, -36($fp)
	li $t0, 0
	sw $t0, -40($fp)
	lw $t0, -4($fp)
	li $t1, 0
	bgt $t0, $t1, L69
	j L70
L69:
	li $t0, 1
	sw $t0, -40($fp)
L70:
	lw $t0, -40($fp)
	li $t1, 0
	bne $t0, $t1, L68
	j L66
L68:
	li $t0, 0
	sw $t0, -44($fp)
	lw $t0, -8($fp)
	li $t1, 0
	ble $t0, $t1, L71
	j L72
L71:
	li $t0, 1
	sw $t0, -44($fp)
L72:
	lw $t0, -44($fp)
	li $t1, 0
	bne $t0, $t1, L67
	j L66
L67:
	li $t0, 0
	sw $t0, -48($fp)
	li $t0, 0
	li $t1, -2147483648
	sub $t2, $t0, $t1
	sw $t2, -52($fp)
	lw $t0, -52($fp)
	lw $t1, -4($fp)
	div $t0, $t1
	mflo $t2	
	sw $t2, -56($fp)
	lw $t0, -8($fp)
	lw $t1, -56($fp)
	blt $t0, $t1, L73
	j L74
L73:
	li $t0, 1
	sw $t0, -48($fp)
L74:
	lw $t0, -48($fp)
	li $t1, 0
	bne $t0, $t1, L65
	j L66
L65:
	li $t0, 1
	sw $t0, -36($fp)
L66:
	lw $t0, -36($fp)
	li $t1, 0
	bne $t0, $t1, L50
	j L53
L53:
	li $t0, 0
	sw $t0, -60($fp)
	li $t0, 0
	sw $t0, -64($fp)
	lw $t0, -4($fp)
	li $t1, 0
	ble $t0, $t1, L79
	j L80
L79:
	li $t0, 1
	sw $t0, -64($fp)
L80:
	lw $t0, -64($fp)
	li $t1, 0
	bne $t0, $t1, L78
	j L76
L78:
	li $t0, 0
	sw $t0, -68($fp)
	lw $t0, -8($fp)
	li $t1, 0
	bgt $t0, $t1, L81
	j L82
L81:
	li $t0, 1
	sw $t0, -68($fp)
L82:
	lw $t0, -68($fp)
	li $t1, 0
	bne $t0, $t1, L77
	j L76
L77:
	li $t0, 0
	sw $t0, -72($fp)
	li $t0, 0
	li $t1, -2147483648
	sub $t2, $t0, $t1
	sw $t2, -76($fp)
	lw $t0, -76($fp)
	lw $t1, -8($fp)
	div $t0, $t1
	mflo $t2	
	sw $t2, -80($fp)
	lw $t0, -4($fp)
	lw $t1, -80($fp)
	blt $t0, $t1, L83
	j L84
L83:
	li $t0, 1
	sw $t0, -72($fp)
L84:
	lw $t0, -72($fp)
	li $t1, 0
	bne $t0, $t1, L75
	j L76
L75:
	li $t0, 1
	sw $t0, -60($fp)
L76:
	lw $t0, -60($fp)
	li $t1, 0
	bne $t0, $t1, L50
	j L52
L52:
	li $t0, 0
	sw $t0, -84($fp)
	li $t0, 0
	sw $t0, -88($fp)
	lw $t0, -4($fp)
	li $t1, 0
	ble $t0, $t1, L90
	j L91
L90:
	li $t0, 1
	sw $t0, -88($fp)
L91:
	lw $t0, -88($fp)
	li $t1, 0
	bne $t0, $t1, L89
	j L86
L89:
	li $t0, 0
	sw $t0, -92($fp)
	lw $t0, -8($fp)
	li $t1, 0
	ble $t0, $t1, L92
	j L93
L92:
	li $t0, 1
	sw $t0, -92($fp)
L93:
	lw $t0, -92($fp)
	li $t1, 0
	bne $t0, $t1, L88
	j L86
L88:
	li $t0, 0
	sw $t0, -96($fp)
	lw $t0, -4($fp)
	li $t1, 0
	bne $t0, $t1, L94
	j L95
L94:
	li $t0, 1
	sw $t0, -96($fp)
L95:
	lw $t0, -96($fp)
	li $t1, 0
	bne $t0, $t1, L87
	j L86
L87:
	li $t0, 0
	sw $t0, -100($fp)
	li $t0, 2147483647
	lw $t1, -4($fp)
	div $t0, $t1
	mflo $t2	
	sw $t2, -104($fp)
	lw $t0, -8($fp)
	lw $t1, -104($fp)
	blt $t0, $t1, L96
	j L97
L96:
	li $t0, 1
	sw $t0, -100($fp)
L97:
	lw $t0, -100($fp)
	li $t1, 0
	bne $t0, $t1, L85
	j L86
L85:
	li $t0, 1
	sw $t0, -84($fp)
L86:
	lw $t0, -84($fp)
	li $t1, 0
	bne $t0, $t1, L50
	j L51
L50:
	li $t0, 1
	sw $t0, -12($fp)
L51:
	lw $t0, -12($fp)
	li $t1, 0
	bne $t0, $t1, L48
	j L49
L48:
	lw $t0, -4($fp)
	move $v0, $t0
	addu $sp, $sp, 108
	lw $fp, 0($sp)
	addu $sp, $sp, 4
	jr $ra
L49:
	lw $t0, -4($fp)
	lw $t1, -8($fp)
	mul $t2, $t0, $t1
	sw $t2, -108($fp)
	lw $t0, -108($fp)
	move $v0, $t0
	addu $sp, $sp, 108
	lw $fp, 0($sp)
	addu $sp, $sp, 4
	jr $ra

safe_div:
	subu $sp, $sp, 4
	sw $fp, 0($sp)
	move $fp, $sp
	subu $sp, $sp, 40
	sw $a0, -8($fp)
	sw $a1, -4($fp)
	li $t0, 0
	sw $t0, -12($fp)
	li $t0, 0
	sw $t0, -16($fp)
	lw $t0, -8($fp)
	li $t1, 0
	beq $t0, $t1, L103
	j L104
L103:
	li $t0, 1
	sw $t0, -16($fp)
L104:
	lw $t0, -16($fp)
	li $t1, 0
	bne $t0, $t1, L100
	j L102
L102:
	li $t0, 0
	sw $t0, -20($fp)
	li $t0, 0
	sw $t0, -24($fp)
	li $t0, 0
	li $t1, -2147483648
	sub $t2, $t0, $t1
	sw $t2, -28($fp)
	lw $t0, -4($fp)
	lw $t1, -28($fp)
	beq $t0, $t1, L108
	j L109
L108:
	li $t0, 1
	sw $t0, -24($fp)
L109:
	lw $t0, -24($fp)
	li $t1, 0
	bne $t0, $t1, L107
	j L106
L107:
	li $t0, 0
	sw $t0, -32($fp)
	li $t0, 0
	li $t1, 1
	sub $t2, $t0, $t1
	sw $t2, -36($fp)
	lw $t0, -8($fp)
	lw $t1, -36($fp)
	beq $t0, $t1, L110
	j L111
L110:
	li $t0, 1
	sw $t0, -32($fp)
L111:
	lw $t0, -32($fp)
	li $t1, 0
	bne $t0, $t1, L105
	j L106
L105:
	li $t0, 1
	sw $t0, -20($fp)
L106:
	lw $t0, -20($fp)
	li $t1, 0
	bne $t0, $t1, L100
	j L101
L100:
	li $t0, 1
	sw $t0, -12($fp)
L101:
	lw $t0, -12($fp)
	li $t1, 0
	bne $t0, $t1, L98
	j L99
L98:
	lw $t0, -4($fp)
	move $v0, $t0
	addu $sp, $sp, 40
	lw $fp, 0($sp)
	addu $sp, $sp, 4
	jr $ra
L99:
	lw $t0, -4($fp)
	lw $t1, -8($fp)
	div $t0, $t1
	mflo $t2	
	sw $t2, -40($fp)
	lw $t0, -40($fp)
	move $v0, $t0
	addu $sp, $sp, 40
	lw $fp, 0($sp)
	addu $sp, $sp, 4
	jr $ra

safe_index:
	subu $sp, $sp, 4
	sw $fp, 0($sp)
	move $fp, $sp
	subu $sp, $sp, 8
	sw $a0, -8($fp)
	sw $a1, -4($fp)
	lw $t0, -4($fp)
	li $t1, 0
	blt $t0, $t1, L112
	j L114
L114:
	lw $t0, -4($fp)
	lw $t1, -8($fp)
	bge $t0, $t1, L112
	j L113
L112:
	li $t0, 0
	move $v0, $t0
	addu $sp, $sp, 8
	lw $fp, 0($sp)
	addu $sp, $sp, 4
	jr $ra
L113:
	lw $t0, -4($fp)
	move $v0, $t0
	addu $sp, $sp, 8
	lw $fp, 0($sp)
	addu $sp, $sp, 4
	jr $ra

i_func_70:
	subu $sp, $sp, 4
	sw $fp, 0($sp)
	move $fp, $sp
	subu $sp, $sp, 108
	sw $a0, -12($fp)
	sw $a1, -8($fp)
	sw $a2, -4($fp)
	lw $t0, -12($fp)
	li $t1, 0
	ble $t0, $t1, L115
	j L116
L115:
	li $t0, 0
	li $t1, 1995635513
	sub $t2, $t0, $t1
	sw $t2, -32($fp)
	lw $t0, -32($fp)
	move $v0, $t0
	addu $sp, $sp, 108
	lw $fp, 0($sp)
	addu $sp, $sp, 4
	jr $ra
	j L117
L116:
	lw $t0, -12($fp)
	li $t1, 1
	sub $t2, $t0, $t1
	sw $t2, -36($fp)
	lw $t0, -36($fp)
	sw $t0, -12($fp)
L117:
	li $t0, 1
	sw $t0, -16($fp)
	lw $t0, -12($fp)
	li $t1, 1
	sub $t2, $t0, $t1
	sw $t2, -40($fp)
	lw $t0, -40($fp)
	sw $t0, -12($fp)
	lw $t0, -12($fp)
	move $a0, $t0
	lw $t0, -16($fp)
	move $a1, $t0
	lw $t0, -8($fp)
	move $a2, $t0
	subu $sp, $sp, 4
	sw $ra, 0($sp)
	jal i_func_70
	sw $v0, -44($fp)
	lw $ra, 0($sp)
	addu $sp, $sp, 4
	lw $t0, -44($fp)
	sw $t0, -4($fp)
	lw $t0, -44($fp)
	sw $t0, -48($fp)
	lw $t0, -48($fp)
	sw $t0, -20($fp)
	lw $t0, -12($fp)
	li $t1, 1
	sub $t2, $t0, $t1
	sw $t2, -52($fp)
	lw $t0, -52($fp)
	sw $t0, -12($fp)
	li $t0, 0
	li $t1, 1326831199
	sub $t2, $t0, $t1
	sw $t2, -56($fp)
	lw $t0, -12($fp)
	move $a0, $t0
	lw $t0, -56($fp)
	move $a1, $t0
	lw $t0, -8($fp)
	move $a2, $t0
	subu $sp, $sp, 4
	sw $ra, 0($sp)
	jal i_func_70
	sw $v0, -60($fp)
	lw $ra, 0($sp)
	addu $sp, $sp, 4
	lw $t0, -60($fp)
	sw $t0, -4($fp)
	lw $t0, -60($fp)
	sw $t0, -64($fp)
	lw $t0, -64($fp)
	sw $t0, -24($fp)
	li $t0, 0
	sw $t0, -68($fp)
	lw $t0, -8($fp)
	li $t1, 0
	bne $t0, $t1, L119
	j L118
L118:
	li $t0, 1
	sw $t0, -68($fp)
L119:
	lw $t0, -68($fp)
	sw $t0, -4($fp)
	lw $t0, -68($fp)
	sw $t0, -72($fp)
	lw $t0, -72($fp)
	sw $t0, -28($fp)
	lw $t0, -12($fp)
	li $t1, 2
	sub $t2, $t0, $t1
	sw $t2, -76($fp)
	lw $t0, -76($fp)
	sw $t0, -12($fp)
	lw $t0, -12($fp)
	move $a0, $t0
	lw $t0, -8($fp)
	move $a1, $t0
	lw $t0, -4($fp)
	move $a2, $t0
	subu $sp, $sp, 4
	sw $ra, 0($sp)
	jal i_func_70
	sw $v0, -80($fp)
	lw $ra, 0($sp)
	addu $sp, $sp, 4
	li $t0, 0
	li $t1, 783396382
	sub $t2, $t0, $t1
	sw $t2, -84($fp)
	lw $t0, -16($fp)
	lw $t1, -84($fp)
	mul $t2, $t0, $t1
	sw $t2, -88($fp)
	lw $t0, -12($fp)
	move $a0, $t0
	lw $t0, -88($fp)
	move $a1, $t0
	lw $t0, -80($fp)
	move $a2, $t0
	subu $sp, $sp, 4
	sw $ra, 0($sp)
	jal i_func_70
	sw $v0, -92($fp)
	lw $ra, 0($sp)
	addu $sp, $sp, 4
	lw $t0, -12($fp)
	li $t1, 1
	sub $t2, $t0, $t1
	sw $t2, -96($fp)
	lw $t0, -96($fp)
	sw $t0, -12($fp)
	li $t0, 0
	li $t1, 313143259
	sub $t2, $t0, $t1
	sw $t2, -100($fp)
	lw $t0, -12($fp)
	move $a0, $t0
	lw $t0, -8($fp)
	move $a1, $t0
	lw $t0, -100($fp)
	move $a2, $t0
	subu $sp, $sp, 4
	sw $ra, 0($sp)
	jal i_func_70
	sw $v0, -104($fp)
	lw $ra, 0($sp)
	addu $sp, $sp, 4
	lw $t0, -104($fp)
	sw $t0, -4($fp)
	lw $t0, -16($fp)
	move $a0, $t0
	subu $sp, $sp, 4
	sw $ra, 0($sp)
	jal write
	lw $ra, 0($sp)
	addu $sp, $sp, 4
	lw $t0, -20($fp)
	move $a0, $t0
	subu $sp, $sp, 4
	sw $ra, 0($sp)
	jal write
	lw $ra, 0($sp)
	addu $sp, $sp, 4
	lw $t0, -24($fp)
	move $a0, $t0
	subu $sp, $sp, 4
	sw $ra, 0($sp)
	jal write
	lw $ra, 0($sp)
	addu $sp, $sp, 4
	lw $t0, -28($fp)
	move $a0, $t0
	subu $sp, $sp, 4
	sw $ra, 0($sp)
	jal write
	lw $ra, 0($sp)
	addu $sp, $sp, 4
	li $t0, 449561367
	sw $t0, -4($fp)
	li $t0, 449561367
	sw $t0, -108($fp)
	lw $t0, -108($fp)
	move $v0, $t0
	addu $sp, $sp, 108
	lw $fp, 0($sp)
	addu $sp, $sp, 4
	jr $ra

top_caller:
	subu $sp, $sp, 4
	sw $fp, 0($sp)
	move $fp, $sp
	subu $sp, $sp, 448
	sw $a0, -4($fp)
	lw $t0, -4($fp)
	li $t1, 0
	ble $t0, $t1, L120
	j L121
L120:
	li $t0, 0
	li $t1, 242219461
	sub $t2, $t0, $t1
	sw $t2, -48($fp)
	lw $t0, -48($fp)
	move $v0, $t0
	addu $sp, $sp, 448
	lw $fp, 0($sp)
	addu $sp, $sp, 4
	jr $ra
	j L122
L121:
	lw $t0, -4($fp)
	li $t1, 1
	sub $t2, $t0, $t1
	sw $t2, -52($fp)
	lw $t0, -52($fp)
	sw $t0, -4($fp)
L122:
	li $t0, 0
	li $t1, 772560566
	sub $t2, $t0, $t1
	sw $t2, -56($fp)
	lw $t0, -56($fp)
	sw $t0, -8($fp)
	lw $t0, -8($fp)
	sw $t0, -12($fp)
	li $t0, 0
	li $t1, 1251629067
	sub $t2, $t0, $t1
	sw $t2, -60($fp)
	lw $t0, -60($fp)
	sw $t0, -16($fp)
	lw $t0, -16($fp)
	sw $t0, -20($fp)
	lw $t0, -4($fp)
	li $t1, 1
	sub $t2, $t0, $t1
	sw $t2, -64($fp)
	lw $t0, -64($fp)
	sw $t0, -4($fp)
	subu $t0, $fp, 24
	sw $t0, -68($fp)
	li $t0, 1
	li $t1, 4
	mul $t2, $t0, $t1
	sw $t2, -72($fp)
	lw $t0, -68($fp)
	lw $t1, -72($fp)
	add $t2, $t0, $t1
	sw $t2, -76($fp)
	li $t0, 331834534
	sw $t0, -20($fp)
	li $t0, 331834534
	sw $t0, -80($fp)
	lw $t0, -4($fp)
	move $a0, $t0
	lw $t0, -80($fp)
	move $a1, $t0
	lw $t0, -12($fp)
	move $a2, $t0
	subu $sp, $sp, 4
	sw $ra, 0($sp)
	jal i_func_70
	sw $v0, -84($fp)
	lw $ra, 0($sp)
	addu $sp, $sp, 4
	lw $t0, -76($fp)
	lw $t1, -84($fp)
	sw $t1, 0($t0)
	subu $t0, $fp, 24
	sw $t0, -88($fp)
	li $t0, 0
	li $t1, 4
	mul $t2, $t0, $t1
	sw $t2, -92($fp)
	lw $t0, -88($fp)
	lw $t1, -92($fp)
	add $t2, $t0, $t1
	sw $t2, -96($fp)
	li $t0, 0
	sw $t0, -100($fp)
	lw $t0, -12($fp)
	li $t1, 0
	bne $t0, $t1, L123
	j L125
L125:
	li $t0, 24770670
	li $t1, 0
	bne $t0, $t1, L123
	j L124
L123:
	li $t0, 1
	sw $t0, -100($fp)
L124:
	lw $t0, -100($fp)
	sw $t0, -8($fp)
	lw $t0, -100($fp)
	sw $t0, -104($fp)
	lw $t0, -96($fp)
	lw $t1, -104($fp)
	sw $t1, 0($t0)
	lw $t0, -4($fp)
	li $t1, 1
	sub $t2, $t0, $t1
	sw $t2, -108($fp)
	lw $t0, -108($fp)
	sw $t0, -4($fp)
	subu $t0, $fp, 32
	sw $t0, -112($fp)
	li $t0, 1
	li $t1, 4
	mul $t2, $t0, $t1
	sw $t2, -116($fp)
	lw $t0, -112($fp)
	lw $t1, -116($fp)
	add $t2, $t0, $t1
	sw $t2, -120($fp)
	subu $t0, $fp, 24
	sw $t0, -124($fp)
	lw $t0, -8($fp)
	li $t1, 4
	mul $t2, $t0, $t1
	sw $t2, -128($fp)
	lw $t0, -124($fp)
	lw $t1, -128($fp)
	add $t2, $t0, $t1
	sw $t2, -132($fp)
	lw $t0, -4($fp)
	move $a0, $t0
	lw $t0, -16($fp)
	move $a1, $t0
	lw $t0, -12($fp)
	move $a2, $t0
	subu $sp, $sp, 4
	sw $ra, 0($sp)
	jal i_func_70
	sw $v0, -136($fp)
	lw $ra, 0($sp)
	addu $sp, $sp, 4
	lw $t0, -132($fp)
	lw $t1, -136($fp)
	sw $t1, 0($t0)
	lw $t0, -136($fp)
	sw $t0, -140($fp)
	lw $t0, -120($fp)
	lw $t1, -140($fp)
	sw $t1, 0($t0)
	subu $t0, $fp, 32
	sw $t0, -144($fp)
	li $t0, 0
	li $t1, 4
	mul $t2, $t0, $t1
	sw $t2, -148($fp)
	lw $t0, -144($fp)
	lw $t1, -148($fp)
	add $t2, $t0, $t1
	sw $t2, -152($fp)
	subu $t0, $fp, 24
	sw $t0, -156($fp)
	lw $t0, -8($fp)
	li $t1, 4
	mul $t2, $t0, $t1
	sw $t2, -160($fp)
	lw $t0, -156($fp)
	lw $t1, -160($fp)
	add $t2, $t0, $t1
	sw $t2, -164($fp)
	lw $t0, -12($fp)
	lw $t1, -16($fp)
	sub $t2, $t0, $t1
	sw $t2, -168($fp)
	lw $t0, -164($fp)
	lw $t1, -168($fp)
	sw $t1, 0($t0)
	lw $t0, -168($fp)
	sw $t0, -172($fp)
	lw $t0, -152($fp)
	lw $t1, -172($fp)
	sw $t1, 0($t0)
	lw $t0, -4($fp)
	li $t1, 1
	sub $t2, $t0, $t1
	sw $t2, -176($fp)
	lw $t0, -176($fp)
	sw $t0, -4($fp)
	subu $t0, $fp, 40
	sw $t0, -180($fp)
	li $t0, 1
	li $t1, 4
	mul $t2, $t0, $t1
	sw $t2, -184($fp)
	lw $t0, -180($fp)
	lw $t1, -184($fp)
	add $t2, $t0, $t1
	sw $t2, -188($fp)
	subu $t0, $fp, 24
	sw $t0, -192($fp)
	subu $t0, $fp, 32
	sw $t0, -196($fp)
	li $t0, 0
	li $t1, 841029242
	sub $t2, $t0, $t1
	sw $t2, -200($fp)
	lw $t0, -200($fp)
	li $t1, 841029243
	add $t2, $t0, $t1
	sw $t2, -204($fp)
	lw $t0, -204($fp)
	li $t1, 4
	mul $t2, $t0, $t1
	sw $t2, -208($fp)
	lw $t0, -196($fp)
	lw $t1, -208($fp)
	add $t2, $t0, $t1
	sw $t2, -212($fp)
	lw $t0, -212($fp)
	lw $t1, 0($t0)
	sw $t1, -216($fp)
	li $t0, 2
	move $a0, $t0
	lw $t0, -216($fp)
	move $a1, $t0
	subu $sp, $sp, 4
	sw $ra, 0($sp)
	jal safe_index
	sw $v0, -220($fp)
	lw $ra, 0($sp)
	addu $sp, $sp, 4
	lw $t0, -220($fp)
	li $t1, 4
	mul $t2, $t0, $t1
	sw $t2, -224($fp)
	lw $t0, -192($fp)
	lw $t1, -224($fp)
	add $t2, $t0, $t1
	sw $t2, -228($fp)
	lw $t0, -4($fp)
	move $a0, $t0
	subu $sp, $sp, 4
	sw $ra, 0($sp)
	jal top_caller
	sw $v0, -232($fp)
	lw $ra, 0($sp)
	addu $sp, $sp, 4
	lw $t0, -228($fp)
	lw $t1, -232($fp)
	sw $t1, 0($t0)
	lw $t0, -232($fp)
	sw $t0, -236($fp)
	lw $t0, -188($fp)
	lw $t1, -236($fp)
	sw $t1, 0($t0)
	subu $t0, $fp, 40
	sw $t0, -240($fp)
	li $t0, 0
	li $t1, 4
	mul $t2, $t0, $t1
	sw $t2, -244($fp)
	lw $t0, -240($fp)
	lw $t1, -244($fp)
	add $t2, $t0, $t1
	sw $t2, -248($fp)
	subu $t0, $fp, 24
	sw $t0, -252($fp)
	subu $t0, $fp, 32
	sw $t0, -256($fp)
	li $t0, 0
	li $t1, 295183140
	sub $t2, $t0, $t1
	sw $t2, -260($fp)
	li $t0, 295183141
	lw $t1, -260($fp)
	add $t2, $t0, $t1
	sw $t2, -264($fp)
	lw $t0, -264($fp)
	li $t1, 4
	mul $t2, $t0, $t1
	sw $t2, -268($fp)
	lw $t0, -256($fp)
	lw $t1, -268($fp)
	add $t2, $t0, $t1
	sw $t2, -272($fp)
	lw $t0, -272($fp)
	lw $t1, 0($t0)
	sw $t1, -276($fp)
	li $t0, 2
	move $a0, $t0
	lw $t0, -276($fp)
	move $a1, $t0
	subu $sp, $sp, 4
	sw $ra, 0($sp)
	jal safe_index
	sw $v0, -280($fp)
	lw $ra, 0($sp)
	addu $sp, $sp, 4
	lw $t0, -280($fp)
	li $t1, 4
	mul $t2, $t0, $t1
	sw $t2, -284($fp)
	lw $t0, -252($fp)
	lw $t1, -284($fp)
	add $t2, $t0, $t1
	sw $t2, -288($fp)
	lw $t0, -288($fp)
	li $t1, 1813673313
	sw $t1, 0($t0)
	li $t0, 1813673313
	sw $t0, -292($fp)
	lw $t0, -248($fp)
	lw $t1, -292($fp)
	sw $t1, 0($t0)
	subu $t0, $fp, 24
	sw $t0, -296($fp)
	subu $t0, $fp, 32
	sw $t0, -300($fp)
	li $t0, 0
	li $t1, 491780218
	sub $t2, $t0, $t1
	sw $t2, -304($fp)
	li $t0, 491780219
	lw $t1, -304($fp)
	add $t2, $t0, $t1
	sw $t2, -308($fp)
	lw $t0, -308($fp)
	li $t1, 4
	mul $t2, $t0, $t1
	sw $t2, -312($fp)
	lw $t0, -300($fp)
	lw $t1, -312($fp)
	add $t2, $t0, $t1
	sw $t2, -316($fp)
	lw $t0, -316($fp)
	lw $t1, 0($t0)
	sw $t1, -320($fp)
	li $t0, 2
	move $a0, $t0
	lw $t0, -320($fp)
	move $a1, $t0
	subu $sp, $sp, 4
	sw $ra, 0($sp)
	jal safe_index
	sw $v0, -324($fp)
	lw $ra, 0($sp)
	addu $sp, $sp, 4
	lw $t0, -324($fp)
	li $t1, 4
	mul $t2, $t0, $t1
	sw $t2, -328($fp)
	lw $t0, -296($fp)
	lw $t1, -328($fp)
	add $t2, $t0, $t1
	sw $t2, -332($fp)
	subu $t0, $fp, 40
	sw $t0, -336($fp)
	li $t0, 0
	li $t1, 832323856
	sub $t2, $t0, $t1
	sw $t2, -340($fp)
	li $t0, 832323856
	lw $t1, -340($fp)
	add $t2, $t0, $t1
	sw $t2, -344($fp)
	lw $t0, -344($fp)
	li $t1, 4
	mul $t2, $t0, $t1
	sw $t2, -348($fp)
	lw $t0, -336($fp)
	lw $t1, -348($fp)
	add $t2, $t0, $t1
	sw $t2, -352($fp)
	lw $t0, -352($fp)
	lw $t1, 0($t0)
	sw $t1, -356($fp)
	lw $t0, -356($fp)
	lw $t1, -8($fp)
	sub $t2, $t0, $t1
	sw $t2, -360($fp)
	lw $t0, -332($fp)
	lw $t1, -360($fp)
	sw $t1, 0($t0)
	lw $t0, -8($fp)
	move $a0, $t0
	subu $sp, $sp, 4
	sw $ra, 0($sp)
	jal write
	lw $ra, 0($sp)
	addu $sp, $sp, 4
	lw $t0, -12($fp)
	move $a0, $t0
	subu $sp, $sp, 4
	sw $ra, 0($sp)
	jal write
	lw $ra, 0($sp)
	addu $sp, $sp, 4
	lw $t0, -16($fp)
	move $a0, $t0
	subu $sp, $sp, 4
	sw $ra, 0($sp)
	jal write
	lw $ra, 0($sp)
	addu $sp, $sp, 4
	lw $t0, -20($fp)
	move $a0, $t0
	subu $sp, $sp, 4
	sw $ra, 0($sp)
	jal write
	lw $ra, 0($sp)
	addu $sp, $sp, 4
	lw $t0, -4($fp)
	li $t1, 1
	sub $t2, $t0, $t1
	sw $t2, -364($fp)
	lw $t0, -364($fp)
	sw $t0, -4($fp)
	subu $t0, $fp, 24
	sw $t0, -368($fp)
	li $t0, 0
	li $t1, 1506371712
	sub $t2, $t0, $t1
	sw $t2, -372($fp)
	lw $t0, -372($fp)
	li $t1, 1506371712
	add $t2, $t0, $t1
	sw $t2, -376($fp)
	lw $t0, -376($fp)
	li $t1, 4
	mul $t2, $t0, $t1
	sw $t2, -380($fp)
	lw $t0, -368($fp)
	lw $t1, -380($fp)
	add $t2, $t0, $t1
	sw $t2, -384($fp)
	lw $t0, -384($fp)
	lw $t1, 0($t0)
	sw $t1, -388($fp)
	subu $t0, $fp, 32
	sw $t0, -392($fp)
	li $t0, 0
	li $t1, 655045212
	sub $t2, $t0, $t1
	sw $t2, -396($fp)
	li $t0, 655045212
	lw $t1, -396($fp)
	add $t2, $t0, $t1
	sw $t2, -400($fp)
	lw $t0, -400($fp)
	li $t1, 4
	mul $t2, $t0, $t1
	sw $t2, -404($fp)
	lw $t0, -392($fp)
	lw $t1, -404($fp)
	add $t2, $t0, $t1
	sw $t2, -408($fp)
	lw $t0, -408($fp)
	lw $t1, 0($t0)
	sw $t1, -412($fp)
	lw $t0, -388($fp)
	lw $t1, -412($fp)
	div $t0, $t1
	mflo $t2	
	sw $t2, -416($fp)
	li $t0, 0
	sw $t0, -420($fp)
	subu $t0, $fp, 40
	sw $t0, -424($fp)
	li $t0, 0
	li $t1, 2104687696
	sub $t2, $t0, $t1
	sw $t2, -428($fp)
	li $t0, 2104687696
	lw $t1, -428($fp)
	add $t2, $t0, $t1
	sw $t2, -432($fp)
	lw $t0, -432($fp)
	li $t1, 4
	mul $t2, $t0, $t1
	sw $t2, -436($fp)
	lw $t0, -424($fp)
	lw $t1, -436($fp)
	add $t2, $t0, $t1
	sw $t2, -440($fp)
	lw $t0, -440($fp)
	lw $t1, 0($t0)
	sw $t1, -444($fp)
	lw $t0, -444($fp)
	lw $t1, -8($fp)
	ble $t0, $t1, L126
	j L127
L126:
	li $t0, 1
	sw $t0, -420($fp)
L127:
	lw $t0, -4($fp)
	move $a0, $t0
	lw $t0, -420($fp)
	move $a1, $t0
	lw $t0, -416($fp)
	move $a2, $t0
	subu $sp, $sp, 4
	sw $ra, 0($sp)
	jal i_func_70
	sw $v0, -448($fp)
	lw $ra, 0($sp)
	addu $sp, $sp, 4
	lw $t0, -448($fp)
	move $v0, $t0
	addu $sp, $sp, 448
	lw $fp, 0($sp)
	addu $sp, $sp, 4
	jr $ra

main:
	subu $sp, $sp, 4
	sw $fp, 0($sp)
	move $fp, $sp
	subu $sp, $sp, 4
	li $t0, 10
	move $a0, $t0
	subu $sp, $sp, 4
	sw $ra, 0($sp)
	jal top_caller
	sw $v0, -4($fp)
	lw $ra, 0($sp)
	addu $sp, $sp, 4
	li $t0, 0
	move $v0, $t0
	addu $sp, $sp, 4
	lw $fp, 0($sp)
	addu $sp, $sp, 4
	jr $ra
