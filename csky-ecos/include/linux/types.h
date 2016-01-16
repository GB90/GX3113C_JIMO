#ifndef __LINUX_TYPES_H__
#define __LINUX_TYPES_H__

#include "cyg/infra/cyg_type.h"

#define uint8_t cyg_uint8
#define uint16_t cyg_uint16
#define uint32_t cyg_uint32

#define int8_t cyg_int8
#define int16_t cyg_int16
#define int32_t cyg_int32

#define loff_t off_t

#ifndef __ASSEMBLY__
/*
 * __xx is ok: it doesn't pollute the POSIX namespace. Use these in the
 * header files exported to user space
 */

typedef signed char __s8;
typedef unsigned char __u8;

typedef signed short __s16;
typedef unsigned short __u16;

typedef signed int __s32;
typedef unsigned int __u32;

typedef signed long long __s64;
typedef unsigned long long __u64;

typedef signed char s8;
typedef unsigned char u8;

typedef signed short s16;
typedef unsigned short u16;

typedef signed int s32;
typedef unsigned int u32;

typedef signed long long s64;
typedef unsigned long long u64;

typedef __u16  __le16;
typedef __u16  __be16;
typedef __u32  __le32;
typedef __u32  __be32;
typedef __u64  __le64;
typedef __u64  __be64;

typedef u32 resource_size_t;
typedef u32 dma_addr_t;

#endif /*  __ASSEMBLY__ */

#define BITS_PER_LONG 32
#define __stringify_1(x)        #x
#define __stringify(x)          __stringify_1(x)

#define put_unaligned(val, ptr) ((void)( *(ptr) = (val) ))

#define kvec iovec
#endif /* __LINUX_TYPES_H__ */

