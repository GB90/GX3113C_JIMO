#ifndef CYGONCE_GXNAND_H
#define CYGONCE_GXNAND_H

#include <pkgconf/io_nand.h>
#include <cyg/nand/nand_device.h>
#include <cyg/hal/drv_api.h>
#include <cyg/infra/diag.h>

#define		NFC_EN(_ctx_)       {_ctx_->cfg_module |= 0xFF030000;}		// en NFC module
#define		NAND_EN(_ctx_)      {_ctx_->sel |= 0x1;}			// select NAND Flash
#define		NAND_DIS(_ctx_)     {_ctx_->sel &= 0xFFFFFFFE;}			// diselect NAND Flash
#define		FIFO_CLEAR(_ctx_)   {_ctx_->cfg_module |= 0x00040000;}		// clear fifo

struct gx_nand_regs {

	volatile cyg_uint32 cfg_module;
	volatile cyg_uint32 sel;
	volatile cyg_uint32 timing_sequence;
	volatile cyg_uint32 cfg_datafifo;
	volatile cyg_uint32 cfg_ecc;
	volatile cyg_uint32 ctl_receive;
	volatile cyg_uint32 status_reg1;
	volatile cyg_uint32 status_reg2;
	volatile cyg_uint32 cmd_reg;
	volatile cyg_uint32 addr_reg;
	volatile cyg_uint32 wrdata_fifo;
	volatile cyg_uint32 rddata_fifo;
	volatile cyg_uint32 ecc_code[4][5];
	volatile cyg_uint32 ecc_wrong_location[4][6];
	volatile cyg_uint32 ecc_status;
	volatile cyg_uint32 cfg_dma_ctrl;
	volatile cyg_uint32 cfg_dma_trans_count;
};
/* Our private structure ======================================= */
// Every instance of the chip needs its own copy of this struct.
// N.B. that this is too big to go on the stack in certain
// eCos configurations; it should normally be static.
struct _gxnand_priv {

	void *plat_priv; // For use by the platform HAL, if desired.
	cyg_nand_page_addr pagestash; // Guarded by dev lock.

#ifdef CYGSEM_IO_NAND_USE_BBT

	unsigned char *bbt_data;

#endif
};

typedef struct _gxnand_priv gxnand_priv;

struct nand_ecc_config {

	cyg_uint32 data_len;

	cyg_uint8  type;
	cyg_uint8  start_sector;
	cyg_uint8  codes_per_sector;
	cyg_uint8  load;
	cyg_uint8  decode;
	cyg_uint8  start_new_ecc;
};

struct nand_flash_dev {

	char *name;
	int id;
	unsigned long chip_bits;
};

/* Prototypes for functions to be provided by the platform driver ==== */

// Low-level chip access functions ------------------
static inline void	write_data_32(struct gx_nand_regs *ctx,cyg_uint32 wrdata);  //send 32 data
static inline cyg_uint8   read_data_8(struct gx_nand_regs *ctx);	            //receive data
static inline cyg_uint32  read_data_32(struct gx_nand_regs *ctx);                   //receive 32 data
static inline cyg_uint8   read_status(struct gx_nand_regs *ctx);                    //read status
static inline void write_data_bulk(struct gx_nand_regs *ctx, const unsigned char *dp, size_t n);
static inline void write_data_bulk_with_dma(struct gx_nand_regs *ctx, const unsigned char *dp, size_t n);
static inline void read_data_bulk(struct gx_nand_regs *ctx, unsigned char *dp, size_t n);
static inline void read_data_bulk_with_dma(struct gx_nand_regs *ctx, unsigned char *dp, size_t n);
static inline void write_addrbytes(struct gx_nand_regs *ctx, CYG_BYTE *buf, size_t n);
static inline void change_read_column(struct gx_nand_regs *ctx, cyg_nand_column_addr col);
static inline void change_write_column(struct gx_nand_regs *ctx, cyg_nand_column_addr col);

// Chip concurrent-access protection ----------------
// (This need not do anything, if the library-provided
//  per-device locking is sufficient.)
static int gx_nand_partition_setup(cyg_nand_device *dev);
static inline int gx_nand_init_manual_partition(cyg_nand_device *dev);
#endif
