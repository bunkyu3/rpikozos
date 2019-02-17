#ifndef	_INTERRUPT_H_INCLUDED_
#define	_INTERRUPT_H_INCLUDED_

extern char _softvec;
#define SOFTVEC_ADDR (&_softvec)

typedef short softvec_type_t;

typedef void (*softvec_handler_t)(softvec_type_t type, unsigned long sp);

#define SOFTVECS ((softvec_handler_t *)SOFTVEC_ADDR)

int softvec_init(void);

int softvec_setintr(softvec_type_t type, softvec_handler_t handler);

void interrupt(softvec_type_t type, unsigned long sp);


void enable_interrupt_controller(void);

#endif
