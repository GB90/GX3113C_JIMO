/*
 *   This file has been modified by Evatronix. 
 */
 
#ifndef _LINUX_KERNEL_H
#define _LINUX_KERNEL_H

#ifdef EVIXPKG_USB_DEVICE
#include <pkgconf/usb_device.h>
#endif
//#include <usb/usb.h>
//#ifdef EVIXPKG_USB_DEBUG_ENABLE
//#define DEBUG 0
//#endif

/*
* 'kernel.h' contains some often-used function prototypes etc
*/

#include <stdarg.h>
#include <usb/linkage.h>
#include <usb/stddef.h>
#include <linux/types.h>
#include <usb/compiler.h>
#include <usb/bitops.h>
#include <usb/arch/byteorder.h>

#ifndef INT_MAX
#define INT_MAX         ((int)(~0U>>1))
#define INT_MIN         (-INT_MAX - 1)
#define UINT_MAX        (~0U)
#define LONG_MAX        ((long)(~0UL>>1))
#define LONG_MIN        (-LONG_MAX - 1)
#define ULONG_MAX       (~0UL)
#endif
#define STACK_MAGIC     0xdeadbeef

#ifndef ALIGN
#define ALIGN(x,a)              __ALIGN_MASK(x,(typeof(x))(a)-1)
#define __ALIGN_MASK(x,mask)    (((x)+(mask))&~(mask))
#endif

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

#define FIELD_SIZEOF(t, f) (sizeof(((t*)0)->f))
#define DIV_ROUND_UP(n,d) (((n) + (d) - 1) / (d))

#ifndef roundup
#define roundup(x, y) ((((x) + ((y) - 1)) / (y)) * (y))
#endif

/**
 * upper_32_bits - return bits 32-63 of a number
 * @n: the number we're accessing
 *
 * A basic shift-right of a 64- or 32-bit quantity.  Use this to suppress
 * the "right shift count >= width of type" warning when that quantity is
 * 32-bits.
 */

#define KERN_EMERG      "<0>"   /* system is unusable                   */
#define KERN_ALERT      "<1>"   /* action must be taken immediately     */
#define KERN_CRIT       "<2>"   /* critical conditions                  */
#define KERN_ERR        "<3>"   /* error conditions                     */
#define KERN_WARNING    "<4>"   /* warning conditions                   */
#define KERN_NOTICE     "<5>"   /* normal but significant condition     */
#define KERN_INFO       "<6>"   /* informational                        */
#define KERN_DEBUG      "<7>"   /* debug-level messages                 */


extern int sprintf(char * buf, const char * fmt, ...)
        __attribute__ ((format (printf, 2, 3)));
extern int snprintf(char * buf, size_t size, const char * fmt, ...)
        __attribute__ ((format (printf, 3, 4)));
extern int vsnprintf(char *buf, size_t size, const char *fmt, va_list args)
        __attribute__ ((format (printf, 3, 0)));
extern int scnprintf(char * buf, size_t size, const char * fmt, ...)
        __attribute__ ((format (printf, 3, 4)));
extern int vscnprintf(char *buf, size_t size, const char *fmt, va_list args)
        __attribute__ ((format (printf, 3, 0)));

extern int sscanf(const char *, const char *, ...)
        __attribute__ ((format (scanf, 2, 3)));
extern int vsscanf(const char *, const char *, va_list)
        __attribute__ ((format (scanf, 2, 0)));
/*
#ifdef CONFIG_PRINTK

asmlinkage int vprintk(const char *fmt, va_list args)
        __attribute__ ((format (printf, 1, 0)));
asmlinkage int printk(const char * fmt, ...)
        __attribute__ ((format (printf, 1, 2)));
#else
static inline int vprintk(const char *s, va_list args)
        __attribute__ ((format (printf, 1, 0)));
static inline int vprintk(const char *s, va_list args) { return 0; }
static inline int printk(const char *s, ...)
        __attribute__ ((format (printf, 1, 2)));
static inline int printk(const char *s, ...) { return 0; }
#endif
*/
//#include <linux/kernel.h>
#ifdef DEBUG
/* If you are writing a driver, please use dev_dbg instead */
#define pr_debug(fmt,arg...) \
        diag_printf(fmt,##arg)
#else
static inline int __attribute__ ((format (printf, 1, 2))) pr_debug(const char * fmt, ...)
{
        return 0;
}
#endif

#define pr_info(fmt,arg...) \
        diag_printf(KERN_INFO fmt,##arg)

//#define __LITTLE_ENDIAN 1

/*
 * min()/max() macros that also do
 * strict type-checking.. See the
 * "unnecessary" pointer comparison.
 */
#define min(x,y) ({ \
        typeof(x) _x = (x);     \
        typeof(y) _y = (y);     \
        (void) (&_x == &_y);            \
        _x < _y ? _x : _y; })

#define max(x,y) ({ \
        typeof(x) _x = (x);     \
        typeof(y) _y = (y);     \
        (void) (&_x == &_y);            \
        _x > _y ? _x : _y; })

/**
 * container_of - cast a member of a structure out to the containing structure
 * @ptr:        the pointer to the member.
 * @type:       the type of the container struct this is embedded in.
 * @member:     the name of the member within the struct.
 *
 */
#define container_of(ptr, type, member) ({                      \
        const typeof( ((type *)0)->member ) *__mptr = (ptr);    \
        (type *)( (char *)__mptr - offsetof(type,member) );})


#endif

