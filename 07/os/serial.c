#include "peripherals/gpio.h"
#include "peripherals/serial.h"
#include "serial.h"

int serial_init(void){
	unsigned int r;

	/* gpio pins function set up */
	r = *GPFSEL1;
	r &= ~(7<<12);	// clean gpio14
	r |= 2<<12;		// set alt5 for gpio14 
	r &= ~(7<<15);	// clean gpio15
	r |= 2<<15;		// set alt5 for gpio15 
	*GPFSEL1 = r;

	/* gpio pins pull up/donw set up */
	*GPPUD = 0;						// disable pull up/down for all pins
	r = 150; while(r--) { asm volatile("nop"); }
	*GPPUDCLK0 = (1<<14)|(1<<15);	// choose gpio 14, 15
	r = 150; while(r--) { asm volatile("nop"); }
	*GPPUDCLK0 = 0;					// flush GPIO setup

	/* initialize UART */
	*AUX_ENABLE |=1;				// enable UART1
	*AUX_MU_CNTL = 0;				// disable Tx, Rx
	*AUX_MU_IER = 0;				// disable Tx, Rx interrupt
	*AUX_MU_LCR = 3;				// 8bits mode
	*AUX_MU_IIR = 0xc6;				// clear receive, transmit FIFO
	*AUX_MU_BAUD = 270;				// 115200 baud
	*AUX_MU_CNTL = 3;				// enable Tx, Rx

	return 0;
}

int serial_is_send_enable(){
	return (*AUX_MU_LSR & (1<<6));
}

int serial_send_byte(char c){
	while(!serial_is_send_enable())
		;

	*AUX_MU_IO = c;

	return 0;
}

int serial_is_recv_enable(){
	return (*AUX_MU_LSR & (1<<0));
}

char serial_recv_byte(){
	char c;

	while(!serial_is_recv_enable())
		;
	c = (char)(*AUX_MU_IO);

	return c;
}
