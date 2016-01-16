#ifndef STORAGE_H
#define STORAGE_H
#include <linux/types.h>
#include <cyg/kernel/kapi.h>
#include <cyg/io/disk.h>
//-------------------------------------------------------------------------------------------
// SCSI layer.
//-------------------------------------------------------------------------------------------
struct TMaxCapacityDesc
{
	u8 Reserved_1[3];
	u8 Length;
	u32 BlockCount;
	u8 DescriptorCode;
	u8 BlockSize[3];
	//u8 reserved[500];
};
//-------------------------------------------------------------------------------------------
// MASS STORAGE layer.
//-------------------------------------------------------------------------------------------
struct TCBW
{
	u32 dCBWSignature;
	u32 dCBWTag;
	u32 dCBWDataTransferLength;
	u8 bmCBWFlags;
	u8 bCBWLUN;
	u8 bCBWCBLength;
	u8 CBWCB[16];
};
//-------------------------------------------------------------------------------------------
struct TCSW
{
	u32 dCSWSignature;
	u32 dCSWTag;
	u32 dCSWDataResidue;
	u8 bCSWStatus;
};

struct bulkonly_dev {
	struct usb_device		*usb_dev;
	struct usb_interface 	*interface;		/* the interface for this device */
	int lun;
	unsigned int		bulk_in_endpointAddr;	/* the address of the bulk in endpoint */
	unsigned int		bulk_out_endpointAddr;	/* the address of the bulk out endpoint */
	cyg_mutex_t 			dev_mutex;
	unsigned int		err_cnt;
	int (*transport_handler)(struct bulkonly_dev	*bdev,
							int				lun,
							void				*cmd,
							unsigned char		cmd_length,
							void				*data,
							unsigned long		length,
							int 				direction);
	
	u32 tag;
};
//-------------------------------------------------------------------------------------------
#define CSW_OUT 0x00
#define CSW_IN  0x80

#define __USB_DMA_BUFFER_SIZE  (1024*4)

/* FIXED: 每次读写BURST传输都须有限制, 即最大到512K字节 */
#define __BURST_READ_MAX_SECTOTS		1024
#define __BURST_WRITE_MAX_SECTORS		1024
//==============add by gaohb========>>>
#define USB_DISK 1
#define MAX_DISK_NUMBER 10
#define MAX_DISK_ERR 5

typedef struct cyg_usb_disk_info_t {  
    cyg_uint32          		 usb_dev;				//卡设备
	volatile cyg_uint32          usb_block_count;	 	//读写扇区量 
    cyg_bool           			 usb_read_only;		 	//读标志
	volatile cyg_bool            usb_connected;			//卡挂载标志
    struct scsi_disk  			 *gx_usb_scsi;
    cyg_sem_t					 disk_sem;				//usb init complete semaphore
	volatile int				 disk_index;			//磁盘序号
	volatile int				 connect_flag;
	cyg_devtab_entry_t			 *usb_tab;
}cyg_usb_disk_info_t;

extern cyg_usb_disk_info_t cyg_usb_disk_hwinfo[MAX_DISK_NUMBER];

#define USB_DISK_CHANNEL(_I)					\
	DISK_CHANNEL(cyg_usb_disk## _I ##_channel,	\
             cyg_usb_disk_funs,					\
             cyg_usb_disk_hwinfo[_I],			\
             cyg_usb_disk_controller_##_I,		\
             true,                          	\
             24                             	\
             );

#define USB_DISK_CONTROLLER(_I)					\
DISK_CONTROLLER(cyg_usb_disk_controller_##_I, cyg_usb_disk_hwinfo[_I])
//==============add by gaohb========<<<
struct scsi_disk {
#define MAX_COMMAND_SIZE 16
	unsigned char cmnd[MAX_COMMAND_SIZE + 16];

#define SCSI_SENSE_BUFFERSIZE 96
	unsigned char sense_buffer[SCSI_SENSE_BUFFERSIZE];

	struct usb_device *usb_dev;
	cyg_usb_disk_info_t *disk_info;
	int index;
	int lun;
	unsigned long block_size;
	unsigned long block_count;
	unsigned char product_name[16];

	unsigned long priv[0]; /* Used for storage of host specific stuff */
}__attribute__ ((aligned (sizeof(unsigned long))));

int scsi_write_sector(struct scsi_disk *disk, void *Buffer, u32 SectorCount, u32 StartSector);
int scsi_read_sector(struct scsi_disk *disk, void *Buffer, u32 SectorCount, u32 StartSector);
void _usb_subsystem_init(void);
#endif
