#ifndef __GX_PARAMTABLE_H__
#define __GX_PARAMTABLE_H__

#include "common/config.h"

#define FLASH_PARTITION_ADDR         (0x10000)
#define FLASH_PARTITION_SIZE         (512)
#define FLASH_PARTITION_NUM          (12)
#define FLASH_PARTITION_HEAD_SIZE    (24)
#define FLASH_PARTITION_NAME_SIZE    (7)
#define MAGIC_SIZE                   (4)
#define PARTITION_NUM_SIZE           (1)
#define FLASH_PARTITION_ADDR_EX      (FLASH_PARTITION_HEAD_SIZE * 17 + MAGIC_SIZE + PARTITION_NUM_SIZE)
#define FLASH_PARTITION_VERSION_ADDR (FLASH_PARTITION_HEAD_SIZE * FLASH_PARTITION_NUM + MAGIC_SIZE + PARTITION_NUM_SIZE)
#define FLASH_PARTITION_RESERVED_LEN (41)

#define FLASH_SECTOR_SIZE            (64 * 1024)
#define PARTITION_MAGIC              0XAABCDEFA

#define PARTITION_BOOT               ("BOOT")
#define PARTITION_LOGO               ("LOGO")
#define PARTITION_OTA                ("OTA")
#define PARTITION_I_OEM              ("I_OEM")
#define PARTITION_V_OEM              ("V_OEM")
#define PARTITION_KERNEL             ("KERNEL")
#define PARTITION_ROOT               ("ROOT")
#define PARTITION_TABLE              ("TABLE")

#define RAW     0
#define ROMFS   1
#define CRAMFS  2
#define JFFS2   3
#define YAFFS2  4
#define RAMFS   5
#define MINIFS  127

#define PARTITION_VERSION 102

__BEGIN_DECLS

/*
 * VERION 1.0.0 (<= V1.0.1-5)
 * +----------------------------+
 * |            512             |
 * +-------+-------+------------+
 * |   4   |   1   |    ...     |
 * +-------+-------+------------+
 * |       |       |            |
 * | MAGIC | COUNT | BASE TABLE |
 * |       |       |            |
 * +-------+-------+------------+
 *
 * VERION 1.0.2 ( <= V1.0.2-0-RC5)
 * +--------------------------------------------~----------------------------------------------+
 * |                                           512                                             |
 * +-------+-------+------------------------+---~----+---------+--------+----------+-----------+
 * |       |       |           360          |        |         |        |          |           |
 * |   4   |   1   +------------+-----------+  141   |    1    |    1   |    1     |     4     |
 * |       |       |  17 * 24   |  17 * 4   |        |         |        |          |           |
 * +-------+-------+------------+-----------+---~----+---------+--------+----------+-----------+
 * |       |       |            |           |        |         |        |          |           |
 * | MAGIC | COUNT | BASE TABLE | CRC TABLE | UNUSED | PROTECT | CRC_EN | PART VER | TABLE CRC |
 * |       |       |            |           |        |         |        |          |           |
 * +-------+-------+------------+-----------+---~----+---------+--------+----------+-----------+
 * |0      |4      |5           |413                 |505      |506     |507        |508
 *
 * VERION 1.0.3
 * +---------------------------------------------------------------~----------------------------------------------+
 * |                                                       512                                                    |
 * +-------+-------+-------------------------------------------+---~----+---------+--------+----------+-----------+
 * |       |       |                                           |        |         |        |          |           |
 * |   4   |   1   +------------+-----------+------+-----------+   45   |    1    |    1   |    1     |     4     |
 * |       |       |  12 * 24   |  12 * 2   |  96  |  12 * 4   |        |         |        |          |           |
 * +-------+-------+------------+-----------+------+-----------+---~----+---------+--------+----------+-----------+
 * |       |       |            |           |      |           |        |         |        |          |           |
 * | MAGIC | COUNT | BASE TABLE | VER TABLE |  UN  | CRC TABLE | UNUSED | PROTECT | CRC_EN | PART VER | TABLE CRC |
 * |       |       |            |           |      |           |        |         |        |          |           |
 * +-------+-------+------------+-----------+------+-----------+---~----+---------+--------+----------+-----------+
 * |0      |4      |5           |293               |413                 |505      |506     |507        |508
 */


struct partition_info {
	char     name[FLASH_PARTITION_NAME_SIZE + 1];
	uint32_t total_size;
	uint32_t used_size;
	uint32_t start_addr;
	char     file_system_type;
	char     mode ;
	char     crc32_enable;
	char     id;
	char     write_protect;
	char     update_tags;
	uint32_t crc_verify;
	uint16_t version;
} __attribute__ ((packed));

struct partition_io_ops {
	int (*open)(const char *file);
	int (*lseek)(int fd, off_t offset, int whence);
	int (*read)(int fd, void *buf, size_t count);
	int (*write)(int fd, const void *buf, size_t count);
	int (*close)(int fd);
	int (*getsize)(int fd);
	int (*protect_lock)(int fd, unsigned long len);
	int (*protect_unlock)(int fd);
	int (*protect_status)(int fd, int *mode);
};

struct partition {
	unsigned int magic;                                      // 0-3
	unsigned char count;                                     // 4
	struct partition_info tables[FLASH_PARTITION_NUM];       // 5-364

	unsigned char write_protect;                             // 505
	unsigned char crc32_enable;                              // 506
	char version;                                            // 507
	unsigned int crc_verify;                                 // 508-512
	struct partition_io_ops *ops;
	int fd;
} __attribute__ ((packed));

struct partition *GxCore_PartitionFlashInit      (void);
struct partition *GxCore_PartitionInit           (struct partition *table, struct partition_io_ops *ops, const char *filename);
struct partition_info *GxCore_PartitionGetByName (struct partition *table, const char* name);
struct partition_info *GxCore_PartitionGetById   (struct partition *table, int id);
char  *GxCore_PartitionGetType                   (struct partition_info *partition);
void   GxCore_PartitionPrint                     (struct partition *table);
int    GxCore_PartitionVerity                    (struct partition *table);
int    GxCore_PartitionRead                      (struct partition_info *partition, void *buf,size_t size);
int    GxCore_PartitionClear                     (struct partition_info *partition);
int    GxCore_PartitionWrite                     (struct partition_info *partition,void *buf, size_t size);
int    GxCore_PartitionTableSave                 (struct partition* table);

__END_DECLS

#endif

