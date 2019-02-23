.global	get_el
get_el:
	mrs		x0, CurrentEL
	lsr		x0, x0, #2
	ret

.global set_elr_el1
set_elr_el1:
	msr		elr_el1, x0
