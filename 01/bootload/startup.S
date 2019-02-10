#include "arm/sysregs.h"

.section .text
.global _start

_start:
	// read cpu id
	mrs		x1, mpidr_el1
	and		x1, x1, #3
	cbz		x1, master

	// if cpu id>0, sleep forever
1:
	wfe
	b	1b

	// if cpu id==0
master:
	// switch EL2 to EL1
	ldr		x0, =HCR_VALUE
	msr		hcr_el2, x0

	ldr		x0, =SPSR_VALUE
	msr		spsr_el2, x0

	adr		x0, el1_entry
	msr		elr_el2, x0

	eret

el1_entry:
	// set stack pointor
	ldr		x1, =_start
	mov		sp, x1
	// jump to C code
	bl		main
	// for failsafe
	b		1b
	
