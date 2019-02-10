#ifndef _SYSREGS_H
#define _SYSREGS_H

// HCR_EL2
#define HCR_RW			(1 << 31)
#define HCR_VALUE		HCR_RW

// SPSR_EL3
#define SPSR_EL1t		(4 << 0)
#define SPSR_VALUE		SPSR_EL1t

#endif
