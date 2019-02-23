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
	ldr		x0,	[x0]
	mov		sp,	x0
	ldp		x0,	x1,	[sp, #16 * 0]

	ldr		x1, =SPSR_VALUE
	msr		spsr_el1, x1
	eret	

