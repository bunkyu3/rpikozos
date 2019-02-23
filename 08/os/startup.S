#include "arm/sysregs.h"

	.section .text
	.global _start
_start:
	// read cpu id
	mrs		x1, mpidr_el1
	and		x1, x1, #3
	cbz		x1, 2f

	// if cpu id>0, sleep forever
1:
	wfe
	b	1b

	// if cpu id==0
2:
	// set stack pointor
	ldr		x1, =_bootstack
	mov		sp, x1
	// jump to C code
	bl		main
	// for failsafe
	b		1b
	
	.global dispatch
dispatch:
/*	ldr		x0,	[x0]
	mov		sp,	x0
	ldp x0, x1, [sp, #16 * 0]
 	ldp x2, x3, [sp, #16 * 1]
	ldp x4, x5, [sp, #16 * 2]
 	ldp x6, x7, [sp, #16 * 3]
 	ldp x8, x9, [sp, #16 * 4]
 	ldp x10, x11, [sp, #16 * 5]
 	ldp x12, x13, [sp, #16 * 6]
 	ldp x14, x15, [sp, #16 * 7]
 	ldp x16, x17, [sp, #16 * 8]
 	ldp x18, x19, [sp, #16 * 9]
 	ldp x20, x21, [sp, #16 * 10]
	ldp x22, x23, [sp, #16 * 11]
	ldp x24, x25, [sp, #16 * 12]
 	ldp x26, x27, [sp, #16 * 13]
 	ldp x28, x29, [sp, #16 * 14]
 	ldr x30, [sp, #16 * 15]
 	add sp, sp, 256
*/
	ldr		x0, =SPSR_VALUE
	msr		spsr_el1, x0

	adr		x0, _start
	msr		elr_el1, x0
	eret

