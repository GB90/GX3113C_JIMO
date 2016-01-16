/*
 *   This file has been modified by Evatronix. 
 */
/* interrupt.h */
#ifndef _LINUX_INTERRUPT_H
#define _LINUX_INTERRUPT_H

#include <usb/arch/system.h>
#include <linux/types.h>
/*
 * IRQ_NONE means we didn't handle it.
 * IRQ_HANDLED means that we did have a valid interrupt and handled it.
 */
typedef s32 irqreturn_t;

#define IRQ_NONE	(0)
#define IRQ_HANDLED	(1)

/*s32 request_irq(unsigned int irq,
		irqreturn_t (*handler_isr) (s32, void *, struct pt_regs *),
		irqreturn_t (*handler_dsr) (s32, void *, struct pt_regs *), 
		u32 flags, const u8 *devname, void *dev_id);
extern void free_irq(unsigned int, void *);
*/
typedef irqreturn_t (*irq_handler_t)(int, void *);
extern int request_irq(unsigned int irq, irq_handler_t handler_isr, irq_handler_t handler_dsr, unsigned long flags,  char *devname, void *dev_id);
#define free_irq(irq, dev_id)

#endif
