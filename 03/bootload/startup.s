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
	ldr		x1, =_start
	mov		sp, x1
	// jump to C code
	bl		main
	// for failsafe
	b		1b
	
