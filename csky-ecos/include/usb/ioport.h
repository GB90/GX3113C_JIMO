/*
 *   This file has been modified by Evatronix. 
 */

/*
 * ioport.h     Definitions of routines for detecting, reserving and
 *              allocating system resources.
 *
 * Authors:     Linus Torvalds
 */

#ifndef _LINUX_IOPORT_H
#define _LINUX_IOPORT_H

#include <usb/compiler.h>
#include <linux/types.h>
/*
 * Resources are tree-like, allowing
 * nesting etc..
 */
struct resource {
        resource_size_t start;
        resource_size_t end;
        const char *name;
        unsigned long flags;
};

/*
 * IO resources have these defined flags.
 */
#define IORESOURCE_BITS         0x000000ff      /* Bus-specific bits */

#define IORESOURCE_IO           0x00000100      /* Resource type */
#define IORESOURCE_MEM          0x00000200
#define IORESOURCE_IRQ          0x00000400
#define IORESOURCE_DMA          0x00000800

#endif  /* _LINUX_IOPORT_H */


