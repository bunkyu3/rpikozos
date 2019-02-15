#include "peripherals/irq.h"
#include "lib.h"
#include "serial.h"

void enable_interrupt_controller(){
	unsigned int r;

	r = *ENABLE_IRQS_1;
	r |= MINI_UART_IRQ;
	*ENABLE_IRQS_1 = r; 
}


void handle_irq(void)
{
	unsigned int irq = *IRQ_PENDING_1;
	switch (irq) {
		case (MINI_UART_IRQ):
			handle_serial();
			break;
		default:
			puts("Unknown pending irq:");
	}
}
