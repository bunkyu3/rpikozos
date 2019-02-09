#include "defines.h"
#include "serial.h"
#include "xmodem.h"
#include "elf.h"
#include "lib.h"
#include "utils.h"

static int init(void){
	extern int _bss_start, _ebss;

	memset(&_bss_start, 0, (long)(&_ebss) - (long)(&_bss_start));
	serial_init();

	return 0;
}

static int dump(char *buf, int size){
	int i;

	if(size < 0){
		puts("no data.\n");
		return -1;
	}
	for(i=0; i<size; i++){
		putxval(buf[i], 2);
		if((i & 0xf) == 15){
			puts("\n");
		}else{
			if((i & 0xf) == 7) puts(" ");
			puts(" ");
		}
	}
	puts("\n");

	return 0;
}

static void wait(){
	volatile long i;
	for (i = 0; i < 300000; i++)
		;
}

int main(void){
	static char buf[16];
	static int size = -1;
	static char *loadbuf = NULL;
	char *entry_point;
	void (*f)(void);
	extern int _buffer_start;

	init();
	unsigned int p;
	p = get_el();
	puts("Exception level is ");
	putxval(p,0);   puts("\n");
	puts("kzload (kozos boot loader) started.\n");
	
	while(1){
		puts("kzload> ");
		gets(buf);

		if(!strcmp(buf, "load")){
			loadbuf = (char *)(&_buffer_start);
			size = xmodem_recv(loadbuf);
			wait();
			if(size < 0){
				puts("\nXMODEM receive error!\n");
			}else{
				puts("\nXMODEM receive succeeded.\n");
			}
		}else if(!strcmp(buf, "dump")){
			puts("size: ");
			putxval(size, 0);
			puts("\n");
			dump(loadbuf, size);
		}else if(!strcmp(buf, "run")){
			entry_point = elf_load(loadbuf);
			if(!entry_point){
				puts("run error!\n");
			}else{
				puts("starting from entry point: ");
				putxval((unsigned int)entry_point, 0);
				puts("\n");
				f = (void (*)(void))entry_point;
				f();
			}
		}else{
			puts("unknown.\n");
		}
	}

	return 0;
}
