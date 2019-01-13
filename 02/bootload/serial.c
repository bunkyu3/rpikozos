#include "gpio.h"
#include "serial.h"

/* Auxilary mini UART registers */
#define AUX_ENABLE      ((volatile unsigned int*)(MMIO_BASE+0x00215004))
#define AUX_MU_IO       ((volatile unsigned int*)(MMIO_BASE+0x00215040))
#define AUX_MU_IER      ((volatile unsigned int*)(MMIO_BASE+0x00215044))
#define AUX_MU_IIR      ((volatile unsigned int*)(MMIO_BASE+0x00215048))
#define AUX_MU_LCR      ((volatile unsigned int*)(MMIO_BASE+0x0021504C))
#define AUX_MU_MCR      ((volatile unsigned int*)(MMIO_BASE+0x00215050))
#define AUX_MU_LSR      ((volatile unsigned int*)(MMIO_BASE+0x00215054))
#define AUX_MU_MSR      ((volatile unsigned int*)(MMIO_BASE+0x00215058))
#define AUX_MU_SCRATCH  ((volatile unsigned int*)(MMIO_BASE+0x0021505C))
#define AUX_MU_CNTL     ((volatile unsigned int*)(MMIO_BASE+0x00215060))
#define AUX_MU_STAT     ((volatile unsigned int*)(MMIO_BASE+0x00215064))
#define AUX_MU_BAUD     ((volatile unsigned int*)(MMIO_BASE+0x00215068))

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
