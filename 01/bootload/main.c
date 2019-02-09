#include "serial.h"
#include "lib.h"
#include "utils.h"

int main(void){
	serial_init();

	char p;
	p = (char)get_el();
	puts("Exception level is ");
	putc(p+0x30); puts("\n");
	puts("Hello World!\n");

	while(1)
		;

	return 0;
}
