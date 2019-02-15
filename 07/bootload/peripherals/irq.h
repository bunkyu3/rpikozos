#ifndef	_P_IRQ_H
#define	_P_IRQ_H

#include "peripherals/base.h"

#define IRQ_PENDING_1		((volatile unsigned int*)(MMIO_BASE+0x0000B204))
#define ENABLE_IRQS_1		((volatile unsigned int*)(MMIO_BASE+0x0000B210))
#define DISABLE_IRQS_1		((volatile unsigned int*)(MMIO_BASE+0x0000B21C))
#define DISABLE_IRQS_2		((volatile unsigned int*)(MMIO_BASE+0x0000B220))
#define DISABLE_BASIC_IRQS	((volatile unsigned int*)(MMIO_BASE+0x0000B224))

#define MINI_UART_IRQ		(1 << 29)

#endif  /*_P_IRQ_H */
