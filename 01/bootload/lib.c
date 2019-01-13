#include "serial.h"
#include "lib.h"

int putc(char c){
	if(c == '\n')
		serial_send_byte('\r');
	return serial_send_byte(c);
}

int puts(char *str){
	while (*str)
		putc(*(str++));
	return 0;
}
