#ifndef _CRAMFS_FS_SB
#define _CRAMFS_FS_SB

#include <pkgconf/fs_cramfs.h>
#include <cyg/io/io.h>
#include <sys/types.h>
#include <asm/atomic.h>
#include <linux/stat.h>
#include <linux/compiler.h>

#include <pkgconf/system.h>
#include <pkgconf/hal.h>
#include <pkgconf/io_fileio.h>

#include <cyg/infra/cyg_trac.h>        // tracing macros
#include <cyg/infra/cyg_ass.h>         // assertion macros

#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
#include <dirent.h>

#include <stdlib.h>
#include <string.h>

#include <cyg/fileio/fileio.h>

#include <cyg/hal/drv_api.h>
#include <cyg/infra/diag.h>

#include <cyg/io/flash.h>
#include <cyg/fs/cramfs_fs.h>
#include <asm/bug.h>

/*
 * cramfs super-block data in memory
 */
typedef struct cramfs_sb_info {
	unsigned long magic;
	unsigned long size;
	unsigned long blocks;
	unsigned long files;
	unsigned long flags;
}cramfs_sb_info;

typedef struct cramfs_super_block {
	cramfs_sb_info	    cramfs_sb;
	//struct _inode *		s_root;
	cramfs_super super;
	unsigned long		s_mount_count;
	cyg_io_handle_t		s_dev;
	cyg_mtab_entry      *mte;
}cramfs_super_block;

#if 0
static inline struct cramfs_sb_info *CRAMFS_SB(struct cramfs_super_block *sb)
{
	return sb->cramfs_sb;
}
#endif

#endif
