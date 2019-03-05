#ifndef _UTILS_H_INCLUDED_
#define _UTILS_H_INCLUDED_

int get_el(void);

unsigned long set_elr_el1(unsigned long p);

unsigned long get_spsr_el1(void);
void set_spsr_el1(unsigned long p);

unsigned long get_esr_el1(void);

#endif 
