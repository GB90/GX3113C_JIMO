/*
 *   This file has been modified by Evatronix. 
 */
 
#ifndef _LINUX_BYTEORDER_BIG_ENDIAN_H
#define _LINUX_BYTEORDER_BIG_ENDIAN_H

#ifndef __BIG_ENDIAN
#define __BIG_ENDIAN 4321
#endif
#ifndef __BIG_ENDIAN_BITFIELD
#define __BIG_ENDIAN_BITFIELD
#endif

#include <linux/types.h>
#include <usb/byteorder/swab.h>

#define __constant_htonl(x) ((__be32)(__u32)(x))
#define __constant_ntohl(x) ((__u32)(__be32)(x))
#define __constant_htons(x) ((__be16)(__u16)(x))
#define __constant_ntohs(x) ((__u16)(__be16)(x))
#define __constant_cpu_to_le32(x) ((__le32)___constant_swab32((x)))
#define __constant_le32_to_cpu(x) ___constant_swab32((__u32)(__le32)(x))
#define __constant_cpu_to_le16(x) ((__le16)___constant_swab16((x)))
#define __constant_le16_to_cpu(x) ___constant_swab16(((__le16)(x))
#define __constant_cpu_to_be32(x) ((__be32)(__u32)(x))
#define __constant_be32_to_cpu(x) ((__u32)(__be32)(x))
#define __constant_cpu_to_be16(x) ((__be16)(__u16)(x))
#define __constant_be16_to_cpu(x) ((__u16)(__be16)(x))
#define __cpu_to_le32(x) ((__le32)__swab32((x)))
#define __le32_to_cpu(x) __swab32((__u32)(__le32)(x))
#define __cpu_to_le16(x) ((__le16)__swab16((x)))
#define __le16_to_cpu(x) __swab16((__u16)(__le16)(x))
#define __cpu_to_be32(x) ((__be32)(__u32)(x))
#define __be32_to_cpu(x) ((__u32)(__be32)(x))
#define __cpu_to_be16(x) ((__be16)(__u16)(x))
#define __be16_to_cpu(x) ((__u16)(__be16)(x))

static inline __le32 __cpu_to_le32p(const __u32 *p)
{
	return (__le32)__swab32p(p);
}
static inline __u32 __le32_to_cpup(const __le32 *p)
{
	return __swab32p((__u32 *)p);
}
static inline __le16 __cpu_to_le16p(const __u16 *p)
{
	return (__le16)__swab16p(p);
}
static inline __u16 __le16_to_cpup(const __le16 *p)
{
	return __swab16p((__u16 *)p);
}
static inline __be32 __cpu_to_be32p(const __u32 *p)
{
	return (__be32)*p;
}
static inline __u32 __be32_to_cpup(const __be32 *p)
{
	return (__u32)*p;
}
static inline __be16 __cpu_to_be16p(const __u16 *p)
{
	return (__be16)*p;
}
static inline __u16 __be16_to_cpup(const __be16 *p)
{
	return (__u16)*p;
}

#define __cpu_to_le32s(x) __swab32s((x))
#define __le32_to_cpus(x) __swab32s((x))
#define __cpu_to_le16s(x) __swab16s((x))
#define __le16_to_cpus(x) __swab16s((x))
#define __cpu_to_be32s(x) do {} while (0)
#define __be32_to_cpus(x) do {} while (0)
#define __cpu_to_be16s(x) do {} while (0)
#define __be16_to_cpus(x) do {} while (0)

#include <usb/byteorder/generic.h>
#endif /* _LINUX_BYTEORDER_BIG_ENDIAN_H */
