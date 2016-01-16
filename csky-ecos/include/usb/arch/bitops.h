/*
 *   This file has been modified by Evatronix. 
 */
 
#ifndef _ARM_BITOPS_H
#define _ARM_BITOPS_H
/*
 * Copyright 1992, Linus Torvalds.
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file COPYING in the main directory of this archive
 * for more details.
 */
#include <cyg/hal/hal_intr.h>
#include <usb/compiler.h>
#include <linux/types.h>
#include <usb/arch/byteorder.h>
#define BITOP_WORD(nr)          ((nr) / BITS_PER_LONG)

#define local_irq_save(flag) HAL_DISABLE_INTERRUPTS(flag)

#define local_irq_restore(flag) HAL_RESTORE_INTERRUPTS(flag)

void *kzalloc(size_t size);

#define test_and_set_bit(nr,vaddr) __constant_test_and_set_bit(nr, vaddr) 

#define __test_and_set_bit(nr,vaddr) test_and_set_bit(nr,vaddr)
static inline int __constant_test_and_set_bit(int nr, unsigned long *vaddr)
{
	unsigned long mask = 1UL << (nr & 31);
	unsigned int irq_save = 0;

	vaddr += nr >> 5;

	local_irq_save(irq_save);
	*vaddr |= mask;
	local_irq_restore(irq_save);
	return 0;
}


#define set_bit(nr,vaddr) \
   __constant_set_bit(nr, vaddr)

#define __set_bit(nr,vaddr) set_bit(nr,vaddr)

static inline void __constant_set_bit(int nr, volatile unsigned long *vaddr)
{
	unsigned long mask = 1UL << (nr & 31);
	unsigned int irq_save = 0;

	vaddr += nr >> 5;

	local_irq_save(irq_save);
	*vaddr |= mask;
	local_irq_restore(irq_save);
}


#define test_and_clear_bit(nr,vaddr) \
   __constant_test_and_clear_bit(nr, vaddr) 

#define __test_and_clear_bit(nr,vaddr) test_and_clear_bit(nr,vaddr)

static inline int __constant_test_and_clear_bit(int nr, unsigned long *vaddr)
{
	unsigned int res;
	unsigned long mask = 1UL << (nr & 31);
	unsigned int irq_save = 0;

	vaddr += nr >> 5;

	local_irq_save(irq_save);
	res = *vaddr;
	*vaddr = res & ~mask;
	local_irq_restore(irq_save);

	return res & mask;
}


/*
 * clear_bit() doesn't provide any barrier for the compiler.
 */
#define smp_mb__before_clear_bit()      barrier()
#define smp_mb__after_clear_bit()       barrier()

#define clear_bit(nr,vaddr) \
   __constant_clear_bit(nr, vaddr) 
   
#define __clear_bit(nr,vaddr) clear_bit(nr,vaddr)

static inline void __constant_clear_bit(int nr, volatile unsigned long *vaddr)
{
	unsigned long mask = 1UL << (nr & 31);
	unsigned int irq_save = 0;

	vaddr += nr >> 5;

	local_irq_save(irq_save);
	*vaddr &= ~mask;
	local_irq_restore(irq_save);
}


#define test_and_change_bit(nr,vaddr) \
   __constant_test_and_change_bit(nr, vaddr) 

#define __test_and_change_bit(nr,vaddr) test_and_change_bit(nr,vaddr)
#define __change_bit(nr,vaddr) change_bit(nr,vaddr)

static inline int __constant_test_and_change_bit(int nr, unsigned long *vaddr)
{
	unsigned int res;
	unsigned long mask = 1UL << (nr & 31);
	unsigned int irq_save = 0;

	vaddr += nr >> 5;

	local_irq_save(irq_save);
	res = *vaddr;
	*vaddr = res ^ mask;
	local_irq_restore(irq_save);

	return (res & mask);
}


#define change_bit(nr,vaddr) \
   __constant_change_bit(nr, vaddr) 
 

static inline void __constant_change_bit(int nr, unsigned long *vaddr)
{
	unsigned long mask = 1UL << (nr & 31);
	unsigned int irq_save = 0;

	vaddr += nr >> 5;

	local_irq_save(irq_save);
	*vaddr ^= mask;
	local_irq_restore(irq_save);
}

static inline int test_bit(int nr, const unsigned long *vaddr)
{
        return (vaddr[nr >> 5] & (1UL << (nr & 31))) != 0;
}

/**
 * ffs - find first bit set
 * @x: the word to search
 *
 * This is defined the same way as
 * the libc and compiler builtin ffs routines, therefore
 * differs in spirit from the above ffz (man ffs).
 */
static inline int ffs(int x)
{
        int r = 1;

        if (!x)
                return 0;
        if (!(x & 0xffff)) {
                x >>= 16;
                r += 16;
        }
        if (!(x & 0xff)) {
                x >>= 8;
                r += 8;
        }
        if (!(x & 0xf)) {
                x >>= 4;
                r += 4;
        }
        if (!(x & 3)) {
                x >>= 2;
                r += 2;
        }
        if (!(x & 1)) {
                x >>= 1;
                r += 1;
        }
        return r;
}

/**
 * __ffs - find first bit in word.
 * @word: The word to search
 *
 * Undefined if no bit exists, so code should check against 0 first.
 */
static inline unsigned long __ffs(unsigned long word)
{
        int num = 0;

        if ((word & 0xffff) == 0) {
                num += 16;
                word >>= 16;
        }
        if ((word & 0xff) == 0) {
                num += 8;
                word >>= 8;
        }
        if ((word & 0xf) == 0) {
                num += 4;
                word >>= 4;
        }
        if ((word & 0x3) == 0) {
                num += 2;
                word >>= 2;
        }
        if ((word & 0x1) == 0)
                num += 1;
        return num;
}

/*
 * ffz - find first zero in word.
 * @word: The word to search
 *
 * Undefined if no zero exists, so code should check against ~0UL first.
 */
#define ffz(x)  __ffs(~(x))
static inline int find_next_zero_bit(const unsigned long *addr, int size,
                                     int offset)
{
         const unsigned long *p = addr + BITOP_WORD(offset);
         unsigned long result = offset & ~(BITS_PER_LONG-1);
         unsigned long tmp;
 
         if (offset >= size)
                 return size;
         size -= result;
         offset %= BITS_PER_LONG;
         if (offset) {
                 tmp = *(p++);
                 tmp |= ~0UL >> (BITS_PER_LONG - offset);
                 if (size < BITS_PER_LONG)
                         goto found_first;
                 if (~tmp)
                         goto found_middle;
                 size -= BITS_PER_LONG;
                 result += BITS_PER_LONG;
         }
         while (size & ~(BITS_PER_LONG-1)) {
                 if (~(tmp = *(p++)))
                         goto found_middle;
                 result += BITS_PER_LONG;
                 size -= BITS_PER_LONG;
         }
         if (!size)
                 return result;
         tmp = *p;
 
 found_first:
         tmp |= ~0UL << size;
         if (tmp == ~0UL)        /* Are any bits zero? */
                 return result + size;   /* Nope. */
 found_middle:
         return result + ffz(tmp);
}

#endif /* _arm_BITOPS_H */

