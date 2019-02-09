#include "serial.h"
#include "lib.h"
#include "utils.h"

int main(void){
	serial_init();

	unsigned int p;
	p = get_el(); 
	puts("Exception level is ");
	putxval(p, 0);		puts("\n");

	puts("Hello World!\n");
	putxval(0x10, 0);   puts("\n");
	putxval(0xffff, 0); puts("\n");

	while(1)
		;

	return 0;
}
