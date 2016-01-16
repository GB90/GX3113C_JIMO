#ifndef __CRAMFS_H
#define __CRAMFS_H

#include <linux/types.h>

//#include "../config.h"
#ifndef __LITTLE_ENDIAN
#define __LITTLE_ENDIAN 1234
#endif
#ifndef __BIG_ENDIAN
#define __BIG_ENDIAN 4321
#endif

#define CONFIG_ARCH_ARM

#if defined(CONFIG_ARCH_ARM)
#define	__BYTE_ORDER	__LITTLE_ENDIAN
#elif defined(CONFIG_ARCH_CK)
#define	__BYTE_ORDER	__BIG_ENDIAN
//#include "../swab.h"
//#include "../types.h"
#endif


#define CS_IFMT  00170000	/* type of file */
#define CS_IFSOCK 0140000	/* named socket */
#define CS_IFLNK  0120000	/* symbolic link */
#define CS_IFREG  0100000	/* regular */
#define CS_IFBLK  0060000	/* block special */
#define CS_IFDIR  0040000	/* directory */
#define CS_IFCHR  0020000	/* character special */
#define CS_IFIFO  0010000	/* fifo */
#define CS_ISUID  0004000	/* set user id on execution */
#define CS_ISGID  0002000	/* set group id on execution */
#define CS_ISVTX  0001000	/* save swapped text even after use */

#define CS_ISLNK(m)      (((m) & CS_IFMT) == CS_IFLNK)
#define CS_ISREG(m)      (((m) & CS_IFMT) == CS_IFREG)
#define CS_ISDIR(m)      (((m) & CS_IFMT) == CS_IFDIR)
#define CS_ISCHR(m)      (((m) & CS_IFMT) == CS_IFCHR)
#define CS_ISBLK(m)      (((m) & CS_IFMT) == CS_IFBLK)
#define CS_ISFIFO(m)     (((m) & CS_IFMT) == CS_IFIFO)
#define CS_ISSOCK(m)     (((m) & CS_IFMT) == CS_IFSOCK)

#define CRAMFS_MAGIC		0x28cd3d45	/* some random number */
#define CRAMFS_MAGIC_WEND	0x453dcd28	/* magic number with the wrong endianess */
#define CRAMFS_SIGNATURE	"Compressed ROMFS"

/*
 * Width of various bitfields in struct cramfs_inode.
 * Primarily used to generate warnings in mkcramfs.
 */
#define CRAMFS_MODE_WIDTH 16
#define CRAMFS_UID_WIDTH 16
#define CRAMFS_SIZE_WIDTH 24
#define CRAMFS_GID_WIDTH 8
#define CRAMFS_NAMELEN_WIDTH 6
#define CRAMFS_OFFSET_WIDTH 26

/*
 * Since inode.namelen is a unsigned 6-bit number, the maximum cramfs
 * path length is 63 << 2 = 252.
 */
#define CRAMFS_MAXPATHLEN (((1 << CRAMFS_NAMELEN_WIDTH) - 1) << 2)

/*
 * Reasonably terse representation of the inode data.
 */
typedef struct cramfs_inode {
	__u32 mode:CRAMFS_MODE_WIDTH, uid:CRAMFS_UID_WIDTH;
	/* SIZE for device files is i_rdev */
	__u32 size:CRAMFS_SIZE_WIDTH, gid:CRAMFS_GID_WIDTH;
	/* NAMELEN is the length of the file name, divided by 4 and
           rounded up.  (cramfs doesn't support hard links.) */
	/* OFFSET: For symlinks and non-empty regular files, this
	   contains the offset (divided by 4) of the file data in
	   compressed form (starting with an array of block pointers;
	   see README).  For non-empty directories it is the offset
	   (divided by 4) of the inode of the first file in that
	   directory.  For anything else, offset is zero. */
	__u32 namelen:CRAMFS_NAMELEN_WIDTH, offset:CRAMFS_OFFSET_WIDTH;
}cramfs_inode;

typedef struct cramfs_info {
	__u32 crc;
	__u32 edition;
	__u32 blocks;
	__u32 files;
}cramfs_info;

/*
 * Superblock information at the beginning of the FS.
 */
typedef struct cramfs_super {
	__u32 magic;			/* 0x28cd3d45 - random number */
	__u32 size;			/* length in bytes */
	__u32 flags;			/* feature flags */
	__u32 future;			/* reserved for future use */
	__u8 signature[16];		/* "Compressed ROMFS" */
	struct cramfs_info fsid;	/* unique filesystem info */
	__u8 name[16];			/* user-defined name */
	struct cramfs_inode root;	/* root inode data */
}cramfs_super;

/*
 * Feature flags
 *
 * 0x00000000 - 0x000000ff: features that work for all past kernels
 * 0x00000100 - 0xffffffff: features that don't work for past kernels
 */
#define CRAMFS_FLAG_FSID_VERSION_2	0x00000001	/* fsid version #2 */
#define CRAMFS_FLAG_SORTED_DIRS		0x00000002	/* sorted dirs */
#define CRAMFS_FLAG_HOLES		0x00000100	/* support for holes */
#define CRAMFS_FLAG_WRONG_SIGNATURE	0x00000200	/* reserved */
#define CRAMFS_FLAG_SHIFTED_ROOT_OFFSET	0x00000400	/* shifted root fs */

/*
 * Valid values in super.flags.  Currently we refuse to mount
 * if (flags & ~CRAMFS_SUPPORTED_FLAGS).  Maybe that should be
 * changed to test super.future instead.
 */
#define CRAMFS_SUPPORTED_FLAGS	( 0x000000ff \
				| CRAMFS_FLAG_HOLES \
				| CRAMFS_FLAG_WRONG_SIGNATURE \
				| CRAMFS_FLAG_SHIFTED_ROOT_OFFSET )

#if __BYTE_ORDER == __LITTLE_ENDIAN
#define CRAMFS_16(x)	(x)
#define CRAMFS_24(x)	(x)
#define CRAMFS_32(x)	(x)
#define CRAMFS_GET_NAMELEN(x)	((x)->namelen)
#define CRAMFS_GET_OFFSET(x)	((x)->offset)
#define CRAMFS_SET_OFFSET(x,y)	((x)->offset = (y))
#define CRAMFS_SET_NAMELEN(x,y) ((x)->namelen = (y))
#elif __BYTE_ORDER == __BIG_ENDIAN
//#ifdef __KERNEL__
#define CRAMFS_16(x)	swab16(x)
#define CRAMFS_24(x)	((swab32(x)) >> 8)
#define CRAMFS_32(x)	swab32(x)
//#else				// not __KERNEL__ 
//#define CRAMFS_16(x)	bswap_16(x)
//#define CRAMFS_24(x)	((bswap_32(x)) >> 8)
//#define CRAMFS_32(x)	bswap_32(x)
//#endif				// not __KERNEL__ 
#define CRAMFS_GET_NAMELEN(x)	(((u8*)(x))[8] & 0x3f)
#define CRAMFS_GET_OFFSET(x)	((CRAMFS_24(((unsigned int*)(x))[2] & 0xffffff) << 2) |\
				 ((((unsigned int*)(x))[2] & 0xc0000000) >> 30))
#define CRAMFS_SET_NAMELEN(x,y)	(((u8*)(x))[8] = (((0x3f & (y))) | \
						  (0xc0 & ((u8*)(x))[8])))
#define CRAMFS_SET_OFFSET(x,y)	(((unsigned int*)(x))[2] = (((y) & 3) << 30) | \
				 CRAMFS_24((((y) & 0x03ffffff) >> 2)) | \
				 (((unsigned int)(((u8*)(x))[8] & 0x3f)) << 24))
#else
#error "__BYTE_ORDER must be __LITTLE_ENDIAN or __BIG_ENDIAN"
#endif

/* Uncompression interfaces to the underlying zlib */
extern int cramfs_uncompress_block(void *dst, int dstlen, void *src, int srclen);
int cramfs_uncompress_init(void);
void cramfs_uncompress_exit(void);

//--------------------< add by yuqj start

typedef struct cramfs_stat_i {
    __u32 mode;
    __u32 uid;
    __u32 size;
    __u8 gid;
}cramfs_stat_i;

#define CRAMFS_PAGE_SIZE (4096)
//-----------------------> add by yuqj end

#endif
