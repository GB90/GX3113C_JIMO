#ifndef __LINUX_SPINLOCK_H
#define __LINUX_SPINLOCK_H

#include <usb/compiler.h>
#include <usb/kernel.h>
#include <usb/arch/system.h>
#include <usb/spinlock_types.h>

extern void cyg_scheduler_lock(void);
extern void cyg_scheduler_unlock(void);
extern void cyg_interrupt_enable(void);
extern void cyg_interrupt_disable(void);
extern int cyg_interrupt_query(void);

#define spin_lock_init(lock) do { *(lock) = SPIN_LOCK_UNLOCKED; } while (0)

#define spin_lock(lock) \
	unsigned int tmp_flag; \
	HAL_DISABLE_INTERRUPTS(tmp_flag);

#define spin_lock_irqsave(lock, flag) HAL_DISABLE_INTERRUPTS(flag);

#define spin_lock_irq(lock) \
	unsigned int tmp_flag; \
	HAL_DISABLE_INTERRUPTS(tmp_flag); 

#define spin_unlock(lock) HAL_RESTORE_INTERRUPTS(tmp_flag);
#define spin_unlock_irqrestore(lock, flag) HAL_RESTORE_INTERRUPTS(flag); 
#define spin_unlock_irq(lock) HAL_RESTORE_INTERRUPTS(tmp_flag);

#endif /* __LINUX_SPINLOCK_H */

