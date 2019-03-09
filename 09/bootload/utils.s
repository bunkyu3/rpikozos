.global	get_el
get_el:
	mrs		x0, CurrentEL
	lsr		x0, x0, #2
	ret

.global set_elr_el1
set_elr_el1:
	msr		elr_el1, x0
	mrs		x0, elr_el1
	ret

.global get_spsr_el1
get_spsr_el1:
	mrs		x0, spsr_el1
	ret

.global set_spsr_el1
set_spsr_el1:
	msr		spsr_el1, x0
	ret

.global get_esr_el1
get_esr_el1:
	mrs		x0, esr_el1
	ret
