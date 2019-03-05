#include "defines.h"
#include "intr.h"
#include "interrupt.h"
#include "peripherals/irq.h"

#include "lib.h"
#include "utils.h"

int softvec_init(void){
	int type;
	for(type=0; type<SOFTVEC_TYPE_NUM; type++)
		softvec_setintr(type, NULL);
	return 0;
}

int softvec_setintr(softvec_type_t type, softvec_handler_t handler){
	SOFTVECS[type] = handler;
	return 0;
}

void interrupt(softvec_type_t type, unsigned long sp)
{
	unsigned long addr;
	addr = get_esr_el1;
	puts("esr in interrupt() "); putxval(addr, 0); puts("\n"); 

	softvec_handler_t handler = SOFTVECS[type];
	if(handler)
		handler(type, sp);
}



void enable_interrupt_controller(){
	unsigned int r;

	r = *ENABLE_IRQS_1;
	r |= MINI_UART_IRQ;
	*ENABLE_IRQS_1 = r; 
}


