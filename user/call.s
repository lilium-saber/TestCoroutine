	.file	"call.c"
	.option pic
	.text
	.align	1
	.globl	g
	.type	g, @function
g:
	addi	sp,sp,-32
	sd	s0,24(sp)
	addi	s0,sp,32
	mv	a5,a0
	sw	a5,-20(s0)
	lw	a5,-20(s0)
	addiw	a5,a5,3
	sext.w	a5,a5
	mv	a0,a5
	ld	s0,24(sp)
	addi	sp,sp,32
	jr	ra
	.size	g, .-g
	.align	1
	.globl	f
	.type	f, @function
f:
	addi	sp,sp,-32
	sd	ra,24(sp)
	sd	s0,16(sp)
	addi	s0,sp,32
	mv	a5,a0
	sw	a5,-20(s0)
	lw	a5,-20(s0)
	mv	a0,a5
	call	g
	mv	a5,a0
	mv	a0,a5
	ld	ra,24(sp)
	ld	s0,16(sp)
	addi	sp,sp,32
	jr	ra
	.size	f, .-f
	.section	.rodata
	.align	3
.LC0:
	.string	"%d %d\n"
	.text
	.align	1
	.globl	main
	.type	main, @function
main:
	addi	sp,sp,-16
	sd	ra,8(sp)
	sd	s0,0(sp)
	addi	s0,sp,16
	li	a0,8
	call	f
	mv	a5,a0
	addiw	a5,a5,1
	sext.w	a5,a5
	li	a2,13
	mv	a1,a5
	lla	a0,.LC0
	call	printf@plt
	li	a0,0
	call	exit@plt
	.size	main, .-main
	.ident	"GCC: (Ubuntu 9.4.0-1ubuntu1~20.04) 9.4.0"
	.section	.note.GNU-stack,"",@progbits
