#include "serial.h"
#include "lib.h"
#include "irq.h"
#include "intr.h"

static int init(void){
	extern int _bss_start, _ebss;

	memset(&_bss_start, 0, (long)(&_ebss) - (long)(&_bss_start));
	serial_init();

	return 0;
}

int main(void){
	init();
	irq_vector_init();
	enable_interrupt_controller();
	enable_irq();
	
	volatile long i;
	while(1){
		for(i=0; i<3000000; i++)
			;
		putc('a');
	}

	return 0;
}
