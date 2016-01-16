#ifndef GX3110_SD_DEFINE_H
#define GX3110_SD_DEFINE_H
/*==============================================================================
name:        gx3110_sd_define.h
Descript:    define some constant and struct for the operation of gx31x0 SD card.

Author:      gaohaibo
Date:        2009-06-16
==============================================================================*/

#include <cyg/infra/cyg_type.h>     // Common types 
#include <cyg/kernel/kapi.h>
#define  SD_CARD 	             0	//the default card type is sd card 
#define  MMC_CARD 	 			 1
#define  MS_CARD     			 2
#define  MHZ         			 1000000
#define SIO_BAUDRATE_INIT		 (3*MHZ)
#define SD_BLOCK_SIZE	   		 512 //SD卡默认块大小 
#define BUS_WIDTH_1B   			 0
#define BUS_WIDTH_4B   			 2   // just meet the argument register
#define BUS_WIDTH_8B   			 1   
#define CMD_RETRY_TIMES    		 5   //命令重试次数
#define SINGLE_BLOCK_OPERATION   0
#define MULTILY_BLOCK_OPERATION  1
/***************Constants**********/
#define SD_OK					(0)
#define SD_ERROR				(1)
#define SD_NO_RESPONSE			(2)
#define SD_RES_TIMEOUT			(3)
#define SD_UNUSABLE				(4)
#define SD_DATA_TIMEOUT			(5)
#define SD_READ_TIMEOUT			(6)
#define SD_RES_ERR				(7)
#define SD_ERR_DATA_READY		(8)
#define SD_PROGRAM_FAIL			(9)
/* SD/MMC Response CRC Err */
#define SD_CMD_CRC_ERR			(10)
#define SD_CMD_IDX_ERR			(11)
/* SD/MMC Send/Receive Data CRC Err */
#define SD_CRC_ERR_RECEIVE		(12)
#define SD_CRC_ERR_SEND			(13)
/* CURRENT STATUS in R1 R1b respones */
#define IDLE_STATE				(0)
#define READY_STATE				(1)
#define IDENT_STATE				(2)
#define STBY_STATE				(3)
#define TRAN_STATE				(4)
#define DATA_STATE				(5)
#define RCV_STATE				(6)
#define PRG_STATE				(7)
#define DIS_STATE				(8)
/*  R1 status: card status  */
#define R1_out_of_range			0x80000000	// er, c 
#define R1_address_err			0x40000000	// erx,c 
#define R1_block_len_err		0x20000000	// er, c 
#define MMC_VDD_27_36			0x00ff8000	// VDD voltage 2.7 ~ 3.6 
#define HCS						0x40000000
extern int SDI_CSD_VERSION; 
extern volatile cyg_uint32 SYS_SDC_CLOCK;
extern volatile cyg_uint32 rPIN_FUNCTION_SEL_1;			//0xd050a130
extern volatile cyg_uint32 rPIN_FUNCTION_SEL_2;	
//PRESENT status (rMCC_PRESENT_STATE>>xx bit)
#define		MS_BUF_IN		 10
#define		SD_BUF_IN		 9
#define		CMD_FINISH		 8
#define     WP_LEVEL         7
#define     BUF_RD_EN        6
#define     BUF_WR_EN        5
#define     RD_TRANS_ACT     4
#define     WR_TRANS_ACT     3
#define     DAT_LINE_ACT     2
#define     CMD_INH_DAT      1
#define     CMD_INH_CMD      0 
//INT_REGISTER ( rMCC_INT_REGISTER>>xx bit)
#define		CMD_FINISH_INT		21
#define		BLK_FINISH_INT		20
#define		IO_INT				19
#define		NAC_TIMEOUT			18 
#define		DAT_CRC_ERR			13 
#define		RDY_TIMEOUT			12
#define		NO_REPONSE			11
#define		CRC_ERR_SEND		10
#define		FLASH_PRO_ERR		9 
#define     CMD_CRC_ERR         8 
#define     CMD_IDX_ERR         7 
#define     CRC_ERR_RECEIVE     6 
#define     BUF_RD_RDY_INT      5
#define     BUF_WR_RDY_INT      4 
#define     ACK_INT             3 
#define     DMA_FINISH          2
#define     TRANS_COMP_INT      1
//INT_REGISTER write 1 to clear( rMCC_INT_REGISTER & xx)
#define		CMD_FINISH_INT_CLR	    0x00200000	//21
#define		BLK_FINISH_INT_CLR		0x00100000	//20
#define		IO_INT_CLR				0x00080000	//19
#define		NAC_TIMEOUT_CLR			0x00040000	//18
#define		DAT_CRC_ERR_CLR			0X00002000	//13
#define		RDY_TIMEOUT_CLR			0X00001000	//12
#define     NO_RESPONSE_CLR		    0X00000800	//11
#define		CRC_ERR_SEND_CLR		0X00000400	//10
#define		FLASH_PRO_ERR_CLR		0X00000200	//9
#define     CMD_CRC_ERR_CLR         0X00000100	//8
#define     CMD_IDX_ERR_CLR         0X00000080	//7
#define     CRC_ERR_RECEIVE_CLR     0X00000040	//6
#define     BUF_RD_RDY_INT_CLR      0X00000020	//5
#define     BUF_WR_RDY_INT_CLR      0X00000010	//4
#define     ACK_INT_CLR             0X00000008	//3
#define     DMA_FINISH_INT_CLR      0X00000004	//2
#define     TRANS_COMP_INT_CLR      0X00000002	//1
#define		BOOT_FINISH_CLR			0X00000001  //0

typedef struct sdi_cid_register {
    cyg_uint32   cid_data[4];					//卡ID
}sdi_cid_register;

typedef struct gx_sdi_controller_t{
	volatile cyg_uint32 GX3110_MCC_BaseAddr;  
	volatile cyg_uint32 rMCC_BLOCK_ARGUMENT; 
	volatile cyg_uint32 rMCC_CMD_ARGUMENT;
	volatile cyg_uint32 rMCC_CMD_TYPE;
	volatile cyg_uint32 rMCC_REPONSE_0;
	volatile cyg_uint32 rMCC_REPONSE_1;
	volatile cyg_uint32 rMCC_REPONSE_2;
	volatile cyg_uint32 rMCC_REPONSE_3;
	volatile cyg_uint32 rMCC_BUF_DATA_PORT;
	volatile cyg_uint32 rMCC_PRESENT_STATE;
	volatile cyg_uint32 rMCC_CARD_CTRL;
	volatile cyg_uint32 rMCC_SCLK_CTRL;
	volatile cyg_uint32 rMCC_INT_REGISTER;
	volatile cyg_uint32 rMCC_DMA_CTRL;
	volatile cyg_uint32 rMCC_BOOT_CTRL;
	volatile cyg_uint32 rMCC_NAC_TIME;
	volatile cyg_uint32 rMCC_INT_EN1;
	volatile cyg_uint32 rMCC_INT_EN2;
}gx_sdi_controller_t;

typedef struct cyg_sdi_disk_info_t {  
    volatile cyg_uint32 sdi_dev;				//卡设备
    cyg_uint32          sdi_saved_baudrate;	
    cyg_uint8           sdi_bus_width;       	//数据线宽度
    cyg_uint32          sdi_block_count;	 	//读写扇区量 
    cyg_bool            sdi_read_only;		 	//读标志
    cyg_bool            sdi_connected;			//卡挂载标志
	cyg_bool			sdi_high_capacity_flag; //高容卡标志
	cyg_uint32          sdi_heads_per_cylinder;	//
    cyg_uint32          sdi_sectors_per_head;	//
    cyg_uint32          sdi_read_block_length;	//读扇区大小
    cyg_uint32          sdi_write_block_length;	//写扇区大小
    struct sdi_cid_register   sdi_id;			//卡ID
    cyg_uint32          sdi_rca;				//rca 
    cyg_uint32          sdi_buffer_addr;		//DMA读写数据块的缓冲区开始地址
    struct  gx_sdi_controller_t  *sdi_ctrl;		//sd卡控制器结构体
    CYG_WORD       		sdi_intr_num;			//interrupt number 	
    cyg_interrupt  		sdi_interrupt_data;		//中断对象返回地址			
    cyg_handle_t   		sdi_interrupt_handle;	//interrupt handle
    cyg_sem_t			sdi_interrupt_sem;		//中断事物信号量
    int					sdi_err_type;
	cyg_uint32			chip_type;				//芯片类型
	int 				ms_card_init_finish_flag;//MS card has been finished init
   }cyg_sdi_disk_info_t;

#define SD_CID_REGISTER_MID(_data_)                    ((_data_)->cid_data[0]>>24)
#define SD_CID_REGISTER_OID(_data_)                    (((_data_)->cid_data[0]&0x00FFFF00)>>16) 
#define SD_CID_REGISTER_PNM(_data_)                    ((const cyg_uint64*)&((((_data_)->cid_data[0]) & 0xFF)<<32) | (_data_)->cid_data[1])
#define SD_CID_REGISTER_PRV(_data_)                    ((_data_)->cid_data[2]>>24)
#define SD_CID_REGISTER_PSN(_data_)                    (((_data_)->cid_data[2]&0x00FFFFFF) |\
															((_data_)->cid_data[3]&0xFF000000))
#define SD_CID_REGISTER_MDT(_data_)                    ((_data_)->cid_data[3]&0x000FFF00)
#define SD_CID_REGISTER_CRC(_data_)                    (((_data_)->cid_data[3]&0x000000FE)>>1)
                                                     
// The CSD register is just lots of small bitfields. For now keep it
// as an array of 16 bytes and provide macros to read the fields.
typedef struct sdi_csd_register {
    cyg_uint32   csd_data[4];
} sdi_csd_register;

#define SD_CSD_REGISTER_CSD_STRUCTURE(_data_)          (((_data_)->csd_data[0] & 0xC0000000) >> 30)
#define SD_CSD_REGISTER_SPEC_VERS(_data_)              (SD_CSD_REGISTER_CSD_STRUCTURE(_data_)+1)
#define SD_CSD_REGISTER_TAAC(_data_)                   (((_data_)->csd_data[0] & 0x00FF0000) >> 16)
#define SD_CSD_REGISTER_TAAC_EXPONENT(_data_)          (SD_CSD_REGISTER_TAAC(_data_)&0x7)
#define SD_CSD_REGISTER_TAAC_MANTISSA(_data_)          ((SD_CSD_REGISTER_TAAC(_data_)&0x78)>>3)
#define SD_CSD_REGISTER_NSAC(_data_)                   (((_data_)->csd_data[0] & 0x0000FF00) >> 8)
#define SD_CSD_REGISTER_TRAN_SPEED(_data_)             ((_data_)->csd_data[0] & 0x000000FF)
#define SD_CSD_REGISTER_TRAN_SPEED_EXPONENT(_data_)    (SD_CSD_REGISTER_TRAN_SPEED(_data_)&0x7)
#define SD_CSD_REGISTER_TRAN_SPEED_MANTISSA(_data_)    (((SD_CSD_REGISTER_TRAN_SPEED(_data_))>>3)&0xF)
                                         
#define SD_CSD_REGISTER_CCC(_data_)                    (((_data_)->csd_data[1] & 0xFFF00000) >> 20)                                           
#define SD_CSD_REGISTER_READ_BL_LEN(_data_)            (((_data_)->csd_data[1] & 0x000F0000) >> 16)                                         
#define SD_CSD_REGISTER_READ_BL_PARTIAL(_data_)        (((_data_)->csd_data[1] & 0x00008000) >> 15)                                         
#define SD_CSD_REGISTER_WRITE_BLK_MISALIGN(_data_)     (((_data_)->csd_data[1] & 0x00004000) >> 14)                                         
#define SD_CSD_REGISTER_READ_BLK_MISALIGN(_data_)      (((_data_)->csd_data[1] & 0x00002000) >> 13)                                         
#define SD_CSD_REGISTER_DSR_IMP(_data_)                (((_data_)->csd_data[1] & 0x00001000) >> 12)                                                         

#define SD_CSD_REGISTER_C_SIZE_0(_data_)                ((((_data_)->csd_data[1] & 0x000003FF) << 2) |     \
                                                       (((_data_)->csd_data[2] & 0xC0000000) >>30))       //CSD VERSION 1.0                                                        
#define SD_CSD_REGISTER_C_SIZE_MULT(_data_)            (((_data_)->csd_data[2] & 0x00038000) >>15)   //VERSION 1.0                                                       
#define SD_CSD_REGISTER_C_SIZE_1(_data_)                ((((_data_)->csd_data[1] & 0x0000003F) << 16) |     \
                                                       (((_data_)->csd_data[2] & 0xFFFF0000) >>16))       //CSD VERSION 2.0 
#define SD_CSD_REGISTER_ERASE_BLK_EN(_data_)           (((_data_)->csd_data[2] & 0x00004000) >>14)                                              
#define SD_CSD_REGISTER_SECTOR_SIZE(_data_)            (((_data_)->csd_data[2] & 0x00003F80) >>7)
#define SD_CSD_REGISTER_WR_GRP_SIZE(_data_)             ((_data_)->csd_data[2] & 0x0000007F)
#define SD_CSD_REGISTER_WR_GRP_ENABLE(_data_)          (((_data_)->csd_data[3] & 0x80000000) >> 31)
#define SD_CSD_REGISTER_DEFAULT_ECC(_data_)            (((_data_)->csd_data[3] & 0x60000000) >> 29)
#define SD_CSD_REGISTER_R2W_FACTOR(_data_)             (((_data_)->csd_data[3] & 0x1C000000) >> 26)
#define SD_CSD_REGISTER_WRITE_BL_LEN(_data_)           (((_data_)->csd_data[3] & 0x03C00000) >> 22) 
#define SD_CSD_REGISTER_WR_BL_PARTIAL(_data_)          (((_data_)->csd_data[3] & 0x00200000) >> 21)
#define SD_CSD_REGISTER_FILE_FORMAT_GROUP(_data_)      (((_data_)->csd_data[3] & 0x00008000) >> 15)
#define SD_CSD_REGISTER_COPY(_data_)                   (((_data_)->csd_data[3] & 0x00004000) >> 14)
#define SD_CSD_REGISTER_PERM_WRITE_PROTECT(_data_)     (((_data_)->csd_data[3] & 0x00002000) >> 13)
#define SD_CSD_REGISTER_TMP_WRITE_PROTECT(_data_)      (((_data_)->csd_data[3] & 0x00001000) >> 12)
#define SD_CSD_REGISTER_FILE_FORMAT(_data_)            (((_data_)->csd_data[3] & 0x00000C00) >> 10)
#define SD_CSD_REGISTER_ECC(_data_)                    (((_data_)->csd_data[3] & 0x00000300) >> 8)
#define SD_CSD_REGISTER_CRC(_data_)                    (((_data_)->csd_data[3] & 0x000000FE) >> 1)

//#define CONFIG_MS_DEBUG
#ifdef CONFIG_MS_DEBUG
#include <cyg/infra/diag.h>    //for diag_printf
#define DEBUG(format, ...) diag_printf(format, ## __VA_ARGS__)
#else
#define DEBUG(...)
#endif

#define DEBUG_SD  0
#if DEBUG_SD > -1
# define DEBUG0(format, ...)    diag_printf(format, ## __VA_ARGS__)
#else
# define DEBUG0(format, ...)
#endif
#if DEBUG_SD > 0
# define DEBUG1(format, ...)    diag_printf(format, ## __VA_ARGS__)
#else
# define DEBUG1(format, ...)
#endif
#if DEBUG_SD > 1
# define DEBUG2(format, ...)    diag_printf(format, ## __VA_ARGS__)
#else
# define DEBUG2(format, ...)
#endif

cyg_bool  get_bit(cyg_uint32 reg,cyg_uint32 bit_num);
void set_clock(cyg_sdi_disk_info_t *disk,cyg_uint32 speed);
#endif
