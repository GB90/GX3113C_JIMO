/*
 *   This file has been modified by Evatronix.
 */
#ifndef _ARM_SYSTEM_H
#define _ARM_SYSTEM_H

#include <cyg/hal/hal_cache.h>
#include <linux/types.h>
#include <usb/kernel.h>
#include <usb/arch/bitops.h>

#define DCache_Sync() HAL_DCACHE_SYNC()
#endif /* _ARM_SYSTEM_H */

