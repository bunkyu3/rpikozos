#ifndef _DEFINES_H_INCLUDED_
#define _DEFINES_H_INCLUDED_

#define NULL ((void *)0)

typedef unsigned char	uint8;
typedef unsigned short	uint16;
typedef unsigned long	uint64;

typedef uint64 kz_thread_id_t;
typedef int (*kz_func_t)(int argc, char *argv[]);
typedef void (*kz_handler_t)(void);

#endif
