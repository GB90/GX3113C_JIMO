#ifndef CYGONCE_HAL_GXINTERRUPT_H
#define CYGONCE_HAL_GXINTERRUPT_H

#include <cyg/infra/cyg_type.h>
#include <pkgconf/hal.h>

extern unsigned short select_interrupt_num[64];
extern unsigned short select_interrupt_source[64];

extern int hal_IRQ_handler(void);
extern int hal_IRQ_source(int num);
extern void hal_interrupt_mask(int IntSrc);
extern void hal_interrupt_unmask(int IntSrc);
extern void hal_interrupt_acknowledge(int IntSrc);
extern int hal_interrupt_SetNormalIntSource(cyg_uint32	IntNum, cyg_uint32	IntSrc);
extern int hal_interrupt_NormalIntEnable(cyg_uint32	IntSrc);
extern int hal_interrupt_SetFastIntSource(cyg_uint32	IntNum, cyg_uint32	IntSrc);
extern int hal_interrupt_FastIntEnable(cyg_uint32	IntNum);
extern void hal_interrupt_set_level(int vector, int level);
#endif

