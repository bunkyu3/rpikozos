#include "intr.h"
#include "interrupt.h"
#include "serial.h"
#include "lib.h"

extern void enable_irq(void);
extern void disable_irq(void);

static void intr(softvec_type_t type, unsigned long sp){
	int c;
	static char buf[32];
	static int len;

	c = getc();

	if(c != '\n'){
		buf[len++] = c;
	}else{
		buf[len++] = '\0';
		if(!strncmp(buf, "echo", 4)){
			puts(buf + 4);
			puts("\n");
		}else{
			puts("unknown.\n");
		}
		puts("> ");
		len = 0;
	}
}

int main(void){
	INTR_DISABLE;

	puts("kozos boot succeed!\n");

	softvec_setintr(SOFTVEC_TYPE_SERINTR, intr);
	serial_intr_recv_enable();

	enable_interrupt_controller();

	puts("> ");

	INTR_ENABLE;
	volatile long i;
	while(1){
		for(i=0; i<3000000; i++)
				;
		putc('a');	
	}

	return 0;
}
