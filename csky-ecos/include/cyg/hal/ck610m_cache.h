#ifndef CYGONCE_CK610M_CACHE_H
#define CYGONCE_CK610M_CACHE_H

#include <pkgconf/hal.h>
#include <cyg/infra/cyg_type.h>

#include <cyg/hal/ck-regs.h>
#include <cyg/hal/hal_arch.h>
#include <cyg/hal/plf_cache.h>
#include <cyg/hal/ck610m_arch.h>

//-----------------------------------------------------------------------------
// Cache dimensions

// Data cache
#define HAL_DCACHE_SIZE                 8192   // Size of data cache in bytes
#define HAL_DCACHE_LINE_SIZE            16      // Size of a data cache line
#define HAL_DCACHE_WAYS                 2       // Associativity of the cache

// Instruction cache
#define HAL_ICACHE_SIZE                 8192   // Size of cache in bytes
#define HAL_ICACHE_LINE_SIZE            16      // Size of a cache line
#define HAL_ICACHE_WAYS                 2       // Associativity of the cache

#define HAL_DCACHE_CLEAN_DEFINED
#define HAL_DCACHE_CLEAN()                 \
    CYG_MACRO_START                        \
    asm volatile (                         \
        "movi r7, 0x22;"                   \
        "sync;"                            \
        "mtcr r7, cr17;"                   \
        :                                  \
        :                                  \
        : "r7", "memory"                   \
        );                                 \
    CYG_MACRO_END

#define HAL_ICACHE_CLEAN_DEFINED
#define HAL_ICACHE_CLEAN()                 \
    CYG_MACRO_START                        \
    asm volatile (                         \
        "movi r7, 0x21;"                   \
        "sync;"                            \
        "mtcr r7, cr17;"                   \
        :                                  \
        :                                  \
        : "r7", "memory"                   \
        );                                 \
    CYG_MACRO_END

#define HAL_DCACHE_SYNC_DEFINED
#define HAL_DCACHE_SYNC()                  \
    CYG_MACRO_START                        \
    asm volatile (                         \
        "movi r7, 0x32;"                   \
        "sync;"                            \
        "mtcr r7, cr17;"                   \
        :                                  \
        :                                  \
        : "r7", "memory"                   \
        );                                 \
    CYG_MACRO_END

// Synchronize the contents of the cache with memory.
#define HAL_ICACHE_SYNC_DEFINED
#define HAL_ICACHE_SYNC()                  \
    CYG_MACRO_START                        \
    asm volatile (                         \
        "movi r7, 0x31;"                   \
        "sync;"                            \
        "mtcr r7, cr17;"                   \
        :                                  \
        :                                  \
        : "r7", "memory"                   \
    );                                     \
    CYG_MACRO_END

#define HAL_ICACHE_INVALIDATE_DEFINED
#define HAL_ICACHE_INVALIDATE() HAL_ICACHE_SYNC()

#define HAL_DCACHE_INVALIDATE_DEFINED
#define HAL_DCACHE_INVALIDATE() HAL_DCACHE_SYNC()

#define HAL_ICACHE_SYNC_ADDR_DEFINED
#define HAL_ICACHE_SYNC_ADDR(_base_, _size_) HAL_ICACHE_SYNC()

#define HAL_DCACHE_SYNC_ADDR_DEFINED
#define HAL_DCACHE_SYNC_ADDR(_base_, _size_) HAL_DCACHE_SYNC()

#define HAL_ICACHE_INVALIDATE_ADDR_DEFINED
#define HAL_ICACHE_INVALIDATE_ADDR( _base_ , _size_ ) HAL_ICACHE_SYNC()

#define HAL_DCACHE_INVALIDATE_ADDR_DEFINED
#define HAL_DCACHE_INVALIDATE_ADDR( _base_ , _size_ ) HAL_DCACHE_SYNC()

#define HAL_ICACHE_CLEAN_ADDR_DEFINED
#define HAL_ICACHE_CLEAN_ADDR( _base_ , _size_ ) HAL_ICACHE_SYNC()

#define HAL_DCACHE_CLEAN_ADDR_DEFINED
#define HAL_DCACHE_CLEAN_ADDR( _base_ , _size_ ) HAL_DCACHE_SYNC()

#endif // ifndef CYGONCE_CK610M_CACHE_H

