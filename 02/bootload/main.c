#include "serial.h"
#include "lib.h"

int main(void){
	serial_init();

	puts("Hello World!\n");
	putxval(0x10, 0);   puts("\n");
	putxval(0xffff, 0); puts("\n");

	while(1)
		;

	return 0;
}
