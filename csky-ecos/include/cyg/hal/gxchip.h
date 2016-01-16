#ifndef CYGONCE_GXCHIP_H
#define CYGONCE_GXCHIP_H

#include <cyg/infra/cyg_type.h>
#include <cyg/hal/hal_misc.h>
#include <cyg/kernel/kapi.h>

#define GX3201_VA_COUNTER_1 (0xA020A000)
#define GX3201_VA_COUNTER_1_STATUS  (GX3201_VA_COUNTER_1 + 0x00)
#define GX3201_VA_COUNTER_1_VALUE   (GX3201_VA_COUNTER_1 + 0x04)
#define GX3201_VA_COUNTER_1_CONTROL (GX3201_VA_COUNTER_1 + 0x10)
#define GX3201_VA_COUNTER_1_CONFIG  (GX3201_VA_COUNTER_1 + 0x20)
#define GX3201_VA_COUNTER_1_PRE     (GX3201_VA_COUNTER_1 + 0x24)
#define GX3201_VA_COUNTER_1_INI     (GX3201_VA_COUNTER_1 + 0x28)
#define GX3201_COUNTER_1_INT_EN     (1<<1)
#define GX3201_COUNTER_1_EN         (1<<0)

#define GX3201_VA_INTC				(0xA0500000)
#define GX3201_VA_INTC_NINT31_00    (GX3201_VA_INTC + 0x00)
#define GX3201_VA_INTC_NINT63_32    (GX3201_VA_INTC + 0x04)
#define GX3201_VA_INTC_NEN31_00     (GX3201_VA_INTC + 0x20)
#define GX3201_VA_INTC_NEN63_32     (GX3201_VA_INTC + 0x24)
#define GX3201_VA_INTC_NMASK31_00   (GX3201_VA_INTC + 0x50)
#define GX3201_VA_INTC_NMASK63_32   (GX3201_VA_INTC + 0x54)
#define GX3201_VA_INTC_SOURCE       (GX3201_VA_INTC + 0x60)

extern void hal_interrupt_init(cyg_uint32 chip_type);
extern cyg_uint32 gx_chip_probe(void) ;
extern void hal_interrupt_config(cyg_uint32	IntNum, cyg_uint32	IntSrc);

#define NR_BANKS_MAX   (8)

struct gx_membank {
	unsigned long start;
	unsigned long size;
	int node;
};

struct gx_meminfo {
	int nr_banks;
	unsigned long size;
	struct gx_membank bank[NR_BANKS_MAX];
};

struct gx_meminfo_fb {
	unsigned int start;
	unsigned int size;
	unsigned int surface_size;
};

#endif
