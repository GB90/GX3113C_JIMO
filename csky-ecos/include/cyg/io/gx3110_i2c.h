#ifndef _CX_ARM_GXI2C_H_
#define _CX_ARM_GXI2C_H_

#include <cyg/infra/cyg_type.h>
#include <cyg/hal/drv_api.h>

#include <cyg/infra/diag.h>

//#define ENABLE_DW_I2C_BUS		//designware i2c bus's driver is not ok now

#define SYS_CLK				27000000UL
#define I2C_DEFAULT_CLK		100000UL

#ifndef EAGAIN
#define EAGAIN           	11    /* Try again later */
#endif

/*Private types/constants */
enum {
	I2C_READ = 0,
	I2C_WRITE
};

enum {
	I2C_WRITE_FIRST_ADDR_FLG = 2,
	I2C_WRITE_SECOND_ADDR_FLG,
	I2C_WRITE_DATA_FLG,
	I2C_READ_DATA_AND_STOP_FLG,
	I2C_SEND_READ_ACK_FLG,
	I2C_SEND_STOP_FLG,
	I2C_FINISH_FLG,
};

#ifdef CYGPKG_HAL_ARM
#define I2C1BaseAddr 		(0xd0405000)
#define I2C2BaseAddr 		(0xd0406000)
#define I2C_BUSES_MAX		2
#define I2C1_ISR_VECTOR		5
#define I2C2_ISR_VECTOR		6
#endif

#ifdef CYGPKG_HAL_CKMMU
#define I2C1BaseAddr 		(0xa0205000)
#define I2C2BaseAddr 		(0xa0206000)
#define I2C3BaseAddr 		(0xa0203000)
#define DW_I2C1BaseAddr 	(0xa0202000)
#define I2C_BUSES_MAX		2
#define I2C1_ISR_VECTOR		5	//is same as dw_i2c_1?
#define I2C2_ISR_VECTOR		6
#define I2C3_ISR_VECTOR		3
#define DW_I2C_ISR_VECTOR 	2
#endif

#define I2C_BASE_ADDR(_extra_)  			((_extra_)->i2c_base)

/* GX I2C register */
#define I2C_I2CON(_extra_)      			(I2C_BASE_ADDR(_extra_) + 0X0000)
#define I2C_I2DATA(_extra_)     			(I2C_BASE_ADDR(_extra_) + 0X0004)
#define I2C_I2DIV(_extra_)      			(I2C_BASE_ADDR(_extra_) + 0X0008)

#define I2CON_W_ACK      					(1 << 1) //write ack
#define I2CON_R_ACK      					(1 << 2) //read ack
#define I2CON_SI         					(1 << 3) //operatin finish
#define I2CON_STO        					(1 << 4) //stop
#define I2CON_STA        					(1 << 5) //start
#define I2CON_I2EN       					(1 << 6) //module enable
#define I2CON_I2IE       					(1 << 7) //interrupt enable

#define I2C_EN(_extra_)                   	(HAL_WRITE_UINT32(I2C_I2CON(_extra_), I2CON_I2EN))
#define I2C_WRITE_ACK(_extra_, val)       	(HAL_READ_UINT32(I2C_I2CON(_extra_), val))
#define I2C_OP_COMPLETE(_extra_, val)     	(HAL_READ_UINT32(I2C_I2CON(_extra_), val))

#define I2C_INT_SEND_START(_extra_)       	(HAL_WRITE_UINT32(I2C_I2CON(_extra_), I2CON_I2IE|I2CON_I2EN|I2CON_STA))
#define I2C_INT_SEND_STOP(_extra_)        	(HAL_WRITE_UINT32(I2C_I2CON(_extra_), I2CON_I2IE|I2CON_I2EN|I2CON_STO))
#define I2C_INT_SEND_WRITE(_extra_)       	(HAL_WRITE_UINT32(I2C_I2CON(_extra_), I2CON_I2IE|I2CON_I2EN))
#define I2C_INT_SEND_READ_NO_ACK(_extra_) 	(HAL_WRITE_UINT32(I2C_I2CON(_extra_), I2CON_I2IE|I2CON_I2EN|I2CON_R_ACK))
#define I2C_INT_SEND_READ_ACK(_extra_)    	(HAL_WRITE_UINT32(I2C_I2CON(_extra_), I2CON_I2IE|I2CON_I2EN))
#define I2C_I2CON_CLR(_extra_)            	(HAL_WRITE_UINT32(I2C_I2CON(_extra_), 0))

#define I2C_SEND_START(_extra_)           	(HAL_WRITE_UINT32(I2C_I2CON(_extra_), I2CON_I2EN|I2CON_STA))
#define I2C_SEND_STOP(_extra_)            	(HAL_WRITE_UINT32(I2C_I2CON(_extra_), I2CON_I2EN|I2CON_STO))
#define I2C_SEND_WRITE(_extra_)           	(HAL_WRITE_UINT32(I2C_I2CON(_extra_), I2CON_I2EN))
#define I2C_SEND_READ_NO_ACK(_extra_)     	(HAL_WRITE_UINT32(I2C_I2CON(_extra_), I2CON_I2EN|I2CON_R_ACK))
#define I2C_SEND_READ_ACK(_extra_)        	(HAL_WRITE_UINT32(I2C_I2CON(_extra_), I2CON_I2EN))

#define I2C_WRITE_DATA(_extra_, val)      	(HAL_WRITE_UINT32(I2C_I2DATA(_extra_), val))
#define I2C_READ_DATA(_extra_, val)       	(HAL_READ_UINT32(I2C_I2DATA(_extra_), val))


/* DW I2C register */
#define DW_IC_CON(_extra_)					(I2C_BASE_ADDR(_extra_) + 0x0000)
#define DW_IC_TAR(_extra_)					(I2C_BASE_ADDR(_extra_) + 0x0004)
#define DW_IC_SAR(_extra_)					(I2C_BASE_ADDR(_extra_) + 0x0008)
#define DW_IC_DATA_CMD(_extra_)				(I2C_BASE_ADDR(_extra_) + 0x0010)
#define DW_IC_SS_SCL_HCNT(_extra_)			(I2C_BASE_ADDR(_extra_) + 0x0014)
#define DW_IC_SS_SCL_LCNT(_extra_)			(I2C_BASE_ADDR(_extra_) + 0x0018)
#define DW_IC_FS_SCL_HCNT(_extra_)			(I2C_BASE_ADDR(_extra_) + 0x001c)
#define DW_IC_FS_SCL_LCNT(_extra_)			(I2C_BASE_ADDR(_extra_) + 0x0020)
#define DW_IC_INTR_STAT(_extra_)			(I2C_BASE_ADDR(_extra_) + 0x002c)
#define DW_IC_INTR_MASK(_extra_)			(I2C_BASE_ADDR(_extra_) + 0x0030)
#define DW_IC_RAW_INTR_STAT(_extra_)		(I2C_BASE_ADDR(_extra_) + 0x0034)
#define DW_IC_RX_TL(_extra_)				(I2C_BASE_ADDR(_extra_) + 0x0038)
#define DW_IC_TX_TL(_extra_)				(I2C_BASE_ADDR(_extra_) + 0x003c)
#define DW_IC_CLR_INTR(_extra_)				(I2C_BASE_ADDR(_extra_) + 0x0040)
#define DW_IC_CLR_RX_UNDER(_extra_)			(I2C_BASE_ADDR(_extra_) + 0x0044)
#define DW_IC_CLR_RX_OVER(_extra_)			(I2C_BASE_ADDR(_extra_) + 0x0048)
#define DW_IC_CLR_TX_OVER(_extra_)			(I2C_BASE_ADDR(_extra_) + 0x004c)
#define DW_IC_CLR_RD_REQ(_extra_)			(I2C_BASE_ADDR(_extra_) + 0x0050)
#define DW_IC_CLR_TX_ABRT(_extra_)			(I2C_BASE_ADDR(_extra_) + 0x0054)
#define DW_IC_CLR_RX_DONE(_extra_)			(I2C_BASE_ADDR(_extra_) + 0x0058)
#define DW_IC_CLR_ACTIVITY(_extra_)			(I2C_BASE_ADDR(_extra_) + 0x005c)
#define DW_IC_CLR_STOP_DET(_extra_)			(I2C_BASE_ADDR(_extra_) + 0x0060)
#define DW_IC_CLR_START_DET(_extra_)		(I2C_BASE_ADDR(_extra_) + 0x0064)
#define DW_IC_CLR_GEN_CALL(_extra_)			(I2C_BASE_ADDR(_extra_) + 0x0068)
#define DW_IC_ENABLE(_extra_)				(I2C_BASE_ADDR(_extra_) + 0x006c)
#define DW_IC_STATUS(_extra_)				(I2C_BASE_ADDR(_extra_) + 0x0070)
#define DW_IC_TXFLR(_extra_)				(I2C_BASE_ADDR(_extra_) + 0x0074)
#define DW_IC_RXFLR(_extra_)				(I2C_BASE_ADDR(_extra_) + 0x0078)
#define DW_IC_SDA_HOLD(_extra_)				(I2C_BASE_ADDR(_extra_) + 0x007c)
#define DW_IC_TX_ABRT_SOURCE(_extra_)		(I2C_BASE_ADDR(_extra_) + 0x0080)
#define DW_IC_SLV_DATA_NACK_ONLY(_extra_)	(I2C_BASE_ADDR(_extra_) + 0x0084)
#define DW_IC_SDA_SETUP(_extra_)			(I2C_BASE_ADDR(_extra_) + 0x0094)
#define DW_IC_ACK_GENERAL_CALL(_extra_)		(I2C_BASE_ADDR(_extra_) + 0x0098)
#define DW_IC_ENABLE_STATUS(_extra_)		(I2C_BASE_ADDR(_extra_) + 0x009c)
#define DW_IC_FS_SPKLEN(_extra_)			(I2C_BASE_ADDR(_extra_) + 0x00a0)

/*DW_IC_CON*/
#define DW_CON_IC_SLAVE_DISABLE				(1 << 6)
#define DW_CON_IC_RESTART_EN				(1 << 5)
#define DW_CON_IC_10BITADDR_MASTER			(1 << 4)
#define DW_CON_IC_10BITADDR_SLAVE			(1 << 3)
#define DW_CON_SPEED(value)					(value << 1)	//b2~b1
#define DW_CON_MASTER_MODE					(1 << 0)

/*DW_IC_TAR*/
#define DW_TAR_IC_10BITADDR_MASTER			(1 << 12)
#define DW_TAR_SPECIAL						(1 << 11)
#define DW_TAR_GC_OR_START					(1 << 10)
#define DW_TAR_IC_TAR(value)				(value << 0) 	//b9~b0

/*DW_IC_DATA_CMD*/
#define DW_DATA_CMD_RESTART		      		(1 << 10) 
#define DW_DATA_CMD_STOP					(1 << 9) 
#define DW_DATA_CMD_CMD		      			(1 << 8) 
#define DW_DATA_CMD_DAT(value)      		(value << 0) 	//b7~b0

/*IC_INTR_MASK*/
#define DW_INTR_MASK_RX_UNDER				(1 << 0)
#define DW_INTR_MASK_RX_OVER				(1 << 1)
#define DW_INTR_MASK_RX_FULL				(1 << 2)
#define DW_INTR_MASK_TX_OVER				(1 << 3)
#define DW_INTR_MASK_TX_EMPTY				(1 << 4)
#define DW_INTR_MASK_RD_REQ					(1 << 5)
#define DW_INTR_MASK_TX_ABRT				(1 << 6)
#define DW_INTR_MASK_RX_DONE				(1 << 7)
#define DW_INTR_MASK_ACTIVITY				(1 << 8)
#define DW_INTR_MASK_STOP_DET				(1 << 9)
#define DW_INTR_MASK_START_DET				(1 << 10)
#define DW_INTR_MASK_GEN_CALL				(1 << 11)

#define DW_INTR_MASK_DEFAULT_MASK		(DW_INTR_MASK_RX_FULL | \
									 		DW_INTR_MASK_TX_EMPTY | \
					 						DW_INTR_MASK_TX_ABRT | \
						 					DW_INTR_MASK_STOP_DET)

/*DW IC_ENABLE*/
#define DW_I2C_MODULE_DIS(_extra_)			HAL_WRITE_UINT32(DW_IC_ENABLE(_extra_), 0))
#define DW_I2C_MODULE_EN(_extra_)			(HAL_WRITE_UINT32(DW_IC_ENABLE(_extra_), 1))

/*dw i2c speed mode */
#define DW_I2C_STANDARD_MODE				(1)
#define DW_I2C_FAST_MODE					(2)
#define DW_I2C_HIGH_SPPED_MODE				(3)

/** I2c debug option **/
//#define I2C_DEBUG
#ifdef I2C_DEBUG
#define i2c_log_trace(FORMAT, ARGS...) diag_printf("<trace>: ""%s()""[%d]   "FORMAT, __FUNCTION__, __LINE__, ##ARGS)
#define i2c_log_debug(FORMAT, ARGS...) diag_printf("<debug>: ""%s()""[%d]   "FORMAT, __FUNCTION__, __LINE__, ##ARGS)
#define i2c_log_error(FORMAT, ARGS...) diag_printf("<error>: ""%s()""[%d]   "FORMAT, __FUNCTION__, __LINE__, ##ARGS)
#define i2c_log_enter(FORMAT, ARGS...) diag_printf("<enter>: ""%s()""[%d]   "FORMAT, __FUNCTION__, __LINE__, ##ARGS)
#define i2c_log_leave(FORMAT, ARGS...) diag_printf("<leave>: ""%s()""[%d]   "FORMAT, __FUNCTION__, __LINE__, ##ARGS)
#else
#define i2c_log_trace(FORMAT, ARGS...)do {} while (0)
#define i2c_log_debug(FORMAT, ARGS...)do {} while (0)
#define i2c_log_error(FORMAT, ARGS...)do {} while (0)
#define i2c_log_enter(FORMAT, ARGS...)do {} while (0)
#define i2c_log_leave(FORMAT, ARGS...)do {} while (0)
#endif

typedef struct cyg_gxi2c_extra {
	// Put statically initialized fields first.
	cyg_uint32       i2c_base;    // Per-bus h/w details
	cyg_vector_t     i2c_isrvec;  // used
	int              i2c_isrpri;  // used
	cyg_handle_t     i2c_interrupt_handle;   // For initializing the interrupt
	cyg_interrupt    i2c_interrupt_data;	
	cyg_uint32       i2c_pclk;    // peripheral clock: used
	cyg_uint32       i2c_bus_freq;// I2C bus frequency (e.g. 100 kHz, 400 kHz)

	cyg_uint32       i2c_rw_flag;
	cyg_uint32       i2c_data_num;    //write or read num

	cyg_uint16       i2c_addr;        //data addr
	cyg_uint16       i2c_dev_addr;    //dev addr

	cyg_drv_mutex_t  i2c_lock;
	cyg_drv_cond_t   i2c_wait;
} cyg_gxi2c_extra;

//==========================================================================
// GX I2C driver interface
//==========================================================================
externC void        cyg_gxi2c_init(struct cyg_i2c_bus*);
externC cyg_uint32  cyg_gxi2c_tx(const cyg_i2c_device*,
                                       cyg_bool, const cyg_uint8*,
                                       cyg_uint32, cyg_bool);
externC cyg_uint32  cyg_gxi2c_rx(const cyg_i2c_device *,
                                       cyg_bool, cyg_uint8 *,
                                       cyg_uint32, cyg_bool, cyg_bool);
externC void        cyg_gxi2c_stop(const cyg_i2c_device*);
externC void 		cyg_gxi2c_clk_cfg(const cyg_i2c_device *dev, 
										cyg_uint32 clk_value);

//==========================================================================
// DW I2C driver interface
//==========================================================================
externC void        cyg_dwi2c_init(struct cyg_i2c_bus*);
externC cyg_uint32  cyg_dwi2c_tx(const cyg_i2c_device*,
                                       cyg_bool, const cyg_uint8*,
                                       cyg_uint32, cyg_bool);
externC cyg_uint32  cyg_dwi2c_rx(const cyg_i2c_device *,
                                       cyg_bool, cyg_uint8 *,
                                       cyg_uint32, cyg_bool, cyg_bool);
externC void        cyg_dwi2c_stop(const cyg_i2c_device*);
externC void 		cyg_dwi2c_clk_cfg(const cyg_i2c_device *dev, 
										cyg_uint32 clk_value);

#endif //_CX_ARM_GXI2C_H_

