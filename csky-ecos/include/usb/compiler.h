/*
 *   This file has been modified by Evatronix. 
 */
#ifndef __LINUX_COMPILER_H
#define __LINUX_COMPILER_H

# define __user
# define __kernel
# define __safe
# define __force
# define __nocast
# define __iomem
# define __chk_user_ptr(x) (void)0
# define __chk_io_ptr(x) (void)0
# define __builtin_warning(x, y...) (1)
# define __acquires(x)
# define __releases(x)
# define __acquire(x) (void)0
# define __release(x) (void)0
# define __cond_lock(x,c) (c)

/*
 * Common definitions for all gcc versions go here.
 */
/* Optimization barrier */
/* The "volatile" is due to gcc bugs */
#define barrier() __asm__ __volatile__("": : :"memory")

#define inline          inline          __attribute__((always_inline))
#define __inline__      __inline__      __attribute__((always_inline))
#define __inline        __inline        __attribute__((always_inline))
#define __deprecated                    __attribute__((deprecated))
#define __packed                        __attribute__((packed))
#define __weak                          __attribute__((weak))
#define __naked                         __attribute__((naked))
#define __noreturn                      __attribute__((noreturn))
#define __pure                          __attribute__((pure))
#define __aligned(x)                    __attribute__((aligned(x)))
#define __printf(a,b)                   __attribute__((format(printf,a,b)))
#define  noinline                       __attribute__((noinline))
#define __attribute_pure__              __attribute__((pure))
#define __attribute_const__             __attribute__((__const__))
#define __maybe_unused                  __attribute__((unused))

/*
 * Generic compiler-dependent macros required for kernel
 * build go below this comment. Actual compiler/compiler version
 * specific implementations come from the above header files
 */
#define likely(x)       __builtin_expect(!!(x), 1)
#define unlikely(x)     __builtin_expect(!!(x), 0)

#endif /* __LINUX_COMPILER_H */

